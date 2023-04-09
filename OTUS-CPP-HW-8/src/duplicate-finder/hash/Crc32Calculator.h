#pragma once

#include "HashCalculator.h"

class Crc32Calculator: public HashCalculator {
public:
    std::size_t calculateHash(uint8_t* data, std::size_t size) override;
};