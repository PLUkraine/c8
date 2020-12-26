#include "gtest/gtest.h"
extern "C"
{
    #include "cummon.h"
    #include "c8.h"
    #include "c8_private.h"
}

// determinted by a objectively random dice roll on my table
const uint32_t TEST_SEED = 4;


class c8_opcode : public ::testing::Test {
protected:
    C8_Random_ptr _rnd;
    C8_ptr          c8;

    c8_opcode() {}
    virtual ~c8_opcode() {}


    void compare_ram_to_regs(uint8_t reg)
    {
        for (size_t i=0; i<NELEMS(c8->Vx); ++i)    
        {
            int ram = c8->Ram[c8->I + i];
            int act = c8->Vx[i];
            if (i <= reg) {
                EXPECT_EQ(ram, act);
            } else {
                EXPECT_NE(ram, act);
            }
        }
    }


    virtual void SetUp()
    {
        _rnd = C8_Random_new(TEST_SEED);
        c8  = C8_init(_rnd);
        C8_reset(c8);
    }

    virtual void TearDown()
    {
        C8_free(&c8);
        C8_Random_free(&_rnd);
    }
};

TEST_F(c8_opcode, DISABLED_CLS)
{
    uint8_t opcode[] = { 0x00, 0xE0, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    C8_cycle(c8);
    FAIL();
}

TEST_F(c8_opcode, SYS)
{
    uint8_t opcode[] = { 0x0A, 0xBC, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0xABC);
}

TEST_F(c8_opcode, JP)
{
    uint8_t opcode[] = { 0x1A, 0xBC, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0xABC);
}

TEST_F(c8_opcode, RET_normal)
{
    uint8_t opcode[] = { 0x00, 0xEE,};
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Stack[1] = 0x0ABC;
    c8->SP = 1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x0ABC);
    EXPECT_EQ(c8->SP, 0);
}

TEST_F(c8_opcode, RET_overflow)
{
    uint8_t opcode[] = { 0x00, 0xEE, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Stack[0] = 0x0ABC;
    c8->SP = 0;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0x0ABC);
    EXPECT_EQ(c8->SP, NELEMS(c8->Stack)-1);
}

TEST_F(c8_opcode, CALL_normal)
{
    uint8_t opcode[] = { 0x2A, 0xBC, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Stack[0] = 0x0000;
    c8->SP = 0;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0xABC);
    EXPECT_EQ(c8->SP, 1);
    EXPECT_EQ(c8->Stack[0], C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, CALL_overflow)
{
    uint8_t opcode[] = { 0x2A, 0xBC, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Stack[15] = 0x0000;
    c8->SP = 15;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0xABC);
    EXPECT_EQ(c8->SP, 0);
    EXPECT_EQ(c8->Stack[15], C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, SE_skip)
{
    uint8_t opcode[] = { 0x35, 0xBC, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[5] = 0xBC;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x04);
}

TEST_F(c8_opcode, SE_no_skip)
{
    uint8_t opcode[] = { 0x35, 0xAA, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[5] = 0xBC;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, SNE_no_skip)
{
    uint8_t opcode[] = { 0x45, 0xBC, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[5] = 0xBC;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, SNE_skip)
{
    uint8_t opcode[] = { 0x45, 0xAA, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[5] = 0xBC;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x04);
}

TEST_F(c8_opcode, SEV_skip)
{
    // reorder C6 to 6C
    uint8_t opcode[] = { 0x5C, 0x60, 0x00, 0x00, 0x56, 0xC0, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0x6] = 0xBC;
    c8->Vx[0xC] = 0xBC;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x04);

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x08);
}

TEST_F(c8_opcode, SEV_no_skip)
{
    // reorder C6 to 6C
    uint8_t opcode[] = { 0x5C, 0x60, 0x56, 0xC0, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0x6] = 0xBC;
    c8->Vx[0xC] = 0xAA;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x04);
}

TEST_F(c8_opcode, LD)
{
    uint8_t opcode[] = { 0x6C, 0xBC, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x00;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0xBC);
}

TEST_F(c8_opcode, ADD_Vx)
{
    uint8_t opcode[] = { 0x7C, 0xBC, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x72;
    c8->Vx[0xF] = 0x00;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], (0xBC + 0x72) % 0x100);
    EXPECT_EQ(c8->Vx[0xF], 0x00);
}

