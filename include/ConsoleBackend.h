#pragma once

#include <string>

#include "ILogBackend.h"

class ConsoleBackend : public ILogBackend
{
public:
  void write(const std::string &formattedMsg) override;
};
