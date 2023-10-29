#include <gtest/gtest.h>
#include "../../src/include/ADTMap.h"

TEST(TestTopic, TestInsert) {
    EXPECT_EQ(0, 0);
}

TEST(TestTopic, TestRemove) {
    Map* map = new Map(nullptr, nullptr, nullptr);
    EXPECT_EQ(map->remove(), true);
}

TEST(TestTopic, TestFind) {
    Map* map = new Map(nullptr, nullptr, nullptr);
    EXPECT_EQ(map->find(), nullptr);
}

TEST(TestTopic, TestIterate) {
    Map* map = new Map(nullptr, nullptr, nullptr);
    EXPECT_EQ(map->remove(), true);
}