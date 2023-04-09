#pragma once

#include <stdlib.h>

#include <boost/asio/buffer.hpp>

class IRequestHandler {
public:
    virtual ~IRequestHandler() = default;

    virtual std::vector<std::string> processData(const char* data, std::size_t size) = 0;
};
