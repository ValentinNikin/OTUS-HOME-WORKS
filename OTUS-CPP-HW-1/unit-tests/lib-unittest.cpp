#include <gtest/gtest.h>

#include "lib.h"

TEST(LIB, simple_test) {
    ASSERT_GE(lib::version(), 0);
}