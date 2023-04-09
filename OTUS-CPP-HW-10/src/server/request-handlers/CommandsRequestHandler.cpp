#include "CommandsRequestHandler.h"

#include "utils/Utils.h"

CommandsRequestHandler::CommandsRequestHandler(
        const std::shared_ptr<CommandBundler>& commandBundler)
        : _commandBundler{commandBundler} {
    _handle = _commandBundler->createExecutionContext();
}

CommandsRequestHandler::~CommandsRequestHandler() {
    _commandBundler->removeExecutionContext(_handle);
}

void CommandsRequestHandler::processData(const char* data, std::size_t size) {
    std::string bufferStr;
    bufferStr.assign(data, size);

    auto commandsStr = Utils::split(bufferStr, '\n');

    for (const auto& c : commandsStr) {
        std::unique_ptr<ICommand> command;
        if (c == "{") {
            command = std::make_unique<StartBlockCommand>();
        }
        else if (c == "}") {
            command = std::make_unique<EndBlockCommand>();
        }
        else {
            command = std::make_unique<ContainerCommand>(c);
        }

        _commandBundler->processCommand(_handle, std::move(command));
    }
}
