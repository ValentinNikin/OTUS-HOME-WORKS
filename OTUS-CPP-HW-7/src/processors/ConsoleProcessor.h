#pragma once

#include "ICommandProcessor.h"

class ConsoleProcessor : public ICommandProcessor {
public:
    void processCommands(std::shared_ptr<Block> block) override;
};
