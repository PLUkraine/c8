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

    EXPECT_DEBUG_DEATH(C8_init(NULL), "Assertion `rnd' failed");
    EXPECT_DEBUG_DEATH(C8_free(NULL), "Assertion `c8 && \\*c8' failed");
    EXPECT_DEBUG_DEATH(C8_free(&c8),  "Assertion `c8 && \\*c8' failed");
}

TEST_F(c8_tests, reset)
{
    auto c8 = C8_init(rnd);

    EXPECT_DEBUG_DEATH(C8_reset(NULL), "Assertion `c8' failed");

    C8_reset(c8);
    EXPECT_EQ(c8->PC,                 0x200);
    EXPECT_EQ(c8->Ram[0],             0xF0);
    EXPECT_EQ(c8->Ram[16*5-1],        0x80);
    EXPECT_EQ(c8->Ram[16*5],          0x00);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x00);
    for (size_t i=0; i<NELEMS(c8->Key); ++i)
    {
        EXPECT_EQ(c8->Key[i], 0x00);
    }

    C8_free(&c8);
}

TEST_F(c8_tests, load_program)
{
    auto c8 = C8_init(rnd);

    C8_reset(c8);
    char data[] = "pidora lol";

    EXPECT_DEBUG_DEATH(C8_load_program(NULL, (uint8_t *)data, strlen(data)), "Assertion `c8' failed.");
    EXPECT_DEBUG_DEATH(C8_load_program(c8, NULL, strlen(data)), "Assertion `data' failed");
    EXPECT_DEBUG_DEATH(C8_load_program(c8, (uint8_t *)data, 0xFFF - 0x200 + 2), "Assertion .* failed");

    C8_load_program(c8, (uint8_t *)data, strlen(data));
    for (size_t i=0; i<NELEMS(data); ++i)
    {
        EXPECT_EQ(c8->Ram[0x200+i], data[i]);
    }

    C8_free(&c8);
}

TEST_F(c8_tests, set_key)
{
    auto c8 = C8_init(rnd);

    C8_reset(c8);

    EXPECT_DEBUG_DEATH(C8_set_key(NULL, 0, 0), "Assertion `c8' failed");
    EXPECT_DEBUG_DEATH(C8_set_key(c8, 16, 0), "Assertion `key < NELEMS\\(c8->Key\\)' failed");
    EXPECT_DEBUG_DEATH(C8_set_key(c8, 0, 2), "Assertion `state == 0 || state == 1' failed");

    C8_set_key(c8, 3, 1);
    EXPECT_EQ(c8->Key[3], 1);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x00);
    C8_set_key(c8, 0xF, 1);
    EXPECT_EQ(c8->Key[0xF], 1);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x00);
    C8_set_key(c8, 3, 0);
    EXPECT_EQ(c8->Key[3], 0);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x00);

    C8_free(&c8);
}

TEST_F(c8_tests, set_key_wait)
{
    auto c8 = C8_init(rnd);
    C8_reset(c8);
    
    c8->Vx[0xA] = 0x00;
    c8->WriteKeyToRegistry = 0xA + 1;
    // no reaction
    C8_set_key(c8, 3, 0);
    EXPECT_EQ(c8->Vx[0xA], 0);
    EXPECT_EQ(c8->Key[3], 0);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0xA + 1);

    // set Vx[0xA] = 0x3
    C8_set_key(c8, 0x3, 1);
    EXPECT_EQ(c8->Vx[0xA], 0x3);
    EXPECT_EQ(c8->Key[3], 0x1);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x0);

    // no change
    C8_set_key(c8, 0x4, 1);
    EXPECT_EQ(c8->Vx[0xA], 0x3);
    EXPECT_EQ(c8->Key[4], 0x1);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x0);
    C8_set_key(c8, 0x3, 0);
    EXPECT_EQ(c8->Vx[0xA], 0x3);
    EXPECT_EQ(c8->Key[3], 0x0);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x0);

    C8_free(&c8);
}

TEST_F(c8_tests, cycle)
{
    auto c8 = C8_init(rnd);

    C8_reset(c8);

    EXPECT_DEBUG_DEATH(C8_cycle(NULL), "Assertion `c8' failed");
    c8->WriteKeyToRegistry = NELEMS(c8->Vx) + 1;
    EXPECT_DEBUG_DEATH(C8_cycle(c8), "Assertion .* failed");

    c8->WriteKeyToRegistry = NELEMS(c8->Vx);
    for (int i=0; i<100; ++i) {
        C8_cycle(c8);
        ASSERT_EQ(c8->PC, 0x200);
    }

    C8_free(&c8);
}
