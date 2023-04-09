#include "Context.h"

#include <stdexcept>

std::size_t Context::_nextContextId = 0;

Context::Context(const std::size_t targetBlockSize)
    : _contextId{_nextContextId++},
    _block{std::make_unique<Block>(_contextId)},
    _targetBlockSize{targetBlockSize}{}

bool Context::blockIsReady() const {
    return _state == State::NormalBlock && _block->size() == _targetBlockSize;
}

std::unique_ptr<Block> Context::resetBlock() {
    auto tempBlock = std::make_unique<Block>(_contextId);
    _block.swap(tempBlock);
    return tempBlock;
}

void Context::addCommandToBlock(const std::string& command) {
    _block->addCommand(command);
}

void Context::setState(const State state) {
    _state = state;
}

State Context::getState() const {
    return _state;
}

void Context::incDynamicBlockNestingLevel() {
    if (_state == State::NormalBlock) {
        _state = State::DynamicBlock;
    }

    _dynamicBlockNestingLevel++;
}

void Context::decDynamicBlockNestingLevel() {
    _dynamicBlockNestingLevel--;

    if (_dynamicBlockNestingLevel == 0) {
        _state = State::NormalBlock;
    }
}