#include "gtest/gtest.h"
#include "../src/Adder.cpp"

namespace {

    TEST(VecNTest, Add) {
        EXPECT_EQ(1, add(0, 1));
    }

};
