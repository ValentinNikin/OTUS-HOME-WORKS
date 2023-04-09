#pragma once

#include "server/IRequestHandlerFactory.h"
#include "command-bundler/CommandBundler.h"

class CommandsRequestHandlerFactory : public IRequestHandlerFactory {
public:
    CommandsRequestHandlerFactory(const std::shared_ptr<CommandBundler>& commandBundler);

    std::unique_ptr<IRequestHandler> buildRequestHandler() override;
private:
    std::shared_ptr<CommandBundler> _commandBundler;
};
