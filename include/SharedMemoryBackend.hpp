#pragma once // NOLINT(llvm-header-guard)

#include <cstddef>
#include <cstdint>
#include <string>

#include "ILogBackend.hpp"

namespace Helper::Logger
{

/**
 * @brief Magic number for shared memory header validation ("LOGR").
 */
constexpr uint32_t SHM_MAGIC = 0x4C4F4752;

/**
 * @brief Current shared memory layout version.
 */
constexpr uint32_t SHM_VERSION = 1;

/**
 * @brief Default shared memory buffer size (1 MB).
 */
constexpr size_t SHM_DEFAULT_BUFFER_SIZE = 1024 * 1024;

/**
 * @brief Default shared memory object name.
 */
constexpr const char* SHM_DEFAULT_NAME = "/logger_crash_buffer";

/**
 * @struct SharedMemoryHeader
 * @brief Header structure for the shared memory ring buffer.
 *
 * This header is placed at the beginning of the shared memory region
 * and contains metadata for managing the circular log buffer.
 * The structure is designed to be cache-line aligned (64 bytes).
 */
struct SharedMemoryHeader
{
    uint32_t magic;          ///< Magic number for validation (SHM_MAGIC)
    uint32_t version;        ///< Layout version for compatibility checks
    uint64_t capacity;       ///< Size of the data buffer in bytes
    uint64_t write_pos;      ///< Current write position in the ring buffer
    uint64_t total_written;  ///< Total bytes ever written (for wrap detection)
    uint32_t pid;            ///< PID of the writer process
    uint8_t wrapped;         ///< Whether the buffer has wrapped around
    uint8_t padding[23];     ///< Padding to 64 bytes for cache alignment
};

static_assert(sizeof(SharedMemoryHeader) == 64, "SharedMemoryHeader must be 64 bytes");

/**
 * @class SharedMemoryBackend
 * @brief Log backend that writes to POSIX shared memory for crash recovery.
 *
 * This backend continuously writes formatted log messages into a
 * kernel-backed shared memory ring buffer. Because the shared memory
 * resides outside the process address space (in /dev/shm), log data
 * survives sudden process crashes (SIGSEGV, SIGABRT, std::terminate).
 *
 * After a crash, logs can be recovered using the static recovery API:
 *   std::string logs = SharedMemoryBackend::recoverLogs();
 *
 * The ring buffer uses a simple circular write pattern with no locking
 * overhead (single-writer model assumed).
 *
 * @note This backend is Linux-only (requires POSIX shared memory).
 */
class SharedMemoryBackend : public ILogBackend
{
public:
    /**
     * @brief Constructs a SharedMemoryBackend.
     *
     * Creates or opens a POSIX shared memory object and maps it
     * into the process address space. Initializes the ring buffer
     * header if creating a new shared memory region.
     *
     * @param shmName    Name of the shared memory object (must start with '/').
     * @param bufferSize Size of the ring buffer data area in bytes.
     * @throws std::runtime_error if shared memory cannot be created or mapped.
     */
    explicit SharedMemoryBackend(
        const std::string& shmName = SHM_DEFAULT_NAME,
        size_t bufferSize = SHM_DEFAULT_BUFFER_SIZE);

    /**
     * @brief Destructor. Unmaps the shared memory region.
     *
     * @note The shared memory object is NOT unlinked on destruction
     * so that crash recovery can read it after the process exits.
     */
    ~SharedMemoryBackend() override;

    SharedMemoryBackend(const SharedMemoryBackend&) = delete;
    SharedMemoryBackend& operator=(const SharedMemoryBackend&) = delete;
    SharedMemoryBackend(SharedMemoryBackend&&) = delete;
    SharedMemoryBackend& operator=(SharedMemoryBackend&&) = delete;

    /**
     * @brief Writes a formatted log message to the ring buffer.
     *
     * Appends the message (with newline) to the circular buffer.
     * If the buffer wraps around, older data is overwritten.
     *
     * @param formattedMsg The fully formatted log message.
     */
    void write(const std::string& formattedMsg) override;

    /**
     * @brief Recovers log data from a shared memory crash buffer.
     *
     * Opens an existing shared memory object in read-only mode,
     * validates the header, and extracts the log contents in
     * chronological order.
     *
     * @param shmName Name of the shared memory object.
     * @return The recovered log content as a string.
     *         Returns empty string if no valid crash buffer exists.
     */
    static std::string recoverLogs(const std::string& shmName = SHM_DEFAULT_NAME);

    /**
     * @brief Checks whether a crash log buffer exists and contains data.
     *
     * @param shmName Name of the shared memory object.
     * @return true if a valid crash buffer with data exists.
     */
    static bool hasCrashLogs(const std::string& shmName = SHM_DEFAULT_NAME);

    /**
     * @brief Removes the shared memory object.
     *
     * Call this after successfully recovering logs to clean up.
     *
     * @param shmName Name of the shared memory object.
     */
    static void clearLogs(const std::string& shmName = SHM_DEFAULT_NAME);

private:
    int m_shmFd { -1 };
    void* m_mappedRegion { nullptr };
    size_t m_totalSize { 0 };
    SharedMemoryHeader* m_header { nullptr };
    char* m_data { nullptr };
    std::string m_shmName;
};

} // namespace Helper::Logger
