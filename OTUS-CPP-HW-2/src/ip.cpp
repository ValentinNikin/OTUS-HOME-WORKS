#include "ip.h"

#include "utils.h"

IpAddress::IpAddress(
        uint8_t byte1, 
        uint8_t byte2, 
        uint8_t byte3, 
        uint8_t byte4) {
    _innerAddr = std::make_tuple(byte1, byte2, byte3, byte4);
}

IpAddress IpAddress::parse(const std::string& line) {
    auto bytes = utils::split(line, '.');

    if (bytes.size() != 4) {
        throw std::invalid_argument(line + " is not ip address");
    }

    auto convertAndCheckByte = [](const std::string& byteStr) {
        auto byte = atoi(byteStr.c_str());

        if (byte >= 0 && byte <= 255) {
            return static_cast<uint8_t>(byte);
        }

        throw std::invalid_argument("IP byte must be in [0, 255] range");
    };

    uint8_t byte1 = convertAndCheckByte(bytes[0]);
    uint8_t byte2 = convertAndCheckByte(bytes[1]);
    uint8_t byte3 = convertAndCheckByte(bytes[2]);
    uint8_t byte4 = convertAndCheckByte(bytes[3]);

    return IpAddress(byte1, byte2, byte3, byte4);
}