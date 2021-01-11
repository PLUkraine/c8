#include "gtest/gtest.h"

#include "cummon.h"
#include "c8_random.h"

TEST(c8_random, two_instances_sync)
{
    C8_Random_ptr rnd_1 = C8_Random_new(42);
    C8_Random_ptr rnd_2 = C8_Random_new(42);
    EXPECT_NE(rnd_1, nullptr);
    EXPECT_NE(rnd_2, nullptr);


    for (int i=0; i<1000000; ++i)
    {
        ASSERT_EQ(C8_Random_next(rnd_1), C8_Random_next(rnd_2));
    }


    C8_Random_free(&rnd_1);
    C8_Random_free(&rnd_2);
    EXPECT_EQ(rnd_1, nullptr);
    EXPECT_EQ(rnd_2, nullptr);
}