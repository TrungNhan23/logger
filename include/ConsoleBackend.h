#pragma once

#include <string>
#include "ILogBackend.h" 

class ConsoleBackend : public ILogBackend {
public: 
    void write(std::string formattedMsg) override; 
};
