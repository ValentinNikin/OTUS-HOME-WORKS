#pragma once

#include <string>

struct ICommand {
    virtual ~ICommand() = default;
};

struct ContainerCommand : ICommand {
    ContainerCommand(const std::string& payload);

    std::string getPayload() const;
private:
    std::string _payload;
};

struct StartBlockCommand : ICommand {};

struct EndBlockCommand : ICommand {};

struct EOFCommand : ICommand {};