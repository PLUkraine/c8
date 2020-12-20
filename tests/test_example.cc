#include "gtest/gtest.h"
extern "C"
{
   #include "c8.h"
}

TEST(c8_tests, init_free)
{
    auto c8 = C8_init();
    EXPECT_NE(c8, nullptr);
    EXPECT_NE(c8->Ram, nullptr);
    C8_free(&c8);
    EXPECT_EQ(c8, nullptr);
}

TEST(c8_tests, reset)
{
    auto c8 = C8_init();

    C8_reset(c8);
    EXPECT_EQ(c8->Ram[0],       0xF0);
    EXPECT_EQ(c8->Ram[16*5-1],  0x80);
    EXPECT_EQ(c8->Ram[16*5],    0x00);

    C8_free(&c8);
}

TEST(c8_tests, load_program)
{
    auto c8 = C8_init();

    C8_reset(c8);
    char data[] = "pidora lol";
    C8_load_program(c8, (uint8_t *)data, strlen(data));

    C8_free(&c8);
}