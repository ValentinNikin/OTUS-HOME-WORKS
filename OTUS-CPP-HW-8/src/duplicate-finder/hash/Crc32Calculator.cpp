#include "Crc32Calculator.h"

#include <boost/crc.hpp>

std::size_t Crc32Calculator::calculateHash(uint8_t* data, std::size_t size) {
    boost::crc_32_type result;
    result.process_bytes(data, size);
    return result.checksum();
}