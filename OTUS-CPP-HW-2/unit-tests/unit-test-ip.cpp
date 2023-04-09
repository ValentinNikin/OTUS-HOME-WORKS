
#include <gtest/gtest.h>

#include "ip.h"

TEST(IpAddress, Constructor) {
    IpAddress addr(192, 168, 126, 21);

    ASSERT_EQ(addr.byte1(), 192);
    ASSERT_EQ(addr.byte2(), 168);
    ASSERT_EQ(addr.byte3(), 126);
    ASSERT_EQ(addr.byte4(), 21);
}

TEST(IpAddress, Comparing) {
    ASSERT_TRUE(IpAddress(1, 2, 0, 10) < IpAddress(1, 3, 0, 0));
    ASSERT_TRUE(IpAddress(0, 0, 0, 0) < IpAddress(255, 255, 255, 255));

    ASSERT_TRUE(IpAddress(1, 4, 0, 10) > IpAddress(1, 3, 0, 0));
    ASSERT_TRUE(IpAddress(1, 4, 0, 10) <= IpAddress(1, 4, 0, 10));
    ASSERT_TRUE(IpAddress(1, 4, 0, 10) >= IpAddress(1, 4, 0, 10));
}

TEST(IpAddress, ParserExceptions) {
    ASSERT_THROW(IpAddress::parse("192.168.126"), std::invalid_argument);
    ASSERT_THROW(IpAddress::parse("192.168"), std::invalid_argument);
    ASSERT_THROW(IpAddress::parse("192."), std::invalid_argument);

    ASSERT_THROW(IpAddress::parse("256.0.0.0"), std::invalid_argument);
    ASSERT_THROW(IpAddress::parse("0.0.256.0"), std::invalid_argument);
    ASSERT_THROW(IpAddress::parse("0.-2.0.0"), std::invalid_argument);
}

TEST(IpAddress, ParserSuccess) {
    auto ipAddr = IpAddress::parse("19.60.10.74");
    ASSERT_EQ(ipAddr.byte1(), 19);
    ASSERT_EQ(ipAddr.byte2(), 60);
    ASSERT_EQ(ipAddr.byte3(), 10);
    ASSERT_EQ(ipAddr.byte4(), 74);
}