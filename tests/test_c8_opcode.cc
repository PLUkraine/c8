#include "gtest/gtest.h"
extern "C"
{
    #include "cummon.h"
    #include "c8.h"
}

C8_ptr init_c8(uint8_t *opcode, size_t N)
{
    auto c8 = C8_init();
    C8_reset(c8);
    C8_load_program(c8, opcode, N);
    return c8;
}

TEST(c8_opcode, SYS)
{
    uint8_t opcode[] = { 0x0A, 0xBC, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0xABC);

    C8_free(&c8);
}

TEST(c8_opcode, JP)
{
    uint8_t opcode[] = { 0x1A, 0xBC, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0xABC);

    C8_free(&c8);
}

TEST(c8_opcode, RET_normal)
{
    uint8_t opcode[] = { 0x00, 0xEE,};
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Stack[1] = 0x0ABC;
    c8->SP = 1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x0ABC);
    EXPECT_EQ(c8->SP, 0);

    C8_free(&c8);
}

TEST(c8_opcode, RET_overflow)
{
    uint8_t opcode[] = { 0x00, 0xEE, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Stack[0] = 0x0ABC;
    c8->SP = 0;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x0ABC);
    EXPECT_EQ(c8->SP, NELEMS(c8->Stack)-1);

    C8_free(&c8);
}

TEST(c8_opcode, CALL_normal)
{
    uint8_t opcode[] = { 0x2A, 0xBC, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Stack[0] = 0x0000;
    c8->SP = 0;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0xABC);
    EXPECT_EQ(c8->SP, 1);
    EXPECT_EQ(c8->Stack[0], 0x202);

    C8_free(&c8);
}

TEST(c8_opcode, CALL_overflow)
{
    uint8_t opcode[] = { 0x2A, 0xBC, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Stack[15] = 0x0000;
    c8->SP = 15;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0xABC);
    EXPECT_EQ(c8->SP, 0);
    EXPECT_EQ(c8->Stack[15], 0x202);

    C8_free(&c8);
}

TEST(c8_opcode, SE_skip)
{
    uint8_t opcode[] = { 0x35, 0xBC, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[5] = 0xBC;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x204);

    C8_free(&c8);
}

TEST(c8_opcode, SE_no_skip)
{
    uint8_t opcode[] = { 0x35, 0xAA, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[5] = 0xBC;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x202);

    C8_free(&c8);
}

TEST(c8_opcode, SNE_no_skip)
{
    uint8_t opcode[] = { 0x45, 0xBC, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[5] = 0xBC;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x202);

    C8_free(&c8);
}

TEST(c8_opcode, SNE_skip)
{
    uint8_t opcode[] = { 0x45, 0xAA, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[5] = 0xBC;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x204);

    C8_free(&c8);
}

TEST(c8_opcode, SEV_skip)
{
    // reorder C6 to 6C
    uint8_t opcode[] = { 0x5C, 0x60, 0x00, 0x00, 0x56, 0xC0, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[0x6] = 0xBC;
    c8->Vx[0xC] = 0xBC;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x204);

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x208);

    C8_free(&c8);
}

TEST(c8_opcode, SEV_no_skip)
{
    // reorder C6 to 6C
    uint8_t opcode[] = { 0x5C, 0x60, 0x56, 0xC0, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[0x6] = 0xBC;
    c8->Vx[0xC] = 0xAA;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x202);

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x204);

    C8_free(&c8);
}

TEST(c8_opcode, LD)
{
    uint8_t opcode[] = { 0x6C, 0xBC, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x00;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x202);
    EXPECT_EQ(c8->Vx[0xC], 0xBC);

    C8_free(&c8);
}

TEST(c8_opcode, ADD_Vx)
{
    uint8_t opcode[] = { 0x7C, 0xBC, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x72;
    c8->Vx[0xF] = 0x00;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x202);
    EXPECT_EQ(c8->Vx[0xC], (0xBC + 0x72) % 0x100);
    EXPECT_EQ(c8->Vx[0xF], 0x00);

    C8_free(&c8);
}

TEST(c8_opcode, LD_V)
{
    uint8_t opcode[] = { 0x8C, 0x40, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x72;
    c8->Vx[0x4] = 0xF1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x202);
    EXPECT_EQ(c8->Vx[0xC], c8->Vx[0x4]);

    C8_free(&c8);
}

TEST(c8_opcode, OR_V)
{
    uint8_t opcode[] = { 0x8C, 0x41, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x72;
    c8->Vx[0x4] = 0xF1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x202);
    EXPECT_EQ(c8->Vx[0xC], 0x72 | 0xF1);

    C8_free(&c8);
}

TEST(c8_opcode, AND_V)
{
    uint8_t opcode[] = { 0x8C, 0x42, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x72;
    c8->Vx[0x4] = 0xF1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x202);
    EXPECT_EQ(c8->Vx[0xC], 0x72 & 0xF1);

    C8_free(&c8);
}

TEST(c8_opcode, XOR_V)
{
    uint8_t opcode[] = { 0x8C, 0x43, };
    auto c8 = init_c8(opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x72;
    c8->Vx[0x4] = 0xF1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x202);
    EXPECT_EQ(c8->Vx[0xC], 0x72 ^ 0xF1);

    C8_free(&c8);
}
