#include "DefaultHashCalculator.h"

#include <boost/functional/hash.hpp>

std::size_t DefaultHashCalculator::calculateHash(uint8_t* data, std::size_t size) {
    return boost::hash_range(data, data + size);
}