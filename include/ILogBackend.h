#pragma once

#include <string>

class ILogBackend
{
public:
  ILogBackend() = default;
  ILogBackend(const ILogBackend &) = default;
  auto operator=(const ILogBackend &) -> ILogBackend & = default;
  ILogBackend(ILogBackend &&) = default;
  auto operator=(ILogBackend &&) -> ILogBackend & = default;
  virtual void write(const std::string &formattedMsg) = 0;
  virtual ~ILogBackend() = default;
};