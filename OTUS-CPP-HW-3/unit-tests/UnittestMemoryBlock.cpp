
#include "gtest/gtest.h"

#include "MemoryBlock.h"

TEST(MemoryBlock, constructor) {
    MemoryBlock<int> memoryBlock {10};
    ASSERT_EQ(memoryBlock.getSize(), 10);
}

TEST(MemoryBlock, allocateFree) {
    MemoryBlock<int> memoryBlock1 {10};

    ASSERT_EQ(memoryBlock1.allocateRange(0), nullptr);
    ASSERT_EQ(memoryBlock1.allocateRange(11), nullptr);

    auto mb1_ptr1 = memoryBlock1.allocateRange(5);
    ASSERT_NE(mb1_ptr1, nullptr);

    auto mb1_ptr2 = memoryBlock1.allocateRange(5);
    ASSERT_NE(mb1_ptr2, nullptr);

    ASSERT_EQ(memoryBlock1.allocateRange(1), nullptr);

    MemoryBlock<int> memoryBlock2 {5};
    auto mb2_ptr = memoryBlock2.allocateRange(5);

    ASSERT_FALSE(memoryBlock1.freeRange(mb2_ptr, 5));
    ASSERT_TRUE(memoryBlock2.freeRange(mb2_ptr, 5));

    ASSERT_TRUE(memoryBlock1.freeRange(mb1_ptr1, 5));
    ASSERT_TRUE(memoryBlock1.freeRange(mb1_ptr2, 5));
}