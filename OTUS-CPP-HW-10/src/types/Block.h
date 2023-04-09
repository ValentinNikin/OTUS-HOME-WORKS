#pragma once

#include <vector>
#include <string>

class Block {
public:
    Block(const std::size_t contextId);

    void addCommand(const std::string& command);

    std::size_t size() const;
    bool empty() const;

    std::size_t getFillingStartTime() const;
    std::vector<std::string> getCommands() const;

    std::size_t getContextId() const;
private:
    std::size_t _fillingStartTimePoint;
    std::vector<std::string> _commands;

    /// Id контекста, который собрал данный блок.
    /// Введено для удобства отладки
    std::size_t _contextId;
};