TEST_F(c8_opcode, LD_V)
{
    uint8_t opcode[] = { 0x8C, 0x40, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x72;
    c8->Vx[0x4] = 0xF1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], c8->Vx[0x4]);
}

TEST_F(c8_opcode, OR_V)
{
    uint8_t opcode[] = { 0x8C, 0x41, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x72;
    c8->Vx[0x4] = 0xF1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0x72 | 0xF1);
}

TEST_F(c8_opcode, AND_V)
{
    uint8_t opcode[] = { 0x8C, 0x42, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x72;
    c8->Vx[0x4] = 0xF1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0x72 & 0xF1);
}

TEST_F(c8_opcode, XOR_V)
{
    uint8_t opcode[] = { 0x8C, 0x43, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x72;
    c8->Vx[0x4] = 0xF1;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0x72 ^ 0xF1);
}

TEST_F(c8_opcode, ADD_V_no_carry)
{
    uint8_t opcode[] = { 0x8C, 0x44, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x33;
    c8->Vx[0x4] = 0xA7;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0x33 + 0xA7);
    EXPECT_EQ(c8->Vx[0xF], 0x00);
}

TEST_F(c8_opcode, ADD_V_carry)
{
    uint8_t opcode[] = { 0x8C, 0x44, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x76;
    c8->Vx[0x4] = 0xA7;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], (0x76 + 0xA7) % 0x100);
    EXPECT_EQ(c8->Vx[0xF], 0x01);
}

TEST_F(c8_opcode, SUB_V_borrow)
{
    uint8_t opcode[] = { 0x8C, 0x45, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x33;
    c8->Vx[0x4] = 0xA7;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], (0x100 + 0x33 - 0xA7));
    EXPECT_EQ(c8->Vx[0xF], 0x00);
}

TEST_F(c8_opcode, SUB_V_no_borrow)
{
    uint8_t opcode[] = { 0x8C, 0x45, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x76;
    c8->Vx[0x4] = 0x31;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0x76 - 0x31);
    EXPECT_EQ(c8->Vx[0xF], 0x01);
}

TEST_F(c8_opcode, SHR_left_0)
{
    uint8_t opcode[] = { 0x8C, 0x46, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x32;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0x32 >> 1);
    EXPECT_EQ(c8->Vx[0xF], 0x00);
}

TEST_F(c8_opcode, SHR_left_1)
{
    uint8_t opcode[] = { 0x8C, 0x46, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x33;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0x33 >> 1);
    EXPECT_EQ(c8->Vx[0xF], 0x01);
}

TEST_F(c8_opcode, SUBN_borrow)
{
    uint8_t opcode[] = { 0x8C, 0x47, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x75;
    c8->Vx[0x4] = 0x31;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], (0x100 + 0x31 - 0x75));
    EXPECT_EQ(c8->Vx[0xF], 0x00);
}

TEST_F(c8_opcode, SUBN_no_borrow)
{
    uint8_t opcode[] = { 0x8C, 0x47, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x31;
    c8->Vx[0x4] = 0x75;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0x75 - 0x31);
    EXPECT_EQ(c8->Vx[0xF], 0x01);
}

TEST_F(c8_opcode, SHL_right_0)
{
    uint8_t opcode[] = { 0x8C, 0x4E, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x31;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0x31 << 1);
    EXPECT_EQ(c8->Vx[0xF], 0x00);
}

TEST_F(c8_opcode, SHL_right_1)
{
    uint8_t opcode[] = { 0x8C, 0x4E, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x81;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->Vx[0xC], 0x2);
    EXPECT_EQ(c8->Vx[0xF], 0x01);
}

TEST_F(c8_opcode, SNE_V_skip)
{
    uint8_t opcode[] = { 0x9C, 0x4E, 0x00, 0x00, 0x94, 0xCE,};
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x81;
    c8->Vx[0x4] = 0xFF;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x04);

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x08);
}

TEST_F(c8_opcode, SNE_V_no_skip)
{
    uint8_t opcode[] = { 0x9C, 0x4E, 0x94, 0xCE,};
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x81;
    c8->Vx[0x4] = 0x81;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x04);
}

TEST_F(c8_opcode, LD_I)
{
    uint8_t opcode[] = { 0xAF, 0xBC, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->I = 0x0000;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
    EXPECT_EQ(c8->I, 0xFBC);
}

TEST_F(c8_opcode, JP_V0_no_overflow)
{
    uint8_t opcode[] = { 0xBA, 0x12, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0] = 0x43;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, 0xA12 + 0x43);
}

