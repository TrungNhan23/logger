#pragma once

#include <string>
#include "logger.h" 

class LogFormatter {
public: 
    static std::string format(logLevel level, std::string message);    
};
