#pragma once

#include <string>
#include "logger.h" 

class LogFormatter {
public: 
    static auto format(logLevel level, std::string message) -> std::string;    
};
