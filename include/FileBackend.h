#pragma once

#include <string>
#include "ILogBackend.h" 

class FileBackend : public ILogBackend {
public: 
    void write(std::string formattedMsg) override; 
private: 
    std::string filePath;
};
