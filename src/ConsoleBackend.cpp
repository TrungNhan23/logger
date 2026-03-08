#include <iostream>
#include <string>

#include "ConsoleBackend.h"


namespace Helper
{
namespace Logger
{

void ConsoleBackend::write(const std::string &formattedMsg)
{
  std::cout << formattedMsg << std::endl;
}

} // namespace Logger
} // namespace Helper