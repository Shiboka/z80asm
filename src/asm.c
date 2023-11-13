#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asm.h"



/* ============================================ */
/* =              PUSH, POP                   = */
/* ============================================ */

int
push(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    switch(rd->regs[0] & 0x3f) {
        case HL:
            bytes[i] = 0xe5;
            return i+1;
        case AF:
            bytes[i] = 0xf5;
            return i+1;
        case BC:
            bytes[i] = 0xc5;
            return i+1;
        case DE:
            bytes[i] = 0xd5;
            return i+1;
    }

    return -1;
}

int
pop(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    switch(rd->regs[0] & 0x3f) {
        case HL:
            bytes[i] = 0xe1;
            return i+1;
        case AF:
            bytes[i] = 0xf1;
            return i+1;
        case BC:
            bytes[i] = 0xc1;
            return i+1;
        case DE:
            bytes[i] = 0xd1;
            return i+1;
    }

    return -1;
}

/* ============================================ */
/* =               INC, DEC                   = */
/* ============================================ */

int
inc(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    // IX/IY prefix byte
    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    // check registers
    switch(rd->regs[0] & 0x3f) {
        #define gen_case_inc(r, c) \
        case r:                    \
            bytes[i] = c;          \
            i++; break;            \

        gen_case_inc(A,      0x3c)
        gen_case_inc(B,      0x04)
        gen_case_inc(BC,     0x03)
        gen_case_inc(C,      0x0c)
        gen_case_inc(D,      0x14)
        gen_case_inc(DE,     0x13)
        gen_case_inc(E,      0x1c)
        gen_case_inc(H,      0x24)
        gen_case_inc(HL,     0x23)
        gen_case_inc(L,      0x2c)
        gen_case_inc(HL|PTR, 0x34)
        gen_case_inc(SP,     0x33)

        default:
            return -1;
    }

    // IX/IY + offset
    if(rd->regs[0] & O) {
        bytes[i] = rd->o;
        i++;
    }

    return i;
}

int
dec(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    // IX/IY prefix byte
    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    // check registers
    // HL will catch IX/IY too
    switch(rd->regs[0] & 0x3f) {
        #define gen_case(r, c) \
        case r:                \
            bytes[i] = c;      \
            i++; break;        \

        gen_case(A,      0x3d)
        gen_case(B,      0x05)
        gen_case(BC,     0x0b)
        gen_case(C,      0x0d)
        gen_case(D,      0x15)
        gen_case(DE,     0x1b)
        gen_case(E,      0x1d)
        gen_case(H,      0x25)
        gen_case(HL,     0x2b)
        gen_case(L,      0x2d)
        gen_case(HL|PTR, 0x35)
        gen_case(SP,     0x3b)

        default:
            return -1;
    }

    // IX/IY + offset
    if(rd->regs[0] & O) {
        bytes[i] = rd->o;
        i++;
    }

    return i;
}

/* ============================================ */
/* =         AND, OR, XOR, CP, SUB            = */
/* ============================================ */

int
and(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0xa6;
        i++;

        if(rd->regs[0] & O) {
            bytes[i] = rd->o;
            i++;
        }

        return i;
    }

    if(rd->regs[0] == N) {
        bytes[i] = 0xe6;
        i++;
        bytes[i] = rd->n[0];
        return i+1;
    }

    switch(rd->regs[0] & 0x3f) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0xa0 + (rd->regs[0]&0x3f) - 1;
            return i+1;
    }

    return -1;
}

int
or(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0xb6;
        i++;

        if(rd->regs[0] & O) {
            bytes[i] = rd->o;
            i++;
        }

        return i;
    }

    if(rd->regs[0] == N) {
        bytes[i] = 0xf6;
        i++;
        bytes[i] = rd->n[0];
        return i+1;
    }

    switch(rd->regs[0] & 0x3f) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0xb0 + (rd->regs[0]&0x3f) - 1;
            return i+1;
    }

    return -1;
}

int
xor(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0xae;
        i++;

        if(rd->regs[0] & O) {
            bytes[i] = rd->o;
            i++;
        }

        return i;
    }

    if(rd->regs[0] == N) {
        bytes[i] = 0xee;
        i++;
        bytes[i] = rd->n[0];
        return i+1;
    }

    switch(rd->regs[0] & 0x3f) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0xa8 + (rd->regs[0]&0x3f) - 1;
            return i+1;
    }

    return -1;
}

