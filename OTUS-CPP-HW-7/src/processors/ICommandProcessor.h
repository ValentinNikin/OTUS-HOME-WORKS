#pragma once

#include <memory>

#include "../commands/Block.h"

class ICommandProcessor {
public:
    virtual ~ICommandProcessor() = default;
    virtual void processCommands(std::shared_ptr<Block> block) = 0;
};

