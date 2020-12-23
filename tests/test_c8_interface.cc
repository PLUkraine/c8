#include "gtest/gtest.h"
extern "C"
{
    #include "cummon.h"
    #include "c8.h"
}

TEST(cummon_test, bit_defines)
{
    auto number   = 0xBB2f56;
    auto number_2 = 0xFF0081;
    EXPECT_EQ(0x0f56, BIT_LO_12(number));
    EXPECT_EQ(0x0056, BIT_LO_8 (number));
    EXPECT_EQ(0x0006, BIT_LO_4 (number));
    EXPECT_EQ(   0x2, BIT_HI_4 (number));
    EXPECT_EQ(  0x2f, BIT_HI_8 (number));
    EXPECT_EQ( 0x2f5, BIT_HI_12(number));
    EXPECT_EQ(   0xf, NIBBLE_3 (number));
    EXPECT_EQ(   0x5, NIBBLE_2 (number));
    EXPECT_EQ(   0x0, MSB_BYTE (number));
    EXPECT_EQ(   0x0, LSB_BYTE (number));
    EXPECT_EQ(   0x1, MSB_BYTE (number_2));
    EXPECT_EQ(   0x1, LSB_BYTE (number_2));
}

static const uint32_t TEST_SEED = 42;

class c8_tests : public ::testing::Test {
protected:
    C8_Random_ptr rnd;

    c8_tests()
    {
        rnd = C8_Random_new(TEST_SEED);
    }
    virtual ~c8_tests()
    {
        C8_Random_free(&rnd);
    }
};


TEST_F(c8_tests, init_free)
{
    auto c8 = C8_init(rnd);
    EXPECT_NE(c8, nullptr);
    EXPECT_NE(c8->Ram, nullptr);
    C8_free(&c8);
    EXPECT_EQ(c8, nullptr);
}

TEST_F(c8_tests, reset)
{
    auto c8 = C8_init(rnd);

    C8_reset(c8);
    EXPECT_EQ(c8->PC,           0x200);
    EXPECT_EQ(c8->Ram[0],       0xF0);
    EXPECT_EQ(c8->Ram[16*5-1],  0x80);
    EXPECT_EQ(c8->Ram[16*5],    0x00);

    C8_free(&c8);
}

TEST_F(c8_tests, load_program)
{
    auto c8 = C8_init(rnd);

    C8_reset(c8);
    char data[] = "pidora lol";

    C8_load_program(c8, (uint8_t *)data, strlen(data));
    for (size_t i=0; i<NELEMS(data); ++i)
    {
        EXPECT_EQ(c8->Ram[0x200+i], data[i]);
    }

    C8_free(&c8);
}

TEST_F(c8_tests, assertion_tests)
{
    auto c8 = C8_init(rnd);
    C8_ptr empty = NULL;
    char data[] = "lol";

    // init and free
    EXPECT_DEATH(C8_init(NULL), "Assertion `rnd' failed");
    EXPECT_DEATH(C8_free(NULL), "Assertion `c8 && \\*c8' failed");
    EXPECT_DEATH(C8_free(&empty),  "Assertion `c8 && \\*c8' failed");

    // reset
    EXPECT_DEATH(C8_reset(NULL), "Assertion `c8' failed");

    // load
    EXPECT_DEATH(C8_load_program(NULL, (uint8_t *)data, strlen(data)), "Assertion `c8' failed.");
    EXPECT_DEATH(C8_load_program(c8, NULL, strlen(data)), "Assertion `data' failed");
    EXPECT_DEATH(C8_load_program(c8, (uint8_t *)data, 0xFFF - 0x200 + 2), "Assertion .* failed");

    // cycle
    EXPECT_DEATH(C8_cycle(NULL), "Assertion `c8' failed");

    C8_free(&c8);
}
