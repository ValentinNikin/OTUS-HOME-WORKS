#pragma once

#include <memory>

#include "IRequestHandler.h"

class IRequestHandlerFactory {
public:
    virtual ~IRequestHandlerFactory() = default;

    virtual std::unique_ptr<IRequestHandler> buildRequestHandler() = 0;
};