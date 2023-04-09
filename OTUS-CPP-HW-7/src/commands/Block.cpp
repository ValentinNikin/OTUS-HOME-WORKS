#include "Block.h"

void Block::addCommand(const std::string& command) {
    if (_commands.empty()) {
        _fillingStartTime = time(NULL);
    }

    _commands.push_back(command);
}

std::size_t Block::size() const {
    return _commands.size();
}

bool Block::empty() const {
    return _commands.empty();
}

time_t Block::getFillingStartTime() const {
    return _fillingStartTime;
}

std::vector<std::string> Block::getCommands() const {
    return _commands;
}