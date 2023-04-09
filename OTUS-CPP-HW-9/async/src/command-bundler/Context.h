#pragma once

#include <memory>
#include <mutex>

#include "State.h"
#include "types/Block.h"

struct Context {
    Context(const std::size_t targetBlockSize);

    bool blockIsReady() const;
    std::unique_ptr<Block> resetBlock();
    void addCommandToBlock(const std::string& command);

    void setState(const State state);
    State getState() const;

    void incDynamicBlockNestingLevel();
    void decDynamicBlockNestingLevel();

    std::mutex mutex;
private:
    /// Собираемый блок
    std::unique_ptr<Block> _block;

    /// Размер блока команд для отдачи его в обработку
    const std::size_t _targetBlockSize;

    /// Текущее состояние сборщика
    State _state {State::NormalBlock};

    /// Уровень вложенности для блока динамического размера
    std::size_t _dynamicBlockNestingLevel {0};
};