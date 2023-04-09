
#include "gtest/gtest.h"

#include "CustomList.h"

TEST(CustomList, constructor) {
    CustomList<int> list;
    ASSERT_EQ(list.size(), 0);
}

TEST(CustomList, pushFront) {
    CustomList<int> list;
    ASSERT_EQ(list.size(), 0);

    list.pushFront(1);
    list.pushFront(2);
    list.pushFront(3);

    ASSERT_EQ(list.size(), 3);

    auto currentIt = list.begin();
    ASSERT_EQ(*currentIt++, 3);
    ASSERT_EQ(*currentIt++, 2);
    ASSERT_EQ(*currentIt++, 1);
    ASSERT_EQ(currentIt, list.end());
}

TEST(CustomList, pushBack) {
    CustomList<int> list;
    ASSERT_EQ(list.size(), 0);

    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);

    ASSERT_EQ(list.size(), 3);

    auto currentIt = list.begin();
    ASSERT_EQ(*currentIt++, 1);
    ASSERT_EQ(*currentIt++, 2);
    ASSERT_EQ(*currentIt++, 3);
    ASSERT_EQ(currentIt, list.end());
}

TEST(CustomList, clear) {
    CustomList<int> list;
    ASSERT_EQ(list.size(), 0);

    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);

    ASSERT_EQ(list.size(), 3);

    list.clear();

    ASSERT_EQ(list.size(), 0);
}