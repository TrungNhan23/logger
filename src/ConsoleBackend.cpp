#include "ConsoleBackend.hpp"

#include <iostream>
#include <string>

namespace Helper::Logger
{

void ConsoleBackend::write(const std::string& formattedMsg)
{
    std::cout << formattedMsg << '\n';
}

void ConsoleBackend::flush()
{
    // Currently we rely on std::cout's internal buffering and flushing behavior.
    // If needed, we can explicitly flush the output stream here.
    std::cout.flush();
}

} // namespace Helper::Logger
