#pragma once

#include <string>
#include "logger.h" 

class LogFormater {
public: 
    std::string format(logLevel level, std::string message);    
}