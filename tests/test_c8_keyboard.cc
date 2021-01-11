#include "gtest/gtest.h"

#include "cummon.h"
#include "c8_keyboard.h"


class c8_keyboard : public ::testing::Test {
protected:
    C8_Keyboard_ptr keys;

    c8_keyboard() {}
    ~c8_keyboard() {}

    virtual void SetUp()
    {
        keys = C8_Keyboard_init();
        EXPECT_NE(keys, nullptr);
    }

    virtual void TearDown()
    {
        C8_Keyboard_free(&keys);
        EXPECT_EQ(keys, nullptr);
    }

};

TEST_F(c8_keyboard, test_init_free)
{
#ifndef NDEBUG
    C8_Keyboard_ptr empty = NULL;
    EXPECT_DEBUG_DEATH(C8_Keyboard_free(NULL), "Assertion `keys && \\*keys' failed");
    EXPECT_DEBUG_DEATH(C8_Keyboard_free(&empty),  "Assertion `keys && \\*keys' failed");
#endif

    for (int i=0; i<C8_KEYBOARD_SIZE; ++i)
    {
        EXPECT_EQ(C8_KEY_UP, C8_Keyboard_get(keys, i));
    }
}

TEST_F(c8_keyboard, test_set_get)
{
    C8_Keyboard_set(keys, 0xF, C8_KEY_DOWN);
    EXPECT_EQ(C8_KEY_DOWN, C8_Keyboard_get(keys, 0xF));

    C8_Keyboard_set(keys, 0xF, C8_KEY_UP);
    EXPECT_EQ(C8_KEY_UP, C8_Keyboard_get(keys, 0xF));


    C8_Keyboard_set(keys, 0xF, C8_KEY_DOWN);
    C8_Keyboard_set(keys, 0xA, C8_KEY_UP);
    EXPECT_EQ(C8_KEY_DOWN, C8_Keyboard_get(keys, 0xF));
    EXPECT_EQ(C8_KEY_UP, C8_Keyboard_get(keys, 0xA));
}

TEST_F(c8_keyboard, test_lock)
{
    uint8_t Vx = 0xFF;
    EXPECT_FALSE(C8_Keyboard_is_locked(keys));

    C8_Keyboard_set(keys, 0xF, C8_KEY_DOWN);
    C8_Keyboard_set(keys, 0xA, C8_KEY_UP);
    EXPECT_FALSE(C8_Keyboard_is_locked(keys));

    C8_Keyboard_lock(keys, &Vx);
    EXPECT_EQ(Vx, 0xFF);
    EXPECT_TRUE(C8_Keyboard_is_locked(keys));

    C8_Keyboard_set(keys, 0xA, C8_KEY_UP);
    EXPECT_EQ(Vx, 0xFF);
    EXPECT_TRUE(C8_Keyboard_is_locked(keys));

    C8_Keyboard_set(keys, 0xA, C8_KEY_DOWN);
    EXPECT_EQ(Vx, 0x0A);
    EXPECT_FALSE(C8_Keyboard_is_locked(keys));
}
