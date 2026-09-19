#include "LogProvider.hpp"

#include <mutex>

namespace Helper::Logger
{
namespace
{
std::shared_ptr<Logger>& provider_logger()
{
    static std::shared_ptr<Logger> instance;
    return instance;
}

std::function<std::shared_ptr<Logger>()>& provider_factory()
{
    static std::function<std::shared_ptr<Logger>()> f;
    return f;
}

std::mutex& provider_mutex()
{
    static std::mutex m;
    return m;
}

} // namespace

void setLoggerFactory(std::function<std::shared_ptr<Logger>()> factory)
{
    std::lock_guard<std::mutex> lk(provider_mutex());
    provider_factory() = std::move(factory);
    provider_logger().reset(); // force recreate on next defaultLogger() call
}

void setDefaultLogger(std::shared_ptr<Logger> logger)
{
    std::lock_guard<std::mutex> lk(provider_mutex());
    provider_logger() = std::move(logger);
}

std::shared_ptr<Logger> defaultLogger()
{
    std::lock_guard<std::mutex> lk(provider_mutex());
    auto& g_logger = provider_logger();
    auto& g_factory = provider_factory();
    if (g_logger)
    {
        return g_logger;
    }
    if (!g_factory)
    {
        return nullptr;
    }
    g_logger = g_factory();
    return g_logger;
}

} // namespace Helper::Logger
