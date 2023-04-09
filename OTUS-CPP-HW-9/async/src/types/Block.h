#pragma once

#include <vector>
#include <string>

class Block {
public:
    void addCommand(const std::string& command);

    std::size_t size() const;
    bool empty() const;

    std::size_t getFillingStartTime() const;
    std::vector<std::string> getCommands() const;
private:
    std::size_t _fillingStartTimePoint;
    std::vector<std::string> _commands;
};

