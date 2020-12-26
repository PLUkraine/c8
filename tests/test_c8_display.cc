#include "gtest/gtest.h"
extern "C"
{
    #include "cummon.h"
    #include "c8_display.h"
}

class c8_display : public ::testing::Test {
protected:
    C8_Display_ptr disp;

    c8_display()
    {
        disp = C8_Display_init();
        EXPECT_NE(disp, nullptr);

    }
    virtual ~c8_display()
    {
        C8_Display_free(&disp);
        EXPECT_EQ(disp, nullptr);
    }
};

TEST_F(c8_display, test_init_free)
{
    C8_Display_ptr empty = NULL;
    EXPECT_DEBUG_DEATH(C8_Display_free(NULL), "Assertion `disp && \\*disp' failed");
    EXPECT_DEBUG_DEATH(C8_Display_free(&empty),  "Assertion `disp && \\*disp' failed");
}

TEST_F(c8_display, coord_1d)
{
    EXPECT_EQ(coord_1d(0, 5, 3), 5);
    EXPECT_EQ(coord_1d(3, 2, 4), 14);
}

TEST_F(c8_display, clear)
{
    C8_Display_clear(disp);
    for (int i=0; i<C8_DISPLAY_HEIGHT; ++i)
    {
        for (int j=0; j<C8_DISPLAY_WIDTH; ++j)
        {
            EXPECT_EQ(C8_Display_pixel(disp, i, j), C8_DISPLAY_OFF);
        }
    }
}

TEST_F(c8_display, get_pixel)
{
    EXPECT_DEBUG_DEATH(C8_Display_pixel(disp, -1, 0), "Assertion `.*' failed");
    EXPECT_DEBUG_DEATH(C8_Display_pixel(disp, 0, -1), "Assertion `.*' failed");

    EXPECT_DEBUG_DEATH(C8_Display_pixel(disp, 32, 0), "Assertion `.*' failed");
    EXPECT_DEBUG_DEATH(C8_Display_pixel(disp, 0, 64), "Assertion `.*' failed");
}

TEST_F(c8_display, toggle)
{
    C8_Display_clear(disp);

    EXPECT_EQ(C8_Display_pixel_toggle(disp, 8, 19), C8_DISPLAY_ON);
    EXPECT_EQ(C8_Display_pixel(disp, 8, 19), C8_DISPLAY_ON);

    EXPECT_EQ(C8_Display_pixel_toggle(disp, 31, 63), C8_DISPLAY_ON);
    EXPECT_EQ(C8_Display_pixel(disp, 31, 63), C8_DISPLAY_ON);

    EXPECT_EQ(C8_Display_pixel_toggle(disp, 0, 0), C8_DISPLAY_ON);
    EXPECT_EQ(C8_Display_pixel(disp, 0, 0), C8_DISPLAY_ON);

    EXPECT_EQ(C8_Display_pixel_toggle(disp, 31, 63), C8_DISPLAY_OFF);
    EXPECT_EQ(C8_Display_pixel(disp, 31, 63), C8_DISPLAY_OFF);

    EXPECT_EQ(C8_Display_pixel_toggle(disp, 8, 19), C8_DISPLAY_OFF);
    EXPECT_EQ(C8_Display_pixel(disp, 8, 19), C8_DISPLAY_OFF);

    EXPECT_EQ(C8_Display_pixel_toggle(disp, 8, 19), C8_DISPLAY_ON);
    EXPECT_EQ(C8_Display_pixel(disp, 8, 19), C8_DISPLAY_ON);

    EXPECT_EQ(C8_Display_pixel_toggle(disp, 0, 0), C8_DISPLAY_OFF);
    EXPECT_EQ(C8_Display_pixel(disp, 0, 0), C8_DISPLAY_OFF);


    EXPECT_DEBUG_DEATH(C8_Display_pixel_toggle(disp, -1, 0), "Assertion `.*' failed");
    EXPECT_DEBUG_DEATH(C8_Display_pixel_toggle(disp, 0, -1), "Assertion `.*' failed");

    EXPECT_DEBUG_DEATH(C8_Display_pixel_toggle(disp, 32, 0), "Assertion `.*' failed");
    EXPECT_DEBUG_DEATH(C8_Display_pixel_toggle(disp, 0, 64), "Assertion `.*' failed");
}

