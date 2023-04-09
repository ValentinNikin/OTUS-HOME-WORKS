
#include <gtest/gtest.h>

#include "utils.h"

TEST(utils, split) {
    auto res = utils::split("192.168.20.84", '.');
    ASSERT_EQ(res.size(), 4);
    ASSERT_EQ(res[0], "192");
    ASSERT_EQ(res[1], "168");
    ASSERT_EQ(res[2], "20");
    ASSERT_EQ(res[3], "84");
}