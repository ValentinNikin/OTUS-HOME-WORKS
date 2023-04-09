#include "Command.h"

ContainerCommand::ContainerCommand(const std::string& payload)
    : _payload(payload) {}

std::string ContainerCommand::getPayload() const {
    return _payload;
}