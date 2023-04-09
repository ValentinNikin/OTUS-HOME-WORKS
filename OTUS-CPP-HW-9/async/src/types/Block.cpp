#include "Block.h"

#include "utils/Utils.h"

void Block::addCommand(const std::string& command) {
    if (_commands.empty()) {
        _fillingStartTimePoint = Utils::getCurrentTimeInMilliseconds();
    }

    _commands.push_back(command);
}

std::size_t Block::size() const {
    return _commands.size();
}

bool Block::empty() const {
    return _commands.empty();
}

std::size_t Block::getFillingStartTime() const {
    return _fillingStartTimePoint;
}

std::vector<std::string> Block::getCommands() const {
    return _commands;
}