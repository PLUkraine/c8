#include "c8_private.h"

#include <assert.h>
#include "common.h"


#ifdef C8_LOG_OPCODE
#include "logging.h"
#define LOG_OPCODE(str, code, pc) log_info("OPCODE: %15s, 0x%04X; PC: %04X", str, code, pc-2)
#else
#define LOG_OPCODE(str, code, pc) Q_UNUSED(pc)
#endif



const uint16_t C8_DIGIT_SIZE    = 5;
const uint16_t C8_SPRITE_WIDTH  = 8;


void C8_exec_opcode(C8_ptr c8, uint16_t opcode)
{
    // registers for commands _XY_
    uint8_t   X = NIBBLE_3(opcode);
    uint8_t *Vx = c8->Vx + X;
    uint8_t *Vy = c8->Vx + NIBBLE_2(opcode);

    if (opcode == 0x00E0)
    {
        // CLS
        LOG_OPCODE("CLS", opcode, c8->PC);
        C8_Display_clear(c8->Display);
    }
    else if (opcode == 0x00EE)
    {
        // RET
        LOG_OPCODE("RET", opcode, c8->PC);
        c8->SP--;
        if (c8->SP == 0xFF) c8->SP = NELEMS(c8->Stack) - 1;
        c8->PC = c8->Stack[c8->SP];
    }
    else if (BIT_HI_4(opcode) == 0x1)
    {
        // SYS addr && JP addr
        LOG_OPCODE("SYS or JP", opcode, c8->PC);
        c8->PC = BIT_LO_12(opcode);
    }
    else if (BIT_HI_4(opcode) == 0x2)
    {
        // CALL addr
        LOG_OPCODE("CALL", opcode, c8->PC);
        c8->Stack[c8->SP++] = c8->PC;
        if (c8->SP >= NELEMS(c8->Stack)) c8->SP = 0;
        c8->PC = BIT_LO_12(opcode);
    }
    else if (BIT_HI_4(opcode) == 0x3)
    {
        // SE VX, byte
        LOG_OPCODE("SE VX, byte", opcode, c8->PC);
        if (*Vx == BIT_LO_8(opcode))
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0x4)
    {
        // SNE VX, byte
        LOG_OPCODE("SNE VX, byte", opcode, c8->PC);
        if (*Vx != BIT_LO_8(opcode))
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0x5
          && BIT_LO_4(opcode) == 0x0)
    {
        // SE Vx, Vy
        LOG_OPCODE("SE Vx, Vy", opcode, c8->PC);
        if (*Vx == *Vy)
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0x6)
    {
        // LD Vx, byte
        LOG_OPCODE("LD Vx, byte", opcode, c8->PC);
        *Vx = BIT_LO_8(opcode);
    }
    else if (BIT_HI_4(opcode) == 0x7)
    {
        // ADD Vx, byte
        // note: no carry
        LOG_OPCODE("ADD Vx, byte", opcode, c8->PC);
        *Vx += BIT_LO_8(opcode);
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x0)
    {
        // LD Vx, Vy
        LOG_OPCODE("LD Vx, Vy", opcode, c8->PC);
        *Vx = *Vy;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x1)
    {
        // OR Vx, Vy
        LOG_OPCODE("OR Vx, Vy", opcode, c8->PC);
        *Vx |= *Vy;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x2)
    {
        // AND Vx, Vy
        LOG_OPCODE("AND Vx, Vy", opcode, c8->PC);
        *Vx &= *Vy;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x3)
    {
        // XOR Vx, Vy
        LOG_OPCODE("XOR Vx, Vy", opcode, c8->PC);
        *Vx ^= *Vy;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x4)
    {
        // ADD Vx, Vy
        LOG_OPCODE("ADD Vx, Vy", opcode, c8->PC);
        uint16_t res = *Vx + *Vy;
        *Vx = (uint8_t) res;
        c8->Vx[0xF] = res > 0xFF;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x5)
    {
        // SUB Vx, Vy
        LOG_OPCODE("SUB Vx, Vy", opcode, c8->PC);
        c8->Vx[0xF]  = *Vx > *Vy;
        *Vx -= *Vy;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x6)
    {
        // SHR Vx {, Vy}
        LOG_OPCODE("SHR Vx {, Vy}", opcode, c8->PC);
        c8->Vx[0xF]   = LSB_BYTE(*Vx);
        *Vx >>= 1;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0x7)
    {
        // SUBN Vx, Vy
        LOG_OPCODE("SUBN Vx, Vy", opcode, c8->PC);
        c8->Vx[0xF] = *Vx < *Vy;
        *Vx = *Vy - *Vx;
    }
    else if (BIT_HI_4(opcode) == 0x8
          && BIT_LO_4(opcode) == 0xE)
    {
        // SHL Vx {, Vy}
        LOG_OPCODE("SHL Vx {, Vy}", opcode, c8->PC);
        c8->Vx[0xF]   = MSB_BYTE(*Vx);
        *Vx <<= 1;
    }
    else if (BIT_HI_4(opcode) == 0x9
          && BIT_LO_4(opcode) == 0x0)
    {
        // SNE Vx, Vy
        LOG_OPCODE("SNE Vx, Vy", opcode, c8->PC);
        if (*Vx != *Vy)
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0xA)
    {
        // LD I, addr
        LOG_OPCODE("LD I, addr", opcode, c8->PC);
        c8->I = BIT_LO_12(opcode);
    }
    else if (BIT_HI_4(opcode) == 0xB)
    {
        // JP V0, addr
        LOG_OPCODE("JP V0, addr", opcode, c8->PC);
        c8->PC = c8->Vx[0] + BIT_LO_12(opcode);
    }
    else if (BIT_HI_4(opcode) == 0xC)
    {
        // RND Vx, byte
        LOG_OPCODE("RND Vx, byte", opcode, c8->PC);
        *Vx = C8_Random_next(c8->Random) & BIT_LO_8(opcode);
    }
    else if (BIT_HI_4(opcode) == 0xD)
    {
        // DRW
        LOG_OPCODE("DRW", opcode, c8->PC);
        bool collision = 0;
        int i;
        for (i=0; i<BIT_LO_4(opcode); ++i)
        {
            int j;
            int ramPos = (c8->I + i) % C8_LAST_ADDR;
            for (j=0; j<C8_SPRITE_WIDTH; ++j)
            {
                int row = (*Vy + i) % C8_DISPLAY_HEIGHT;
                int col = (*Vx + j) % C8_DISPLAY_WIDTH;
                uint8_t pixel = c8->Ram[ramPos] & (1 << (C8_SPRITE_WIDTH - 1 - j));
                if (pixel)
                {
                    if (!C8_Display_pixel_toggle(c8->Display, row, col))
                    {
                        collision = 1;
                    }
                }
            }
        }
        c8->Vx[0xF] = collision;
    }
    else if (BIT_HI_4(opcode) == 0xE
          && BIT_LO_8(opcode) == 0x9E)
    {
        // SKP Vx
        LOG_OPCODE("SKP Vx", opcode, c8->PC);
        if (C8_Keyboard_get(c8->Keyboard, *Vx) == C8_KEY_DOWN)
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0xE
          && BIT_LO_8(opcode) == 0xA1)
    {
        // SKNP Vx
        LOG_OPCODE("SKNP Vx", opcode, c8->PC);
        if (C8_Keyboard_get(c8->Keyboard, *Vx) == C8_KEY_UP)
        {
            c8->PC += 2;
        }
    }
    else if (BIT_HI_4(opcode) == 0xF
          && BIT_LO_8(opcode) == 0x07)
    {
        // LD Vx, DT
        LOG_OPCODE("LD Vx, DT", opcode, c8->PC);
        *Vx = c8->DT;
    }
    else if (BIT_HI_4(opcode) == 0xF
          && BIT_LO_8(opcode) == 0x0A)
    {
        // LD Vx, K
        LOG_OPCODE("LD Vx, K", opcode, c8->PC);
        C8_Keyboard_lock(c8->Keyboard, Vx);
    }
    else if (BIT_HI_4(opcode) == 0xF
          && BIT_LO_8(opcode) == 0x15)
    {
        // LD DT, Vx
        LOG_OPCODE("LD DT, Vx", opcode, c8->PC);
        c8->DT = *Vx;
    }
    else if (BIT_HI_4(opcode) == 0xF
          && BIT_LO_8(opcode) == 0x18)
    {
        // LD ST, Vx
        LOG_OPCODE("LD ST, Vx", opcode, c8->PC);
        c8->ST = *Vx;
    }
    else if (BIT_HI_4(opcode) == 0xF
          && BIT_LO_8(opcode) == 0x1E)
    {
        // ADD I, Vx -> No Overflow Flag!
        LOG_OPCODE("ADD I, Vx", opcode, c8->PC);
        c8->I += *Vx;
        assert(c8->I <= C8_LAST_ADDR);
    }
    else if (BIT_HI_4(opcode) == 0xF
          && BIT_LO_8(opcode) == 0x29)
    {
        // LD F, Vx
        LOG_OPCODE("LD F, Vx", opcode, c8->PC);
        c8->I = C8_DIGIT_SIZE * (*Vx);
    }
    else if (BIT_HI_4(opcode) == 0xF
          && BIT_LO_8(opcode) == 0x33)
    {
        // LD B, Vx
        LOG_OPCODE("LD B, Vx", opcode, c8->PC);
        assert(c8->I + 2 <= C8_LAST_ADDR);

        int i;
        int vx = *Vx;
        for (i=0; i<3; ++i) {
            int loc = (c8->I + 2 - i);
            c8->Ram[loc] = vx % 10;
            vx /= 10;
        }
    }
    else if (BIT_HI_4(opcode) == 0xF
          && BIT_LO_8(opcode) == 0x55)
    {
        // LD [I], Vx
        LOG_OPCODE("LD [I], Vx", opcode, c8->PC);
        assert(c8->I + X <= C8_LAST_ADDR);

        int i;
        for (i=0; i<=X; ++i) {
            c8->Ram[c8->I + i] = c8->Vx[i];
        }
    }
    else if (BIT_HI_4(opcode) == 0xF
          && BIT_LO_8(opcode) == 0x65)
    {
        // LD Vx, [I]
        LOG_OPCODE("LD Vx, [I]", opcode, c8->PC);
        assert(c8->I + X <= C8_LAST_ADDR);

        int i;
        for (i=0; i<=X; ++i) {
            c8->Vx[i] = c8->Ram[c8->I + i];
        }
    }
    else {
        // Invalid opcode - crash the app
        assert(!"Bad Opcode");
    }
}