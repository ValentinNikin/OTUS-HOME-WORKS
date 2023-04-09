#include "CommandsRequestHandlerFactory.h"

#include "CommandsRequestHandler.h"

CommandsRequestHandlerFactory::CommandsRequestHandlerFactory(
        const std::shared_ptr<CommandBundler>& commandBundler)
        : _commandBundler{commandBundler} {}

std::unique_ptr<IRequestHandler> CommandsRequestHandlerFactory::buildRequestHandler() {
    return std::make_unique<CommandsRequestHandler>(_commandBundler);
}