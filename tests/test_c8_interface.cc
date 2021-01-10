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
    C8_Display_ptr disp;
    C8_ptr c8;

    c8_tests() {}
    virtual ~c8_tests() {}

    virtual void SetUp()
    {  
        rnd = C8_Random_new(TEST_SEED);
        disp = C8_Display_init();

        c8 = C8_init(rnd, disp);
        EXPECT_NE(c8, nullptr);
        EXPECT_NE(c8->Ram, nullptr);
    }
    virtual void TearDown()
    {
        C8_free(&c8);
        C8_Random_free(&rnd);
        C8_Display_free(&disp);
        EXPECT_EQ(c8, nullptr);
    }
};


TEST_F(c8_tests, init_free)
{
#ifndef NDEBUG
    C8_ptr empty = NULL;

    EXPECT_DEBUG_DEATH(C8_init(NULL, disp), "Assertion `rnd' failed");
    EXPECT_DEBUG_DEATH(C8_init(rnd, NULL), "Assertion `disp' failed");
    EXPECT_DEBUG_DEATH(C8_free(NULL), "Assertion `c8 && \\*c8' failed");
    EXPECT_DEBUG_DEATH(C8_free(&empty),  "Assertion `c8 && \\*c8' failed");
#endif
}

TEST_F(c8_tests, reset)
{
    
#ifndef NDEBUG
    EXPECT_DEBUG_DEATH(C8_reset(NULL), "Assertion `c8' failed");
#endif

    c8->Ram[0x500] = 0xFA;
    c8->Ram[0x501] = 0xA0;
    C8_reset(c8);
    EXPECT_EQ(C8_Display_is_clear(c8->Display), true);
    EXPECT_EQ(c8->PC,                 0x200);
    EXPECT_EQ(c8->Ram[0],             0xF0);
    EXPECT_EQ(c8->Ram[16*5-1],        0x80);
    EXPECT_EQ(c8->Ram[16*5],          0x00);
    EXPECT_EQ(c8->Ram[0x500],         0xFA);
    EXPECT_EQ(c8->Ram[0x501],         0xA0);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x00);
    for (size_t i=0; i<NELEMS(c8->Key); ++i)
    {
        EXPECT_EQ(c8->Key[i], 0x00);
    }
}

TEST_F(c8_tests, timer)
{
    
#ifndef NDEBUG
    EXPECT_DEBUG_DEATH(C8_timers(NULL), "Assertion `c8' failed");
#endif

    C8_reset(c8);
    c8->DT = 1;
    c8->ST = 3;

    C8_timers(c8);
    EXPECT_EQ(c8->DT, 0);
    EXPECT_EQ(c8->ST, 2);
    
    C8_timers(c8);
    EXPECT_EQ(c8->DT, 0);
    EXPECT_EQ(c8->ST, 1);

    C8_timers(c8);
    EXPECT_EQ(c8->DT, 0);
    EXPECT_EQ(c8->ST, 0);
    
    C8_timers(c8);
    EXPECT_EQ(c8->DT, 0);
    EXPECT_EQ(c8->ST, 0);
}

TEST_F(c8_tests, load_program)
{
    C8_reset(c8);
    char data[] = "pidora lol";

#ifndef NDEBUG
    EXPECT_DEBUG_DEATH(C8_load_program(NULL, (uint8_t *)data, strlen(data)), "Assertion `c8' failed.");
    EXPECT_DEBUG_DEATH(C8_load_program(c8, NULL, strlen(data)), "Assertion `data' failed");
    EXPECT_DEBUG_DEATH(C8_load_program(c8, (uint8_t *)data, 0xFFF - 0x200 + 2), "Assertion .* failed");
#endif

    C8_load_program(c8, (uint8_t *)data, strlen(data));
    for (size_t i=0; i<NELEMS(data); ++i)
    {
        EXPECT_EQ(c8->Ram[0x200+i], data[i]);
    }
}

TEST_F(c8_tests, set_key)
{
    C8_reset(c8);

#ifndef NDEBUG
    EXPECT_DEBUG_DEATH(C8_set_key(NULL, 0, false), "Assertion `c8' failed");
    EXPECT_DEBUG_DEATH(C8_set_key(c8, 16, false), "Assertion `key < NELEMS\\(c8->Key\\)' failed");
#endif

    C8_set_key(c8, 3, true);
    EXPECT_EQ(c8->Key[3], 1);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x00);
    C8_set_key(c8, 0xF, true);
    EXPECT_EQ(c8->Key[0xF], 1);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x00);
    C8_set_key(c8, 3, false);
    EXPECT_EQ(c8->Key[3], 0);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x00);
}

TEST_F(c8_tests, set_key_wait)
{
    C8_reset(c8);
    
    c8->Vx[0xA] = 0x00;
    c8->WriteKeyToRegistry = 0xA + 1;
    // no reaction
    C8_set_key(c8, 3, false);
    EXPECT_EQ(c8->Vx[0xA], 0);
    EXPECT_EQ(c8->Key[3], false);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0xA + 1);

    // set Vx[0xA] = 0x3
    C8_set_key(c8, 0x3, true);
    EXPECT_EQ(c8->Vx[0xA], 0x3);
    EXPECT_EQ(c8->Key[3], true);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x0);

    // no change
    C8_set_key(c8, 0x4, true);
    EXPECT_EQ(c8->Vx[0xA], 0x3);
    EXPECT_EQ(c8->Key[4], true);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x0);
    C8_set_key(c8, 0x3, false);
    EXPECT_EQ(c8->Vx[0xA], 0x3);
    EXPECT_EQ(c8->Key[3], false);
    EXPECT_EQ(c8->WriteKeyToRegistry, 0x0);
}

TEST_F(c8_tests, cycle)
{
    C8_reset(c8);

#ifndef NDEBUG
    EXPECT_DEBUG_DEATH(C8_cycle(NULL), "Assertion `c8' failed");
    c8->WriteKeyToRegistry = NELEMS(c8->Vx) + 1;
    EXPECT_DEBUG_DEATH(C8_cycle(c8), "Assertion .* failed");
#endif

    // won't execute until key was pressed
    c8->WriteKeyToRegistry = NELEMS(c8->Vx);
    for (int i=0; i<100; ++i) {
        C8_cycle(c8);
        ASSERT_EQ(c8->PC, 0x200);
    }

    //               JP 0x202
    uint8_t data[] = {0x12, 0x02,};
    C8_load_program(c8, data, NELEMS(data));

    // releasing the key shoudn't remove the lock
    C8_set_key(c8, NELEMS(c8->Vx)-1, false);
    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x200);

    // key press unlocks execution
    C8_set_key(c8, NELEMS(c8->Vx)-1, true);
    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x202);
}
