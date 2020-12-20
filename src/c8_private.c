#include "c8_private.h"
#include "cummon.h"


void C8_exec_opcode(C8_ptr c8, uint16_t opcode)
{

    if (opcode == 0x00E0) 
    {
        // CLS
        // TODO implement display
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
}