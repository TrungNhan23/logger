#include <gtest/gtest.h>

#include "SharedMemoryBackend.hpp"

#include <string>

// ============================================================
// Shared Memory Backend Tests (Feature #7)
// ============================================================

// Use a unique name per test to avoid interference
static const std::string TEST_SHM_NAME = "/logger_test_shm";

class SharedMemoryBackendTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Clean up any leftover shared memory from previous test runs
        Helper::Logger::SharedMemoryBackend::clearLogs(TEST_SHM_NAME);
    }

    void TearDown() override
    {
        Helper::Logger::SharedMemoryBackend::clearLogs(TEST_SHM_NAME);
    }
};

TEST_F(SharedMemoryBackendTest, Construction_CreatesSharedMemory)
{
    EXPECT_NO_THROW({
        Helper::Logger::SharedMemoryBackend backend(TEST_SHM_NAME, 4096);
    });
}

TEST_F(SharedMemoryBackendTest, Write_DataPersistsAfterDestruction)
{
    {
        Helper::Logger::SharedMemoryBackend backend(TEST_SHM_NAME, 4096);
        backend.write("test message 1");
        backend.write("test message 2");
    }
    // Backend destroyed, but data should persist in shared memory

    EXPECT_TRUE(Helper::Logger::SharedMemoryBackend::hasCrashLogs(TEST_SHM_NAME));

    std::string recovered = Helper::Logger::SharedMemoryBackend::recoverLogs(TEST_SHM_NAME);
    EXPECT_NE(recovered.find("test message 1"), std::string::npos)
        << "Should find message 1 in recovered logs: " << recovered;
    EXPECT_NE(recovered.find("test message 2"), std::string::npos)
        << "Should find message 2 in recovered logs: " << recovered;
}

TEST_F(SharedMemoryBackendTest, ClearLogs_RemovesSharedMemory)
{
    {
        Helper::Logger::SharedMemoryBackend backend(TEST_SHM_NAME, 4096);
        backend.write("data to clear");
    }

    EXPECT_TRUE(Helper::Logger::SharedMemoryBackend::hasCrashLogs(TEST_SHM_NAME));

    Helper::Logger::SharedMemoryBackend::clearLogs(TEST_SHM_NAME);

    EXPECT_FALSE(Helper::Logger::SharedMemoryBackend::hasCrashLogs(TEST_SHM_NAME));
}

TEST_F(SharedMemoryBackendTest, HasCrashLogs_FalseWhenEmpty)
{
    EXPECT_FALSE(Helper::Logger::SharedMemoryBackend::hasCrashLogs(TEST_SHM_NAME));
}

TEST_F(SharedMemoryBackendTest, HasCrashLogs_FalseForNonexistent)
{
    EXPECT_FALSE(Helper::Logger::SharedMemoryBackend::hasCrashLogs("/nonexistent_shm_test"));
}

TEST_F(SharedMemoryBackendTest, RecoverLogs_EmptyForNonexistent)
{
    std::string result = Helper::Logger::SharedMemoryBackend::recoverLogs("/nonexistent_shm_test");
    EXPECT_TRUE(result.empty());
}

TEST_F(SharedMemoryBackendTest, RingBuffer_WrapsCorrectly)
{
    // Use a very small buffer to force wrapping
    constexpr size_t SMALL_BUFFER = 128;
    {
        Helper::Logger::SharedMemoryBackend backend(TEST_SHM_NAME, SMALL_BUFFER);

        // Write enough data to wrap the buffer multiple times
        for (int i = 0; i < 20; ++i)
        {
            backend.write("message number " + std::to_string(i));
        }
    }

    std::string recovered = Helper::Logger::SharedMemoryBackend::recoverLogs(TEST_SHM_NAME);

    // Should contain some of the later messages (earlier ones are overwritten)
    // The exact messages depend on the wrap, but the recovered data should
    // be non-empty and contain valid text
    EXPECT_FALSE(recovered.empty()) << "Recovered logs should not be empty after wrapping";
    EXPECT_NE(recovered.find("message number"), std::string::npos)
        << "Should contain log message fragments: " << recovered;
}

TEST_F(SharedMemoryBackendTest, MultipleWrites_PreserveOrder)
{
    {
        Helper::Logger::SharedMemoryBackend backend(TEST_SHM_NAME, 8192);
        backend.write("first");
        backend.write("second");
        backend.write("third");
    }

    std::string recovered = Helper::Logger::SharedMemoryBackend::recoverLogs(TEST_SHM_NAME);

    auto posFirst = recovered.find("first");
    auto posSecond = recovered.find("second");
    auto posThird = recovered.find("third");

    EXPECT_NE(posFirst, std::string::npos);
    EXPECT_NE(posSecond, std::string::npos);
    EXPECT_NE(posThird, std::string::npos);

    EXPECT_LT(posFirst, posSecond) << "first should come before second";
    EXPECT_LT(posSecond, posThird) << "second should come before third";
}
