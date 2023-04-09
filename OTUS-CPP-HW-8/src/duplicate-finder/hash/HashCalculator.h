#pragma once

#include <cstdlib>
#include <cstdint>

class HashCalculator {
public:
    virtual ~HashCalculator() = default;
    virtual std::size_t calculateHash(uint8_t* data, std::size_t size) = 0;
};