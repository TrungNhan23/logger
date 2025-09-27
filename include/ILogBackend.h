#pragma once

#include <string>


class ILogBackend {
public:
    virtual void write(const std::string& formattedMsg) = 0;
    virtual ~ILogBackend() = default;
};