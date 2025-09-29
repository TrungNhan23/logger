#include "ConsoleBackend.h"
#include <string>
#include <iostream>

void ConsoleBackend::write(const std::string& formattedMsg)
{
    std::cout << formattedMsg << std::endl; 
}
