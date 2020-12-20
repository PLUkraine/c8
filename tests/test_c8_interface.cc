#include "gtest/gtest.h"
extern "C"
{
    #include "cummon.h"
    #include "c8.h"
}

TEST(cummon_test, bit_defines)
{
    auto number = 0xBB2f56;
    EXPECT_EQ(0x0f56, BIT_LO_24(number));
    EXPECT_EQ(0x0056, BIT_LO_16(number));
    EXPECT_EQ(0x0006, BIT_LO_8 (number));
    EXPECT_EQ(0x2000, BIT_HI_8 (number));
    EXPECT_EQ(0x2f00, BIT_HI_16(number));
    EXPECT_EQ(0x2f50, BIT_HI_24(number));
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
    EXPECT_EQ(c8->PC,           0x200);
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