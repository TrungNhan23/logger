#include "ConsoleBackend.h"

#include <iostream>
#include <string>

void ConsoleBackend::write(const std::string &formattedMsg)
{
  std::cout << formattedMsg << std::endl;
}
