#pragma once

#include <string>

struct ICommand {
    virtual ~ICommand() = default;
};

struct ContainerCommand : ICommand {
    ContainerCommand(const std::string& command);

    std::string getCommand() const;
private:
    std::string _command;
};

struct StartBlockCommand : ICommand {};

struct EndBlockCommand : ICommand {};

struct EOFCommand : ICommand {};