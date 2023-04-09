#pragma once

#include <vector>
#include <memory>

#include "processors/ICommandProcessor.h"
#include "commands/Command.h"
#include "commands/Block.h"

class CommandBundler {
public:
    CommandBundler(std::size_t targetBlockSize);

    void addProcessor(const std::shared_ptr<ICommandProcessor>& processor);

    void processCommand(std::unique_ptr<ICommand> command);

    bool isEof() const;

private:
    std::vector<std::shared_ptr<ICommandProcessor>> _processors;
    std::shared_ptr<Block> _block;

    /// Отправить текущий блок команд в обработчики
    void sendBlockToProcessors();

    void collectCommand(ContainerCommand* cmd);
    void startDynamicBlock();
    void endDynamicBlock();
    void eof();

    enum class State {
        NormalBlock,
        DynamicBlock,
        Eof
    };

    /// Размер блока команд для отдачи его в обработку
    const std::size_t _targetBlockSize;

    /// Уровень вложенности для блока динамического размера
    std::size_t _dynamicBlockNestingLevel {0};

    /// Текущее состояние сборщика
    State _state {State::NormalBlock};
};