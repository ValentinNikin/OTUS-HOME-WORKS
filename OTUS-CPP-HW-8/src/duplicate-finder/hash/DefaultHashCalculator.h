#pragma once

#include "HashCalculator.h"

class DefaultHashCalculator : public HashCalculator {
public:
    std::size_t calculateHash(uint8_t* data, std::size_t size) override;
};