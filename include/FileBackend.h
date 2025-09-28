#pragma once

#include <string>
#include "ILogBackend.h" 
#include <fstream>

class FileBackend : public ILogBackend 
{
public: 
    FileBackend(const std::string& filename); 
    void write(const std::string& formattedMsg) override; 
private: 
    // std::string filePath;
    std::ofstream file;
};