TEST_F(c8_opcode, JP_V0_overflow)
{
    uint8_t opcode[] = { 0xBF, 0x12, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0] = 0xFA;

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, (0xF12 + 0xFA) & 0xFFFF);
}

TEST_F(c8_opcode, RND)
{
    uint8_t opcode[] = {
        0xCC, 0xA1,
        0xCC, 0xA1,
        0xCC, 0xA1,
        0xCC, 0xA1,
    };
    C8_load_program(c8, opcode, NELEMS(opcode));

    auto rnd = C8_Random_new(TEST_SEED);

    for (int i=0; i<4; ++i)
    {
        uint8_t val = C8_Random_next(rnd) & 0xA1;
        C8_cycle(c8);
        EXPECT_EQ(val, c8->Vx[0xC]);
    }

    EXPECT_EQ(c8->PC, C8_START_ADDR + 4*2);

    C8_Random_free(&rnd);
}

TEST_F(c8_opcode, DISABLED_DRW)
{
    uint8_t opcode[] = { 0xDB, 0xA8, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    C8_cycle(c8);
    FAIL();
}

TEST_F(c8_opcode, SKP_Vx_skip)
{
    uint8_t opcode[] = { 0xEA, 0x9E, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xA] = 3;
    C8_set_key(c8, 0x3, 1);

    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x04);
}

TEST_F(c8_opcode, SKP_Vx_no_skip)
{
    uint8_t opcode[] = { 0xEA, 0x9E, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xA] = 3;
    C8_set_key(c8, 0x3, 0);
    
    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, SKP_Vx_too_big)
{
    uint8_t opcode[] = { 0xEA, 0x9E, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xA] = 17;
    C8_set_key(c8, 0xA, 1);
    
    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, SKNP_Vx_skip)
{
    uint8_t opcode[] = { 0xEA, 0xA1, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xA] = 3;
    C8_set_key(c8, 0x3, 0);
    
    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x04);
}

