#pragma once

#include <string>


class ILogBackend {
public:
    virtual void write(std::string formattedMsg) = 0; 
};