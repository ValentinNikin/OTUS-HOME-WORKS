#pragma once

#include <stdlib.h>

class IRequestHandler {
public:
    virtual ~IRequestHandler() = default;

    virtual void processData(const char* data, std::size_t size) = 0;
};
