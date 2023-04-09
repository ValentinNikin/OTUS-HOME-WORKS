
#include "gtest/gtest.h"

#include "DynamicBitset.h"

TEST(DynamicBitset, constructor) {
    DynamicBitset bitset {5};

    ASSERT_FALSE(bitset.test(0));
    ASSERT_FALSE(bitset.test(1));
    ASSERT_FALSE(bitset.test(2));
    ASSERT_FALSE(bitset.test(3));
    ASSERT_FALSE(bitset.test(4));
}

TEST(DynamicBitset, set) {
    DynamicBitset bitset {5};

    bitset.set(0);
    bitset.set(1, 3);
    bitset.set(4);

    ASSERT_TRUE(bitset.test(0));
    ASSERT_TRUE(bitset.test(1));
    ASSERT_TRUE(bitset.test(2));
    ASSERT_TRUE(bitset.test(3));
    ASSERT_TRUE(bitset.test(4));
}

TEST(DynamicBitset, reset) {
    DynamicBitset bitset {5};

    bitset.set(0, 5);

    bitset.reset(0);
    bitset.reset(1, 3);
    bitset.reset(4);

    ASSERT_FALSE(bitset.test(0));
    ASSERT_FALSE(bitset.test(1));
    ASSERT_FALSE(bitset.test(2));
    ASSERT_FALSE(bitset.test(3));
    ASSERT_FALSE(bitset.test(4));
}

TEST(DynamicBitset, findUnsetContinuousRange) {
    DynamicBitset bitset {5};

    bitset.set(0);
    bitset.set(4);

    ASSERT_EQ(bitset.findUnsetContinuousRange(3), 1);
    ASSERT_EQ(bitset.findUnsetContinuousRange(4), SIZE_MAX);
    ASSERT_EQ(bitset.findUnsetContinuousRange(0), SIZE_MAX);
}