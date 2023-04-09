#pragma once

#include <string>
#include <iostream>
#include <tuple>

struct IpAddress {
public:

    IpAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);
    ~IpAddress() = default;

    uint8_t byte1() const { return std::get<0>(_innerAddr); }
    uint8_t byte2() const { return std::get<1>(_innerAddr); }
    uint8_t byte3() const { return std::get<2>(_innerAddr); }
    uint8_t byte4() const { return std::get<3>(_innerAddr); }

    inline friend bool operator< (const IpAddress& addr1, const IpAddress& addr2) {
        return addr1._innerAddr < addr2._innerAddr;
    }

    inline friend bool operator> (const IpAddress& addr1, const IpAddress& addr2) {
        return addr2 < addr1;
    }

    inline friend bool operator<= (const IpAddress& addr1, const IpAddress& addr2) {
        return !(addr2 < addr1);
    }

    inline friend bool operator>= (const IpAddress& addr1, const IpAddress& addr2) {
        return !(addr1 < addr2);
    }

    friend std::ostream& operator<< (std::ostream& stream, const IpAddress& addr) {
        stream << +addr.byte1() << '.' << +addr.byte2() << '.' << +addr.byte3() << '.' << +addr.byte4(); 
        return stream;
    }

    static IpAddress parse(const std::string& line);
private:

    IpAddress() = delete;

    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> _innerAddr;
};