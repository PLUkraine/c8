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

TEST(c8_diplay, test_init_free)
{
    C8_Display_ptr empty = NULL;
    EXPECT_DEBUG_DEATH(C8_Display_free(NULL), "Assertion `disp && \\*disp' failed");
    EXPECT_DEBUG_DEATH(C8_Display_free(&empty),  "Assertion `disp && \\*disp' failed");
}