#include "c8_private.h"

#include <assert.h>
#include "cummon.h"


void C8_exec_opcode(C8_ptr c8, uint16_t opcode)
{
    // registers for commands 0x_XY_
    uint8_t *Vx = c8->Vx + NIBBLE_3(opcode);
    uint8_t *Vy = c8->Vx + NIBBLE_2(opcode);

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
        if (*Vx == BIT_LO_8(opcode))
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0x4)
    {
        // SNE VX, byte
        if (*Vx != BIT_LO_8(opcode))
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0x5
          && BIT_LO_4(opcode) == 0x0)
    {
        // SE Vx, Vy
        if (*Vx == *Vy)
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0x6)
    {
        // LD Vx, byte
        *Vx = BIT_LO_8(opcode);
    }
    else if (BIT_HI_4(opcode) == 0x7)
    {
        // ADD Vx, byte
        // note: no carry
        *Vx += BIT_LO_8(opcode);
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x0)
    {
        // LD Vx, Vy
        *Vx = *Vy;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x1)
    {
        // OR Vx, Vy
        *Vx |= *Vy;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x2)
    {
        // AND Vx, Vy
        *Vx &= *Vy;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x3)
    {
        // XOR Vx, Vy
        *Vx ^= *Vy;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x4)
    {
        // ADD Vx, Vy
        uint16_t res = *Vx + *Vy;
        *Vx = res;
        c8->Vx[0xF           ] = res > 0xFF;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x5)
    {
        // SUB Vx, Vy
        c8->Vx[0xF           ]  = *Vx > *Vy;
        *Vx -= *Vy;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x6)
    {
        // SHR Vx {, Vy}
        c8->Vx[0xF           ]   = LSB_BYTE(*Vx);
        *Vx >>= 1;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x7)
    {
        // SUBN Vx, Vy
        c8->Vx[0xF           ] = *Vx < *Vy;
        *Vx = *Vy - *Vx;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0xE)
    {
        // SHL Vx {, Vy}
        c8->Vx[0xF           ]   = MSB_BYTE(*Vx);
        *Vx <<= 1;
    }
    else if (BIT_HI_4(opcode) == 0x9
          && BIT_LO_4(opcode) == 0xE)
    {
        // SNE Vx, Vy
        if (*Vx != *Vy)
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0xA)
    {
        // LD I, addr
        c8->I = BIT_LO_12(opcode);
    }
    else if (BIT_HI_4(opcode) == 0xB)
    {
        // JP V0, addr
        c8->PC = c8->Vx[0] + BIT_LO_12(opcode);
    }
}