TEST_F(c8_opcode, SKNP_Vx_no_skip)
{
    uint8_t opcode[] = { 0xEA, 0xA1, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xA] = 3;
    C8_set_key(c8, 0x3, 1);
    
    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, SKNP_Vx_too_big)
{
    uint8_t opcode[] = { 0xEA, 0xA1, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xA] = 17;
    C8_set_key(c8, 0xA, 1);
    
    C8_cycle(c8);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, LD_Vx_DT)
{
    uint8_t opcode[] = { 0xF8, 0x07, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->DT = 0xCC;
    
    C8_cycle(c8);
    EXPECT_EQ(c8->Vx[0x08], c8->DT);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, LD_Vx_K)
{
    //                   block       0x204
    uint8_t opcode[] = { 0xF8, 0x0A, 0xB2, 0x04, };
    C8_load_program(c8, opcode, NELEMS(opcode));
    
    // set key first, shouldn't unblock
    c8->Vx[0x8] = 0;
    C8_set_key(c8, 0xA, 1);

    // block execution until key is pressed
    C8_cycle(c8);
    ASSERT_EQ(c8->PC, C8_START_ADDR + 0x02);
    for (int i=0; i<100; ++i)
    {
        C8_cycle(c8);
        ASSERT_EQ(c8->PC, C8_START_ADDR + 0x02);
    }

    // key was not pressed, should block
    C8_set_key(c8, 0xB, 0);
    C8_cycle(c8);
    ASSERT_EQ(c8->PC, C8_START_ADDR + 0x02);
    ASSERT_EQ(c8->Vx[0x8], 0);

    // key was pressed
    C8_set_key(c8, 0xB, 1);
    C8_cycle(c8);
    ASSERT_EQ(c8->PC, C8_START_ADDR + 0x04);
    ASSERT_EQ(c8->Vx[0x8], 0xB);
}

TEST_F(c8_opcode, LD_DT_Vx)
{
    uint8_t opcode[] = { 0xF8, 0x15, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0x8] = 0x18;
    
    C8_cycle(c8);
    EXPECT_EQ(c8->Vx[0x08], c8->DT);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, LD_ST_Vx)
{
    uint8_t opcode[] = { 0xF8, 0x18, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0x8] = 0x18;
    
    C8_cycle(c8);
    EXPECT_EQ(c8->Vx[0x08], c8->ST);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, ADD_I_Vx)
{
    uint8_t opcode[] = { 0xF8, 0x1E, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    // CONSIDER what happens when I >= 0x1000?
    // CONSIDER what happens when I overflows?
    c8->Vx[0x8] = 0x25;
    c8->I = 0x062A;

    C8_cycle(c8);
    EXPECT_EQ(c8->I, (0x062A + 0x25));
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, ADD_I_Vx_overflow)
{
    uint8_t opcode[] = { 0xF8, 0x1E, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0x8] = 0x25;
    c8->I = 0x0FFA;

    EXPECT_DEBUG_DEATH(C8_cycle(c8), "Assertion .* failed");
}

TEST_F(c8_opcode, LD_F_Vx_multidigit)
{
    uint8_t opcode[] = { 0xFC, 0x29, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0xFA;

    C8_cycle(c8);
    EXPECT_EQ(c8->I, (5 * 0xA));
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, LD_F_Vx_single_digit)
{
    uint8_t opcode[] = { 0xFC, 0x29, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->Vx[0xC] = 0x9;

    C8_cycle(c8);
    EXPECT_EQ(c8->I, (5 * 0x9));
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, LD_B_Vx_normal)
{
    uint8_t opcode[] = { 0xFC, 0x33, 0xFC, 0x33, 0xFC, 0x33, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->I = 0x509;
    c8->Vx[0xC] = 209;

    C8_cycle(c8);
    EXPECT_EQ(c8->Ram[c8->I + 0], 2);
    EXPECT_EQ(c8->Ram[c8->I + 1], 0);
    EXPECT_EQ(c8->Ram[c8->I + 2], 9);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);


    c8->I = 0x505;
    c8->Vx[0xC] = 0;

    C8_cycle(c8);
    EXPECT_EQ(c8->Ram[c8->I + 0], 0);
    EXPECT_EQ(c8->Ram[c8->I + 1], 0);
    EXPECT_EQ(c8->Ram[c8->I + 2], 0);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x04);


    c8->I = 0x501;
    c8->Vx[0xC] = 97;

    C8_cycle(c8);
    EXPECT_EQ(c8->Ram[c8->I + 0], 0);
    EXPECT_EQ(c8->Ram[c8->I + 1], 9);
    EXPECT_EQ(c8->Ram[c8->I + 2], 7);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x06);
}

TEST_F(c8_opcode, LD_B_Vx_I_overflow)
{
    uint8_t opcode[] = { 0xFC, 0x33, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->I = 0x0FFE;
    c8->Vx[0xC] = 0x9;

    EXPECT_DEBUG_DEATH(C8_cycle(c8), "Assertion .* failed");
}

TEST_F(c8_opcode, LD_Ram_Vx_normal)
{
    uint8_t opcode[] = { 0xFB, 0x55, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    uint8_t start_val = 0x60;
    c8->I = 0x0515;
    for (size_t i=0; i<NELEMS(c8->Vx); ++i)
    {
        c8->Vx[i] = start_val + i;
    }

    C8_cycle(c8);
    compare_ram_to_regs(0xB);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, LD_Ram_Vx_overflow)
{
    uint8_t opcode[] = { 0xFB, 0x55, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->I = C8_LAST_ADDR - 0xA;

    EXPECT_DEBUG_DEATH(C8_cycle(c8), "Assertion .* failed");
}

TEST_F(c8_opcode, LD_Vx_Ram_normal)
{
    uint8_t opcode[] = { 0xFB, 0x65, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    uint8_t start_val = 0x60;
    c8->I = 0x0515;
    for (size_t i=0; i<NELEMS(c8->Vx); ++i)
    {
        c8->Ram[c8->I + i] = start_val + i;
    }

    C8_cycle(c8);
    compare_ram_to_regs(0xB);
    EXPECT_EQ(c8->PC, C8_START_ADDR + 0x02);
}

TEST_F(c8_opcode, LD_Vx_Ram_overflow)
{
    uint8_t opcode[] = { 0xFB, 0x65, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    c8->I = C8_LAST_ADDR - 0xA;

    EXPECT_DEBUG_DEATH(C8_cycle(c8), "Assertion .* failed");
}

TEST_F(c8_opcode, bad_opcode)
{
    uint8_t opcode[] = { 0xFF, 0xFF, };
    C8_load_program(c8, opcode, NELEMS(opcode));

    EXPECT_DEBUG_DEATH(C8_cycle(c8), "Assertion `!\"Bad Opcode\"' failed");
}
