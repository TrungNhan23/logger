#include <fstream>
#include <ios>
#include <ostream>
#include <string>

#include "FileBackend.h"

namespace Helper
{
namespace Logger
{

FileBackend::FileBackend(const std::string &fileName)
    : m_fileName(fileName, std::ios::app)
{
}

void FileBackend::write(const std::string &formattedMessage)
{
  if(m_fileName.is_open())
    {
      m_fileName << formattedMessage  << std::endl;
    }
}

} // namespace Logger
} // namespace Helper