#include "FileBackend.h"
#include <string>
#include <fstream>
#include <ios> 
#include <ostream> 


FileBackend::FileBackend(const std::string& filename)
    : file(filename, std::ios::app)
{}


void FileBackend::write(const std::string& formattedMsg)
{
    if (file.is_open()) 
    {
        file << formattedMsg << std::endl;
    }

}
