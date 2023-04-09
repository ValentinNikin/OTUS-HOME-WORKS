#pragma once

#include <memory>

#include "types/Block.h"

class IBlockProcessor {
public:
    virtual ~IBlockProcessor() = default;
    virtual void processBlock(std::shared_ptr<Block> block) = 0;
};