#pragma once

#include <memory>

#include "server/Server.h"
#include "server/request-handlers/CommandsRequestHandlerFactory.h"
#include "command-bundler/CommandBundler.h"

class App {
public:
    App(const ushort port, const std::size_t blockSize);
    void run();
private:
    std::shared_ptr<Server> _server;
    std::shared_ptr<CommandsRequestHandlerFactory> _requestHandlersFactory;
    std::shared_ptr<CommandBundler> _commandBundler;
};
