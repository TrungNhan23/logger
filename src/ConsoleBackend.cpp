#include "ConsoleBackend.hpp"

#include <iostream>
#include <string>

namespace Helper::Logger
{

void ConsoleBackend::write(const std::string& formattedMsg)
{
    std::cout << formattedMsg << '\n';
}

} // namespace Helper::Logger
