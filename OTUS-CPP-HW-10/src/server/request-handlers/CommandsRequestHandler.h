#pragma once

#include "server/IRequestHandler.h"
#include "command-bundler/CommandBundler.h"
#include "command-bundler/Context.h"

class CommandsRequestHandler : public IRequestHandler {
public:
    CommandsRequestHandler(const std::shared_ptr<CommandBundler>& commandBundler);
    ~CommandsRequestHandler();

    void processData(const char* data, std::size_t size) override;
private:
    std::shared_ptr<CommandBundler> _commandBundler;
    handle_t _handle;
};
