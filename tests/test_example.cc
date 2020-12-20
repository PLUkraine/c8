#include "gtest/gtest.h"
extern "C"
{
   #include "c8.h"
}

TEST(c8_tests, init_free)
{
    auto c8 = C8_init();
    EXPECT_NE(c8, nullptr);
    C8_free(&c8);
    EXPECT_EQ(c8, nullptr);
}