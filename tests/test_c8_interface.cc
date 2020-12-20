#include "gtest/gtest.h"
extern "C"
{
    #include "cummon.h"
    #include "c8.h"
}

TEST(cummon_test, bit_defines)
{
    auto number = 0xBB2f56;
    EXPECT_EQ(0x0f56, BIT_LO_12(number));
    EXPECT_EQ(0x0056, BIT_LO_8 (number));
    EXPECT_EQ(0x0006, BIT_LO_4 (number));
    EXPECT_EQ(   0x2, BIT_HI_4 (number));
    EXPECT_EQ(  0x2f, BIT_HI_8 (number));
    EXPECT_EQ( 0x2f5, BIT_HI_12(number));
    EXPECT_EQ(   0xf, NIMB_3   (number));
    EXPECT_EQ(   0x5, NIMB_2   (number));
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
    for (size_t i=0; i<NELEMS(data); ++i)
    {
        EXPECT_EQ(c8->Ram[0x200+i], data[i]);
    }

    C8_free(&c8);
}