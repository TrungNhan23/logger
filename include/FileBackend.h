#pragma once

#include <fstream>
#include <string>

#include "ILogBackend.h"

class FileBackend : public ILogBackend
{
public:
  explicit FileBackend(const std::string &filename);
  void write(const std::string &formattedMsg) override;

private:
  // std::string filePath;
  std::ofstream file;
};
