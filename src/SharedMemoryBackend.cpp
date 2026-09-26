#include "SharedMemoryBackend.hpp"

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>

#ifdef __linux__
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace Helper::Logger
{

#ifdef __linux__

SharedMemoryBackend::SharedMemoryBackend(const std::string& shmName, size_t bufferSize)
    : m_totalSize(sizeof(SharedMemoryHeader) + bufferSize)
    , m_shmName(shmName)
{
    // Create or open the shared memory object
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    m_shmFd = shm_open(shmName.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (m_shmFd == -1)
    {
        throw std::runtime_error(
            "SharedMemoryBackend: Failed to open shared memory '" + shmName +
            "': " + std::strerror(errno));
    }

    // Set the size of the shared memory region
    if (ftruncate(m_shmFd, static_cast<off_t>(m_totalSize)) == -1)
    {
        close(m_shmFd);
        throw std::runtime_error(
            "SharedMemoryBackend: Failed to set shared memory size: " +
            std::string(std::strerror(errno)));
    }

    // Map the shared memory into process address space
    m_mappedRegion = mmap(nullptr, m_totalSize, PROT_READ | PROT_WRITE,
                          MAP_SHARED, m_shmFd, 0);
    if (m_mappedRegion == MAP_FAILED) // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,performance-no-int-to-ptr)
    {
        close(m_shmFd);
        throw std::runtime_error(
            "SharedMemoryBackend: Failed to mmap shared memory: " +
            std::string(std::strerror(errno)));
    }

    m_header = static_cast<SharedMemoryHeader*>(m_mappedRegion);
    m_data = static_cast<char*>(m_mappedRegion) + sizeof(SharedMemoryHeader);

    // Initialize header if this is a new or stale region
    if (m_header->magic != SHM_MAGIC || m_header->version != SHM_VERSION)
    {
        std::memset(m_header, 0, sizeof(SharedMemoryHeader));
        m_header->magic = SHM_MAGIC;
        m_header->version = SHM_VERSION;
        m_header->capacity = bufferSize;
        m_header->write_pos = 0;
        m_header->total_written = 0;
        m_header->pid = static_cast<uint32_t>(getpid());
        m_header->wrapped = 0;
    }
    else
    {
        // Existing valid buffer — update PID for this session
        m_header->pid = static_cast<uint32_t>(getpid());
    }
}

SharedMemoryBackend::~SharedMemoryBackend()
{
    if (m_mappedRegion != nullptr && m_mappedRegion != MAP_FAILED) // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,performance-no-int-to-ptr)
    {
        munmap(m_mappedRegion, m_totalSize);
    }
    if (m_shmFd != -1)
    {
        close(m_shmFd);
    }
    // Intentionally NOT calling shm_unlink — we want the data
    // to survive for crash recovery
}

void SharedMemoryBackend::write(const std::string& formattedMsg)
{
    if (m_header == nullptr || m_data == nullptr)
    {
        return;
    }

    std::string line = formattedMsg + "\n";
    size_t len = line.size();
    uint64_t capacity = m_header->capacity;
    uint64_t pos = m_header->write_pos;

    // Circular write — byte by byte for wrap correctness
    for (size_t i = 0; i < len; ++i)
    {
        m_data[(pos + i) % capacity] = line[i];
    }

    m_header->write_pos = (pos + len) % capacity;
    m_header->total_written += len;
    if (m_header->total_written >= capacity)
    {
        m_header->wrapped = 1;
    }
}

std::string SharedMemoryBackend::recoverLogs(const std::string& shmName)
{
    // Open existing shared memory in read-only mode
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    int fd = shm_open(shmName.c_str(), O_RDONLY, 0);
    if (fd == -1)
    {
        return "";
    }

    // Get the size of the shared memory region
    struct stat sb{};
    if (fstat(fd, &sb) == -1 || sb.st_size < static_cast<off_t>(sizeof(SharedMemoryHeader)))
    {
        close(fd);
        return "";
    }

    auto totalSize = static_cast<size_t>(sb.st_size);

    // Map in read-only mode
    void* mapped = mmap(nullptr, totalSize, PROT_READ, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,performance-no-int-to-ptr)
    {
        close(fd);
        return "";
    }

    const auto* header = static_cast<const SharedMemoryHeader*>(mapped);
    const char* data = static_cast<const char*>(mapped) + sizeof(SharedMemoryHeader);

    std::string result;

    // Validate header
    if (header->magic == SHM_MAGIC && header->version == SHM_VERSION && header->total_written > 0)
    {
        uint64_t capacity = header->capacity;
        uint64_t writePos = header->write_pos;

        if (header->wrapped != 0)
        {
            // Buffer has wrapped — read from write_pos to end, then start to write_pos
            result.reserve(capacity);
            for (uint64_t i = 0; i < capacity; ++i)
            {
                result.push_back(data[(writePos + i) % capacity]);
            }
        }
        else
        {
            // Buffer hasn't wrapped — read from 0 to write_pos
            result.assign(data, writePos);
        }
    }

    munmap(mapped, totalSize);
    close(fd);
    return result;
}

bool SharedMemoryBackend::hasCrashLogs(const std::string& shmName)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    int fd = shm_open(shmName.c_str(), O_RDONLY, 0);
    if (fd == -1)
    {
        return false;
    }

    SharedMemoryHeader header{};
    ssize_t bytesRead = read(fd, &header, sizeof(header));
    close(fd);

    return (bytesRead == static_cast<ssize_t>(sizeof(header)) &&
            header.magic == SHM_MAGIC &&
            header.version == SHM_VERSION &&
            header.total_written > 0);
}

void SharedMemoryBackend::clearLogs(const std::string& shmName)
{
    shm_unlink(shmName.c_str());
}

#else

// Non-Linux stubs
SharedMemoryBackend::SharedMemoryBackend(const std::string& /*shmName*/, size_t /*bufferSize*/)
{
    throw std::runtime_error("SharedMemoryBackend is only supported on Linux");
}

SharedMemoryBackend::~SharedMemoryBackend() = default;
void SharedMemoryBackend::write(const std::string& /*formattedMsg*/) {}
std::string SharedMemoryBackend::recoverLogs(const std::string& /*shmName*/) { return ""; }
bool SharedMemoryBackend::hasCrashLogs(const std::string& /*shmName*/) { return false; }
void SharedMemoryBackend::clearLogs(const std::string& /*shmName*/) {}

#endif

} // namespace Helper::Logger