int
cp(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0xbe;
        i++;

        if(rd->regs[0] & O) {
            bytes[i] = rd->o;
            i++;
        }

        return i;
    }

    if(rd->regs[0] == N) {
        bytes[i] = 0xfe;
        i++;
        bytes[i] = rd->n[0];
        return i+1;
    }

    switch(rd->regs[0] & 0x3f) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0xb8 + (rd->regs[0]&0x3f) - 1;
            return i+1;
    }
    
    return -1;
}

int
sub(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0x96;
        i++;

        if(rd->regs[0] & O) {
            bytes[i] = rd->o;
            i++;
        }

        return i;
    }

    if(rd->regs[0] == N) {
        bytes[i] = 0xd6;
        i++;
        bytes[i] = rd->n[0];
        return i+1;
    }

    switch(rd->regs[0] & 0x3f) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0x90 + (rd->regs[0]&0x3f) - 1;
            return i+1;
    }

    return -1;
}

/* ============================================ */
/* =               ADC, SBC                   = */
/* ============================================ */

int
adc(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] == A) {
        if(rd->regs[1] & IX) {
            bytes[i] = 0xdd;
            i++;
        } else if(rd->regs[1] & IY) {
            bytes[i] = 0xfd;
            i++;
        }

        if((rd->regs[1] & 0x3f) == (HL|PTR)) {
            bytes[i] = 0x8e;
            i++;

            if(rd->regs[1] & O) {
                bytes[i] = rd->o;
                i++;
            }

            return i;
        }

        if(rd->regs[1] == N) {
            bytes[i] = 0xce;
            i++;
            bytes[i] = rd->n[0];
            return i+1;
        }

        switch(rd->regs[1] & 0x3f) {
            case A:
            case B:
            case C:
            case D:
            case E:
            case H:
            case L:
                bytes[i] = 0x88 + (rd->regs[1]&0x3f) - 1;
                return i+1;
        }
    }

    else if(rd->regs[0] == HL) {
        bytes[i] = 0xed;
        i++;

        switch(rd->regs[1]) {
            case BC:
                bytes[i] = 0x4a;
                return i+1;
            case DE:
                bytes[i] = 0x5a;
                return i+1;
            case HL:
                bytes[i] = 0x6a;
                return i+1;
            case SP:
                bytes[i] = 0x7a;
                return i+1;
        }
    }

    return -1;
}

int
sbc(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] == A) {
        if(rd->regs[1] & IX) {
            bytes[i] = 0xdd;
            i++;
        } else if(rd->regs[1] & IY) {
            bytes[i] = 0xfd;
            i++;
        }

        if((rd->regs[1] & 0x3f) == (HL|PTR)) {
            bytes[i] = 0x9e;
            i++;

            if(rd->regs[1] & O) {
                bytes[i] = rd->o;
                i++;
            }

            return i;
        }

        if(rd->regs[1] == N) {
            bytes[i] = 0xde;
            i++;
            bytes[i] = rd->n[0];
            return i+1;
        }

        switch(rd->regs[1] & 0x3f) {
            case A:
            case B:
            case C:
            case D:
            case E:
            case H:
            case L:
                bytes[i] = 0x98 + (rd->regs[1]&0x3f) - 1;
                return i+1;
        }
    }

    else if(rd->regs[0] == HL) {
        bytes[i] = 0xed;
        i++;

        switch(rd->regs[1]) {
            case BC:
                bytes[i] = 0x42;
                return i+1;
            case DE:
                bytes[i] = 0x52;
                return i+1;
            case HL:
                bytes[i] = 0x62;
                return i+1;
            case SP:
                bytes[i] = 0x72;
                return i+1;
        }
    }

    return -1;
}

/* ============================================ */
/* =            RES, SET, BIT                 = */
/* ============================================ */

int
res(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] != N || rd->n[0] > 7)
        return -1;

    if(rd->regs[1] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[1] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    bytes[i] = 0xcb;
    i++;

    if(rd->regs[1] & O) {
        bytes[i] = rd->o;
        i++;
    }

    if((rd->regs[1] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0x86 + 8 * rd->n[1];
        return i+1;
    }

    switch(rd->regs[1]) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0x80 + 8 * rd->n[0] + rd->regs[1] - 1;
            return i+1;
    }

    return -1;
}

int
set(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] != N || rd->n[1] > 7)
        return -1;

    if(rd->regs[1] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[1] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    bytes[i] = 0xcb;
    i++;

    if(rd->regs[1] & O) {
        bytes[i] = rd->o;
        i++;
    }

    if((rd->regs[1] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0xc6 + 8 * rd->n[0];
        return i+1;
    }

    switch(rd->regs[1]) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0xc0 + 8 * rd->n[0] + rd->regs[1] - 1;
            return i+1;
    }

    return -1;
}

