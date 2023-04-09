#include "Server.h"

#include "processors/MultiThreadProcessor.h"
#include "processors/threads/ConsoleThreadProcessor.h"
#include "processors/threads/FileThreadProcessor.h"

#include "command-bundler/Command.h"

#include "utils/Utils.h"

using handle_t = async::handle_t;

Server::Server() {}

std::unique_ptr<Server> Server::_instance;
std::mutex Server::_mutex;

Server* Server::getInstance() {
    if (_instance != nullptr) {
        return _instance.get();
    }

    std::lock_guard<std::mutex> lg(_mutex);
    if (_instance == nullptr) {
        _instance.reset(new Server());

        // create and initialize command bundler
        auto commandBundler = std::make_unique<CommandBundler>();

        auto consoleMultiThreadProcessor = std::make_shared<MultiThreadProcessor>();
        consoleMultiThreadProcessor->addThread(std::make_unique<ConsoleThreadProcessor>("log"));
        commandBundler->addProcessor(consoleMultiThreadProcessor);

        auto fileMultiThreadProcessor = std::make_shared<MultiThreadProcessor>();
        fileMultiThreadProcessor->addThread(std::make_unique<FileThreadProcessor>("file1"));
        fileMultiThreadProcessor->addThread(std::make_unique<FileThreadProcessor>("file2"));
        commandBundler->addProcessor(fileMultiThreadProcessor);

        _instance->_commandBundler = std::move(commandBundler);
    }

    return _instance.get();
}

handle_t Server::connect(std::size_t blockSize) {
    return _commandBundler->createExecutionContext(blockSize);
}

void Server::receive(handle_t handle, const char *data, std::size_t size) {
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
        _commandBundler->processCommand(handle, std::move(command));
    }
}

void Server::disconnect(handle_t handle) {
    _commandBundler->removeExecutionContext(handle);
}