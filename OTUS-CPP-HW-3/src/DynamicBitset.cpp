#include "DynamicBitset.h"

#include <stdexcept>

DynamicBitset::DynamicBitset(std::size_t size) {
    _size = size;
    auto countBitsets = _size / BITS_IN_BYTE + 1;
    _bitsets = new std::bitset<BITS_IN_BYTE>[countBitsets];
}

DynamicBitset::~DynamicBitset() {
    delete[] _bitsets;
}

std::size_t DynamicBitset::findUnsetContinuousRange(std::size_t size) {
    if (size == 0) return SIZE_MAX;

    std::size_t accumulated = 0;
    for (std::size_t pos = 0; pos < _size; pos++) {
        if (!test(pos)) {
            accumulated++;
        }
        else {
            accumulated = 0;
        }

        if (accumulated == size) {
            return pos - size + 1;
        }
    }

    return SIZE_MAX;
}

bool DynamicBitset::test(std::size_t pos) {
    if (pos >= _size) throw std::out_of_range("Out of range position: " + std::to_string(pos));

    return _bitsets[pos / BITS_IN_BYTE].test(pos % BITS_IN_BYTE);
}

void DynamicBitset::set(std::size_t pos) {
    if (pos >= _size) throw std::out_of_range("Out of range position: " + std::to_string(pos));

    _bitsets[pos / BITS_IN_BYTE].set(pos % BITS_IN_BYTE);
}

void DynamicBitset::set(std::size_t pos, std::size_t size) {
    if (pos + size > _size) throw std::out_of_range("Out of range position: " + std::to_string(pos + size));

    for (std::size_t i = pos; i < pos + size; i++) {
        set(i);
    }
}

void DynamicBitset::reset(std::size_t pos) {
    if (pos >= _size) throw std::out_of_range("Out of range position: " + std::to_string(pos));

    _bitsets[pos / BITS_IN_BYTE].reset(pos % BITS_IN_BYTE);
}

void DynamicBitset::reset(std::size_t pos, std::size_t size) {
    if (pos + size > _size) throw std::out_of_range("Out of range position: " + std::to_string(pos + size));

    for (std::size_t i = pos; i < pos + size; i++) {
        reset(i);
    }
}