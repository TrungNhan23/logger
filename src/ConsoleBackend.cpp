#include "ConsoleBackend.hpp"

#include <iostream>
#include <string>

namespace Helper
{
namespace Logger
{

void ConsoleBackend::write(const std::string& formattedMsg)
{
    std::cout << formattedMsg << '\n';
}

} // namespace Logger
} // namespace Helper