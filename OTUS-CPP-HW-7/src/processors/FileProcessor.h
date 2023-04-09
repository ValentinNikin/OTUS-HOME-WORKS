#pragma once

#include "ICommandProcessor.h"

class FileProcessor : public ICommandProcessor {
public:
    void processCommands(std::shared_ptr<Block> block) override;
};