int
bit(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] != N || rd->n[1] > 7)
        return -1;

    if(rd->regs[1] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[1] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    bytes[i] = 0xcb;
    i++;

    if(rd->regs[1] & O) {
        bytes[i] = rd->o;
        i++;
    }

    if((rd->regs[1] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0x46 + 8 * rd->n[0];
        return i+1;
    }

    switch(rd->regs[1]) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0x40 + 8 * rd->n[0] + rd->regs[1] - 1;
            return i+1;
    }

    return -1;
}

/* ============================================ */
/* =       RL, RR, RRC, SLA, SRA, SRL         = */
/* ============================================ */

int
rl(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    bytes[i] = 0xcb;
    i++;

    if(rd->regs[0] & O) {
        bytes[i] = rd->o;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0x16;
        return i+1;
    }

    switch(rd->regs[0]) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0x10 + rd->regs[0] - 1;
            return i+1;
    }

    return -1;
}

int
rr(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    bytes[i] = 0xcb;
    i++;

    if(rd->regs[0] & O) {
        bytes[i] = rd->o;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0x1e;
        return i+1;
    }

    switch(rd->regs[0]) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0x18 + rd->regs[0] - 1;
            return i+1;
    }

    return -1;
}

int
rrc(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    bytes[i] = 0xcb;
    i++;

    if(rd->regs[0] & O) {
        bytes[i] = rd->o;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0x0e;
        return i+1;
    }

    switch(rd->regs[0]) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0x08 + rd->regs[0] - 1;
            return i+1;
    }

    return -1;
}

int
sla(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    bytes[i] = 0xcb;
    i++;

    if(rd->regs[0] & O) {
        bytes[i] = rd->o;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0x26;
        return i+1;
    }

    switch(rd->regs[0]) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0x20 + rd->regs[0] - 1;
            return i+1;
    }

    return -1;
}

int
sra(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    bytes[i] = 0xcb;
    i++;

    if(rd->regs[0] & O) {
        bytes[i] = rd->o;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0x2e;
        return i+1;
    }

    switch(rd->regs[0]) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0x28 + rd->regs[0] - 1;
            return i+1;
    }

    return -1;
}

int
srl(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    bytes[i] = 0xcb;
    i++;

    if(rd->regs[0] & O) {
        bytes[i] = rd->o;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0x3e;
        return i+1;
    }

    switch(rd->regs[0]) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            bytes[i] = 0x38 + rd->regs[0] - 1;
            return i+1;
    }

    return -1;
}

/* ============================================ */
/* =                 MISC                     = */
/* ============================================ */

