#include "CommandBundler.h"

#include <cassert>
#include <stdexcept>

CommandBundler::CommandBundler(std::size_t targetBlockSize)
    : _targetBlockSize(targetBlockSize) {
    if (_targetBlockSize < 1) {
        throw std::runtime_error("Block size should be more than or equal to 1");
    }

    _block = std::make_shared<Block>();
}

void CommandBundler::addProcessor(const std::shared_ptr<ICommandProcessor>& processor) {
    _processors.push_back(processor);
}

bool CommandBundler::isEof() const {
    return _state == State::Eof;
}

void CommandBundler::processCommand(std::unique_ptr<ICommand> command) {
    if (isEof()) {
        throw std::runtime_error("Command bundler stopped by EOF command");
    }

    /**
     * Была задумка реализовать через паттерн visitor, но не стал этого делать намеренно,
     * чтобы производные ICommand не содержали дополнительной логики, а также чтобы не нарушать
     * инкапсуляцию CommandBundler.
     */
    if (auto cmd = dynamic_cast<ContainerCommand*>(command.get())) {
        collectCommand(cmd);
    }
    else if (dynamic_cast<StartBlockCommand*>(command.get())) {
        startDynamicBlock();
    }
    else if (dynamic_cast<EndBlockCommand*>(command.get())) {
        endDynamicBlock();
    }
    else if (dynamic_cast<EOFCommand*>(command.get())) {
        eof();
    }
    else {
        throw std::runtime_error("Unsupported command");
    }
}

void CommandBundler::collectCommand(ContainerCommand* cmd) {
    _block->addCommand(cmd->getCommand());

    if (_state == State::NormalBlock && _block->size() == _targetBlockSize) {
        // Требуемый размер блока достигнут
        sendBlockToProcessors();
    }
}

void CommandBundler::startDynamicBlock() {
    if (_state == State::NormalBlock) {
        // Принудительно завершаем предыдущий блок команд
        sendBlockToProcessors();
        _state = State::DynamicBlock;
    }

    _dynamicBlockNestingLevel++;
}

void CommandBundler::endDynamicBlock() {
    if (_dynamicBlockNestingLevel == 0) {
        throw std::runtime_error("Dynamic block wasn't open");
    }

    _dynamicBlockNestingLevel--;

    if (_dynamicBlockNestingLevel == 0) {
        // Динамический блок закрыт
        sendBlockToProcessors();
        _state = State::NormalBlock;
    }
}

void CommandBundler::eof() {
    if (_state == State::NormalBlock) {
        sendBlockToProcessors();
    }
    // Если команды закончились внутри динамического блока, собранный блок будет проигнорирован

    _state = State::Eof;
}

void CommandBundler::sendBlockToProcessors() {
    if (!_block->empty()) {
        for (const auto& p : _processors) {
            p->processCommands(_block);
        }
    }
    _block = std::make_shared<Block>();
}