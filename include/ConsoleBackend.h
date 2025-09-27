#pragma once

#include <string>
#include <iostream>
#include "ILogBackend.h" 

class ConsoleBackend : public ILogBackend {
public: 
    void write(const std::string& formattedMsg) override; 
};
