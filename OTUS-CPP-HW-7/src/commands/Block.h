#pragma once

#include <vector>
#include <string>

class Block {
public:
    void addCommand(const std::string& command);

    std::size_t size() const;
    bool empty() const;

    time_t getFillingStartTime() const;
    std::vector<std::string> getCommands() const;
private:
    time_t _fillingStartTime;
    std::vector<std::string> _commands;
};

