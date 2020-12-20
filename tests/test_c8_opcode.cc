#include "gtest/gtest.h"
extern "C"
{
    #include "cummon.h"
    #include "c8.h"
}

TEST(c8_opcode, SYS)
{
    auto c8 = C8_init();

    C8_reset(c8);
    uint8_t opcode[] = { 0x0A, 0xBC, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0xABC);

    C8_free(&c8);
}

TEST(c8_opcode, RET_normal)
{
    auto c8 = C8_init();

    C8_reset(c8);
    uint8_t opcode[] = { 0x00, 0xEE,};
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Stack[1] = 0x0ABC;
    c8->SP = 1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x0ABC);
    EXPECT_EQ(c8->SP, 0);

    C8_free(&c8);
}

TEST(c8_opcode, RET_overflow)
{
    auto c8 = C8_init();

    C8_reset(c8);
    uint8_t opcode[] = { 0x00, 0xEE, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Stack[0] = 0x0ABC;
    c8->SP = 0;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x0ABC);
    EXPECT_EQ(c8->SP, NELEMS(c8->Stack)-1);

    C8_free(&c8);
}
