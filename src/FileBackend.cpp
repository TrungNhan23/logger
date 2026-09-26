#include "FileBackend.hpp"

#include <fstream>
#include <ios>
#include <ostream>
#include <string>

namespace Helper::Logger
{

FileBackend::FileBackend(const std::string& fileName)
    : m_fileName(fileName, std::ios::app)
{
}

void FileBackend::write(const std::string& formattedMessage)
{
    if (m_fileName.is_open())
    {
        m_fileName << formattedMessage << std::endl;
    }
}

void FileBackend::flush()
{
    if (m_fileName.is_open())
    {
        m_fileName.flush();
    }
}

} // namespace Helper::Logger
