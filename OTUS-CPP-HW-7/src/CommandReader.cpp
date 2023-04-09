#include "CommandReader.h"

#include <string>
#include <iostream>

#include "commands/Command.h"

CommandReader::CommandReader(std::unique_ptr<CommandBundler> bundler)
    : _bundler(std::move(bundler)) {}

void CommandReader::run() {
    std::string line;
    while (!_bundler->isEof()) {
        std::getline(std::cin, line);

        std::unique_ptr<ICommand> command;
        if (line == "{") {
            command = std::make_unique<StartBlockCommand>();
        }
        else if (line == "}") {
            command = std::make_unique<EndBlockCommand>();
        }
        else if (line == "EOF") {
            command = std::make_unique<EOFCommand>();
        }
        else {
            command = std::make_unique<ContainerCommand>(line);
        }

        _bundler->processCommand(std::move(command));
    }
}