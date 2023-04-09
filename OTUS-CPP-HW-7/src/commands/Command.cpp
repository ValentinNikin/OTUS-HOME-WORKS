#include "Command.h"

ContainerCommand::ContainerCommand(const std::string& command)
    : _command(command) {}

std::string ContainerCommand::getCommand() const {
    return _command;
}