int
ld(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int j, i = 0;
    int r16 = 0;
    int hl = 0;
    int sp = 0;

    // Optional Prefix bytes
    if((rd->regs[0] | rd->regs[1]) & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if((rd->regs[0] | rd->regs[1]) & IY) {
        bytes[i] = 0xfd;
        i++;
    } else if(rd->regs[1] == (N|PTR)) {
        bytes[i] = 0xed;
        i++;
    }

    // Edge Cases
    /* ========================== */
    if(rd->regs[0] == A) {
        switch(rd->regs[1]) {
            case BC|PTR:
                bytes[i] = 0x0a;
                return i+1;
            case DE|PTR:
                bytes[i] = 0x1a;
                return i+1;
            case I:
                bytes[i] = 0xed;
                i++;
                bytes[i] = 0x57;
                return i+1;
            case R:
                bytes[i] = 0xed;
                i++;
                bytes[i] = 0x5f;
                return i+1;
            case N|PTR:
                bytes[i] = 0x3a;
                i++;
                bytes[i] = rd->n[0];
                i++;
                bytes[i] = rd->n[0] >> 8;
                return i+1;
        }
    }

    if(rd->regs[0] == I) {
        if(rd->regs[1] != A)
            return -1;

        bytes[0] = 0xed;
        bytes[1] = 0x47;
        return 2;
    }

    if(rd->regs[0] == R) {
        if(rd->regs[1] != A)
            return -1;

        bytes[0] = 0xed;
        bytes[1] = 0x4f;
        return 2;
    }

    if(rd->regs[0] == (BC|PTR)) {
        if(rd->regs[1] != A)
            return -1;

        bytes[0] = 0x02;
        return 1;
    }

    if(rd->regs[0] == (DE|PTR)) {
        if(rd->regs[1] != A)
            return -1;

        bytes[0] = 0x12;
        return 1;
    }

    if(rd->regs[0] == (N|PTR)) {
        switch(rd->regs[1] & 0x3f) {
            case A:
                bytes[i] = 0x32;
                i++; break;
            case BC:
                bytes[i] = 0xed;
                i++;
                bytes[i] = 0x43;
                i++; break;
            case DE:
                bytes[i] = 0xed;
                i++;
                bytes[i] = 0x53;
                i++; break;
            case HL:
                bytes[i] = 0x22;
                i++; break;
            case SP:
                bytes[i] = 0x73;
                i++; break;
            default:
                return -1;
        }

        bytes[i] = rd->n[0];
        i++;
        bytes[i] = rd->n[0] >> 8;
        return i+1;
    }
    /* ========================== */

    // Probably not needed or should be moved
    if(((rd->regs[0] & 0x3f) == H || (rd->regs[0] & 0x3f) == L)
        && rd->regs[1] & (IX | IY))
            return -1;

    // All other cases
    /* ========================== */
    switch(rd->regs[0] & 0x3f) {
        case A:
            bytes[i] = 0x78;
            break;
        case B:
            bytes[i] = 0x40;
            break;
        case C:
            bytes[i] = 0x48;
            break;
        case D:
            bytes[i] = 0x50;
            break;
        case E:
            bytes[i] = 0x58;
            break;
        case H:
            bytes[i] = 0x60;
            break;
        case L:
            bytes[i] = 0x68;
            break;
        case BC:
            bytes[i] = 0x00;
            r16 = 1;
            break;
        case DE:
            bytes[i] = 0x10;
            r16 = 1;
            break;
        case HL:
            bytes[i] = 0x20;
            r16 = 1;
            hl = 1;
            break;
        case SP:
            bytes[i] = 0x30;
            r16 = 1;
            sp = 1;
            break;
        case (HL|PTR):
            bytes[i] = 0x70;
    }

    switch(rd->regs[1] & 0x3f) {
        case A:
        case B:
        case C:
        case D:
        case E:
        case H:
        case L:
            if(r16)
                return -1;
            bytes[i] += (rd->regs[1]&0x3f) - 1;
            i++; break;
        case LAB:
        case N:
            if(r16)
                bytes[i]++;
            else
                bytes[i] -= 58;
            i++; break;
        case (N|PTR):
            if(!r16)
                return -1;
            if(hl)
                bytes[i] += 10;
            else
                bytes[i] += 75;
            i++; break;
        case HL:
            if(!sp)
                return -1;
            bytes[i] += 0xc9;
            i++;
        case HL|PTR:
            if(r16)
                return -1;
            bytes[i] += 6;
            i++; break;

    }
    /* ========================== */

    // Insert IX/IY offset if there is one
    if(rd->regs[0] & O || rd->regs[1] & O) {
        bytes[i] = rd->o;
        i++;
    }

    // Insert immediate value if there is one
    if(rd->regs[1] == N || rd->regs[1] == (N|PTR)) {
        bytes[i] = rd->n[0];
        i++;

        if(r16) {
            bytes[i] = rd->n[0] >> 8;
            i++;
        }
    }

    // Insert label address if there is one
    if(rd->regs[1] == LAB) {
        if(pd->pass == 1)
            return i+2;

        for(j = 0; j < pd->labn; j++)
            if(!strcmp(pd->labs[j], rd->lab))
                break;

        if(j == pd->labn)
            return -1;

        uint16_t n = pd->org + pd->labo[j];

        bytes[i] = n;
        i++;
        bytes[i] = n >> 8;
        i++;
    }

    return i;
}

int
jp(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[1] && rd->regs[1] != N && rd->regs[1] != LAB)
        return -1;

    if(rd->regs[0] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[0] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    if((rd->regs[0] & 0x3f) == (HL|PTR)) {
        bytes[i] = 0xe9;
        return i+1;
    }

    switch(rd->regs[0]) {
        case LAB:
        case N:
            bytes[0] = 0xc3;
            break;
        case M:
            bytes[0] = 0xfa;
            break;
        case Z:
            bytes[0] = 0xca;
            break;
        case NZ:
            bytes[0] = 0xc2;
            break;
        case C:
            bytes[0] = 0xda;
            break;
        case NC:
            bytes[0] = 0xd2;
            break;
        case P:
            bytes[0] = 0xf2;
            break;
        case PE:
            bytes[0] = 0xea;
            break;
        case PO:
            bytes[0] = 0xe2;
            break;
        default:
            return -1;
    }

    if(rd->regs[0] == LAB || rd->regs[1] == LAB) {
        if(pd->pass == 1)
            return 3;

        for(i = 0; i < pd->labn; i++)
            if(!strcmp(pd->labs[i], rd->lab))
                break;

        if(i == pd->labn)
            return -1;

        uint16_t n = pd->org + pd->labo[i];

        bytes[1] = n;
        bytes[2] = n >> 8;
        return 3;
    }

    bytes[1] = rd->n[0];
    bytes[2] = rd->n[0] >> 8;
    return 3;
}

int
jr(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i;

    if(rd->regs[1] && rd->regs[1] != N && rd->regs[1] != LAB)
        return -1;

    switch(rd->regs[0]) {
        case LAB:
        case N:
            bytes[0] = 0x18;
            break;
        case Z:
            bytes[0] = 0x28;
            break;
        case NZ:
            bytes[0] = 0x20;
            break;
        case C:
            bytes[0] = 0x38;
            break;
        case NC:
            bytes[0] = 0x30;
            break;
        default:
            return -1;
    }

    if(rd->regs[0] == LAB || rd->regs[1] == LAB) {
        if(pd->pass == 1)
            return 2;

        for(i = 0; i < pd->labn; i++)
            if(!strcmp(pd->labs[i], rd->lab))
                break;

        if(pd->labn = i)
            return -1;

        bytes[1] = pd->labo[i] - (pd->bt + 2);
        return 2;
    }

    bytes[1] = rd->n[0];
    return 2;
}

int
add(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if((rd->regs[0] | rd->regs[1]) & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if((rd->regs[0] | rd->regs[1]) & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    if(rd->regs[0] == A) {
        if((rd->regs[1] & 0x3f) == (HL|PTR)) {
            bytes[i] = 0x86;
            i++;

            if(rd->regs[1] & O) {
                bytes[i] = rd->o;
                i++;
            }

            return i;
        }

        if(rd->regs[1] == N) {
            bytes[i] = 0xc6;
            i++;
            bytes[i] = rd->n[0];
            return i+1;
        }

        switch(rd->regs[1] & 0x3f) {
            case A:
            case B:
            case C:
            case D:
            case E:
            case H:
            case L:
                bytes[i] = 0x80 + (rd->regs[1]&0x3f) - 1;
                return i+1;
        }
    }

    else if((rd->regs[0] & 0x3f) == HL) {
        switch(rd->regs[1] & 0x3f) {
            case BC:
                bytes[i] = 0x09;
                return i+1;
            case DE:
                bytes[i] = 0x19;
                return i+1;
            case HL:
                bytes[i] = 0x29;
                return i+1;
            case SP:
                bytes[i] = 0x39;
                return i+1;
        }
    }

    return -1;
}

int
call(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i;

    if(rd->regs[1] && rd->regs[1] != N && rd->regs[1] != LAB)
        return -1;

    switch(rd->regs[0]) {
        case LAB:
        case N:
            bytes[0] = 0xcd;
            break;
        case M:
            bytes[0] = 0xfc;
            break;
        case Z:
            bytes[0] = 0xcc;
            break;
        case NZ:
            bytes[0] = 0xc4;
            break;
        case C:
            bytes[0] = 0xdc;
            break;
        case NC:
            bytes[0] = 0xd4;
            break;
        case P:
            bytes[0] = 0xf4;
            break;
        case PE:
            bytes[0] = 0xec;
            break;
        case PO:
            bytes[0] = 0xe4;
            break;
        default:
            return -1;
    }

    if(rd->regs[0] == LAB || rd->regs[1] == LAB) {
        if(pd->pass == 1)
            return 3;

        for(i = 0; i < pd->labn; i++)
            if(!strcmp(pd->labs[i], rd->lab))
                break;

        if(i == pd->labn)
            return -1;

        uint16_t n = pd->org + pd->labo[i];

        bytes[1] = n;
        bytes[2] = n >> 8;
        return 3;
    }

    bytes[1] = rd->n[0];
    bytes[2] = rd->n[0] >> 8;
    return 3;
}

int
ret(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    if(!rd->regs[0]) {
        bytes[0] = 0xc9;
        return 1;
    }

    switch(rd->regs[0]) {
        #define gen_case_ret(r, c) \
        case r: \
            bytes[0] = c; \
            return 1; \

        gen_case_ret(Z,  0xc8)
        gen_case_ret(NZ, 0xc0)
        gen_case_ret(C,  0xd8)
        gen_case_ret(NC, 0xd0)
        gen_case_ret(M,  0xf8)
        gen_case_ret(P,  0xf0)
        gen_case_ret(PE, 0xe8)
        gen_case_ret(PO, 0xe0)
    }

    return -1;
}

int
rst(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    if(rd->regs[0] != N)
        return -1;

    switch(rd->n[0]) {
        case 0:
            bytes[0] = 0xc7;
            return 1;
        case 8:
            bytes[0] = 0xcf;
            return 1;
        case 16:
            bytes[0] = 0xd7;
            return 1;
        case 24:
            bytes[0] = 0xdf;
            return 1;
        case 32:
            bytes[0] = 0xe7;
            return 1;
        case 40:
            bytes[0] = 0xef;
            return 1;
        case 48:
            bytes[0] = 0xf7;
            return 1;
        case 56:
            bytes[0] = 0xff;
            return 1;
    }

    return -1;
}

int
ex(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i = 0;

    if(rd->regs[1] & IX) {
        bytes[i] = 0xdd;
        i++;
    } else if(rd->regs[1] & IY) {
        bytes[i] = 0xfd;
        i++;
    }

    if(rd->regs[0] == (SP|PTR) && (rd->regs[1] & 0x3f) == HL)
        bytes[i] = 0xe3;
    else if(rd->regs[0] == AF && rd->regs[1] == (AF|AR))
        bytes[i] = 0x08;
    else if(rd->regs[0] == DE && rd->regs[1] == HL)
        bytes[i] = 0xeb;
    else
        return -1;

    return i+1;
}

int
im(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    if(rd->regs[0] != N)
        return -1;

    bytes[0] = 0xed;

    switch(rd->n[0]) {
        case 0:
            bytes[1] = 0x46;
            return 2;
        case 1:
            bytes[1] = 0x56;
            return 2;
        case 2:
            bytes[1] = 0x5e;
            return 2;
    }

    return -1;
}

int
in(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    if(rd->regs[0] == A && rd->regs[1] == (N|PTR)) {
        bytes[0] = 0xdb;
        bytes[1] = rd->n[0];
        return 2;
    } else if(rd->regs[1] == (C|PTR))
        bytes[0] = 0xed;

    switch(rd->regs[0]) {
        #define gen_case_in(r, c) \
        case r:                   \
            bytes[1] = c;         \
            return 2;             \

        gen_case_in(A, 0x78)
        gen_case_in(B, 0x40)
        gen_case_in(C, 0x48)
        gen_case_in(D, 0x50)
        gen_case_in(E, 0x58)
        gen_case_in(H, 0x60)
        gen_case_in(L, 0x68)
        gen_case_in(F, 0x70)
    }

    return -1;
}

int
out(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    if(rd->regs[0] == (N|PTR) && rd->regs[1] == A) {
        bytes[0] = 0xd3;
        bytes[1] = rd->n[0];
        return 2;
    }

    if(rd->regs[0] != (C|PTR))
        return -1;

    bytes[0] = 0xed;

    switch(rd->regs[1]) {
        case A:
            bytes[1] = 0x79;
            return 2;
        case B:
            bytes[1] = 0x41;
            return 2;
        case C:
            bytes[1] = 0x49;
            return 2;
        case D:
            bytes[1] = 0x51;
            return 2;
        case E:
            bytes[1] = 0x59;
            return 2;
        case H:
            bytes[1] = 0x61;
            return 2;
        case L:
            bytes[1] = 0x69;
            return 2;
    }

    return -1;
}

int
org(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    if(rd->regs[0] != N || rd->regs[1])
        return -1;

    pd->org = rd->n[0];
    return 0;
}

int
db(prog_data_t *pd, reg_data_t *rd, uint8_t *bytes)
{
    int i, j, k, l;

    k = l = 0;
    for(i = 0; i < rd->regn; i++) {
        if(rd->regs[i] == STR) {
            for(j = 0; rd->db[j]; j++)
                bytes[k+j] = rd->db[j];
            k += j;
        } else if(rd->regs[i] == N) {
            bytes[k] = rd->n[l];
            k++; l++;
        } else
            return -1;
    }

    return k;
}

/* ============================================ */


int
parse(char *line, reg_data_t *rd, prog_data_t *pd, int (**instr)(prog_data_t *, reg_data_t *, uint8_t *))
{
    char *pch[10];

    /* =================================== */

    if(!line[0])
        return 0;

    pch[0] = NULL;
    rd->regn = 0;
    line = trim(line);

    if(pch[1] = strchr(line, ':')) {
        pch[0] = line;
        pch[1][0] = 0;
        pch[1]++;
    } else
        pch[1] = line;

    pch[1] = trim(pch[1]);

    if(pch[2] = strchr(pch[1], ' ')) {
        pch[2][0] = 0;
        pch[2]++;
        pch[2] = trim(pch[2]);
        strtok(pch[2], ",");
    }

    for(int i = 3; pch[i-1]; i++) {
        pch[i-1] = trim(pch[i-1]);
        pch[i] = strtok(NULL, ",");
        rd->regn++;
    }

    /* ================================== */

    if(pch[0]) {
        if(pd->pass == 1) {
            strcpy(pd->labs[pd->labn], pch[0]);
            pd->labo[pd->labn] = pd->bt;
            pd->labn++;
        }

        if(!pch[1][0])
            return 0;
    }

    //printf("instr: %s\n", pch[1]);
    // get the instruction
    if(!strcmp(pch[1], "LD")) {
        *instr = &ld;
    } else if(!strcmp(pch[1], "JP")) {
        *instr = &jp;
    } else if(!strcmp(pch[1], "ADC")) {
        *instr = &adc;
    } else if(!strcmp(pch[1], "ADD")) {
        *instr = &add;
    } else if(!strcmp(pch[1], "AND")) {
        *instr = &and;
    } else if(!strcmp(pch[1], "CP")) {
        *instr = &cp;
    } else if(!strcmp(pch[1], "DEC")) {
        *instr = &dec;
    } else if(!strcmp(pch[1], "EX")) {
        *instr = &ex;
    } else if(!strcmp(pch[1], "IM")) {
        *instr = &im;
    } else if(!strcmp(pch[1], "IN")) {
        *instr = &in;
    } else if(!strcmp(pch[1], "INC")) {
        *instr = &inc;
    } else if(!strcmp(pch[1], "JR")) {
        *instr = &jr;
    } else if(!strcmp(pch[1], "OR")) {
        *instr = &or;
    } else if(!strcmp(pch[1], "OUT")) {
        *instr = &out;
    } else if(!strcmp(pch[1], "POP")) {
        *instr = &pop;
    } else if(!strcmp(pch[1], "PUSH")) {
        *instr = &push;
    } else if(!strcmp(pch[1], "RES")) {
        *instr = &res;
    } else if(!strcmp(pch[1], "RET")) {
        *instr = &ret;
    } else if(!strcmp(pch[1], "RL")) {
        *instr = &rl;
    } else if(!strcmp(pch[1], "RR")) {
        *instr = &rr;
    } else if(!strcmp(pch[1], "RRC")) {
        *instr = &rrc;
    } else if(!strcmp(pch[1], "RST")) {
        *instr = &rst;
    } else if(!strcmp(pch[1], "SBC")) {
        *instr = &sbc;
    } else if(!strcmp(pch[1], "SET")) {
        *instr = &set;
    } else if(!strcmp(pch[1], "SLA")) {
        *instr = &sla;
    } else if(!strcmp(pch[1], "SRA")) {
        *instr = &sra;
    } else if(!strcmp(pch[1], "SRL")) {
        *instr = &srl;
    } else if(!strcmp(pch[1], "SUB")) {
        *instr = &sub;
    } else if(!strcmp(pch[1], "XOR")) {
        *instr = &xor;
    } else if(!strcmp(pch[1], "BIT")) {
        *instr = &bit;
    } else if(!strcmp(pch[1], "CALL")) {
        *instr = &call;
    } else if(!strcmp(pch[1], "DB")) {
        *instr = &db;
    } else if(!strcmp(pch[1], "ORG")) {
        *instr = &org;
    } else {
        return -1;
    }

    rd->regs[0] = 0;
    rd->regs[1] = 0;
    rd->o = 0;

    // get the registers
    // TODO: needs to parse more accurately
    int j = 0;
    for(int i = 2; pch[i]; i++) {
        if(!strcmp(pch[i], "A")) {
            rd->regs[i-2] = A;
        } else if(!strcmp(pch[i], "B")) {
            rd->regs[i-2] = B;
        } else if(!strcmp(pch[i], "C")) {
            rd->regs[i-2] = C;
        } else if(!strcmp(pch[i], "D")) {
            rd->regs[i-2] = D;
        } else if(!strcmp(pch[i], "E")) {
            rd->regs[i-2] = E;
        } else if(!strcmp(pch[i], "H")) {
            rd->regs[i-2] = H;
        } else if(!strcmp(pch[i], "L")) {
            rd->regs[i-2] = L;
        } else if(!strcmp(pch[i], "I")) {
            rd->regs[i-2] = I;
        } else if(!strcmp(pch[i], "R")) {
            rd->regs[i-2] = R;
        } else if(!strcmp(pch[i], "BC")) {
            rd->regs[i-2] = BC;
        } else if(!strcmp(pch[i], "DE")) {
            rd->regs[i-2] = DE;
        } else if(!strcmp(pch[i], "HL")) {
            rd->regs[i-2] = HL;
        } else if(!strcmp(pch[i], "SP")) {
            rd->regs[i-2] = SP;
        } else if(!strcmp(pch[i], "IX")) {
            rd->regs[i-2] = IX|HL;
        } else if(!strcmp(pch[i], "IY")) {
            rd->regs[i-2] = IY|HL;
        } else if(!strcmp(pch[i], "IXH")) {
            rd->regs[i-2] = IX|H;
        } else if(!strcmp(pch[i], "IXL")) {
            rd->regs[i-2] = IX|L;
        } else if(!strcmp(pch[i], "IYH")) {
            rd->regs[i-2] = IY|H;
        } else if(!strcmp(pch[i], "IYL")) {
            rd->regs[i-2] = IY|L;
        } else if(!strcmp(pch[i], "(BC)")) {
            rd->regs[i-2] = BC|PTR;
        } else if(!strcmp(pch[i], "(DE)")) {
            rd->regs[i-2] = DE|PTR;
        } else if(!strcmp(pch[i], "(HL)")) {
            rd->regs[i-2] = HL|PTR;
        } else if(!strcmp(pch[i], "(IX)")) {
            rd->regs[i-2] = IX|HL|PTR;
        } else if(!strcmp(pch[i], "(IY)")) {
            rd->regs[i-2] = IY|HL|PTR;
        } else if(strstr(pch[i], "(IX+")) {
            rd->regs[i-2] = IX|HL|O|PTR;
            strtok(pch[i], "()");
            strtok(pch[i], "+");
            rd->o = atoi(strtok(NULL, "+"));
        } else if(strstr(pch[i], "(IY+")) {
            rd->regs[i-2] = IY|HL|O|PTR;
            strtok(pch[i], "()");
            strtok(pch[i], "+");
            rd->o = atoi(strtok(NULL, "+"));
        } else if(!strcmp(pch[i], "M")) {
            rd->regs[i-2] = M;
        } else if(!strcmp(pch[i], "NC")) {
            rd->regs[i-2] = NC;
        } else if(!strcmp(pch[i], "NZ")) {
            rd->regs[i-2] = NZ;
        } else if(!strcmp(pch[i], "P")) {
            rd->regs[i-2] = P;
        } else if(!strcmp(pch[i], "PE")) {
            rd->regs[i-2] = PE;
        } else if(!strcmp(pch[i], "PO")) {
            rd->regs[i-2] = PO;
        } else if(!strcmp(pch[i], "Z")) {
            rd->regs[i-2] = Z;
        } else if(!strcmp(pch[i], "(SP)")) {
            rd->regs[i-2] = SP|PTR;
        } else if(!strcmp(pch[i], "AF")) {
            rd->regs[i-2] = AF;
        } else if(!strcmp(pch[i], "AF'")) {
            rd->regs[i-2] = AF|AR;
        } else if(!strcmp(pch[i], "F")) {
            rd->regs[i-2] = F;
        } else if(!strcmp(pch[i], "(C)")) {
            rd->regs[i-2] = C|PTR;
        } else if(pch[i][0] == '(') {
            rd->regs[i-2] = N|PTR;
            rd->n[j] = atoi(strtok(pch[i], "()"));
            j++;
        } else if(is_num(pch[i])) {
            rd->regs[i-2] = N;
            rd->n[j] = atoi(pch[i]);
            j++;
        } else if(is_hex(pch[i])) {
            rd->regs[i-2] = N;
            rd->n[j] = htoi(pch[i]);
            j++;
        } else if(pch[i][0] == '\'') {
            rd->regs[i-2] = N;
            rd->n[j] = pch[i][1];
            j++;
        } else if(pch[i][0] == '\"') {
            rd->regs[i-2] = STR;
            size_t len = strlen(pch[i]);
            strncpy(rd->db, pch[i]+1, len-2);
        } else {
            rd->regs[i-2] = LAB;
            strcpy(rd->lab, pch[i]);
        }
    }

    return 1;
}

