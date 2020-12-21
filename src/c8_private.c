#include <assert.h>

#include "c8_private.h"
#include "cummon.h"


void C8_exec_opcode(C8_ptr c8, uint16_t opcode)
{

    if (opcode == 0x00E0) 
    {
        // CLS
        // TODO implement display
        assert(0);
    }
    else if (opcode == 0x00EE)
    {
        // RET
        c8->PC = c8->Stack[c8->SP--];
        if (c8->SP == 0xFF) c8->SP = NELEMS(c8->Stack) - 1;
    }
    else if (BIT_HI_4(opcode) == 0x0 || BIT_HI_4(opcode) == 0x1)
    {
        // SYS addr && JP addr
        c8->PC = BIT_LO_12(opcode);
    }
    else if (BIT_HI_4(opcode) == 0x2)
    {
        // CALL addr
        c8->Stack[c8->SP++] = c8->PC;
        if (c8->SP >= NELEMS(c8->Stack)) c8->SP = 0;
        c8->PC = BIT_LO_12(opcode);
    }
    else if (BIT_HI_4(opcode) == 0x3)
    {
        // SE VX, byte
        if (c8->Vx[NIMB_3(opcode)] == BIT_LO_8(opcode))
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0x4)
    {
        // SNE VX, byte
        if (c8->Vx[NIMB_3(opcode)] != BIT_LO_8(opcode))
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0x5
          && BIT_LO_4(opcode) == 0x0)
    {
        // SE Vx, Vy
        if (c8->Vx[NIMB_3(opcode)] == c8->Vx[NIMB_2(opcode)])
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0x6)
    {
        // LD Vx, byte
        c8->Vx[NIMB_3(opcode)] = BIT_LO_8(opcode);
    }
    else if (BIT_HI_4(opcode) == 0x7)
    {
        // ADD Vx, byte
        // note: no carry
        c8->Vx[NIMB_3(opcode)] += BIT_LO_8(opcode);
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x0)
    {
        // LD Vx, Vy
        c8->Vx[NIMB_3(opcode)] = c8->Vx[NIMB_2(opcode)];
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x1)
    {
        // OR Vx, Vy
        c8->Vx[NIMB_3(opcode)] |= c8->Vx[NIMB_2(opcode)];
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x2)
    {
        // AND Vx, Vy
        c8->Vx[NIMB_3(opcode)] &= c8->Vx[NIMB_2(opcode)];
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x3)
    {
        // XOR Vx, Vy
        c8->Vx[NIMB_3(opcode)] ^= c8->Vx[NIMB_2(opcode)];
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x4)
    {
        // ADD Vx, Vy
        uint16_t res = c8->Vx[NIMB_3(opcode)] + c8->Vx[NIMB_2(opcode)];
        c8->Vx[NIMB_3(opcode)] = res;
        c8->Vx[0xF           ] = res > 0xFF;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x5)
    {
        // SUB Vx, Vy
        c8->Vx[0xF           ]  = c8->Vx[NIMB_3(opcode)] > c8->Vx[NIMB_2(opcode)];
        c8->Vx[NIMB_3(opcode)] -= c8->Vx[NIMB_2(opcode)];
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x6)
    {
        // SHR Vx {, Vy}
        c8->Vx[0xF           ]  = LSB(c8->Vx[NIMB_3(opcode)]);
        c8->Vx[NIMB_3(opcode)] >>= 1;
    }
}