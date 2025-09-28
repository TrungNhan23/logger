#include "ConsoleBackend.h"

void ConsoleBackend::write(const std::string& formattedMsg)
{
    std::cout << formattedMsg << std::endl; 
}
