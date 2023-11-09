#ifndef _Z80_ASM_H_
#define _Z80_ASM_H_

#include <stdint.h>
#include "util.h"

/* ==================== */

#define A 8
#define B 1
#define C 2
#define D 3
#define E 4
#define H 5
#define L 6
#define F 7

#define I 9
#define R 10

#define AF 11
#define BC 12
#define DE 13
#define HL 14
#define SP 15

#define Z 16
#define P 17
#define M 18

#define NZ 19
#define PE 20
#define PO 21
#define NC 22

#define N  23
#define STR 24
#define LAB 25

#define IX 64
#define IY 128

#define PTR 32
#define O   (1 << 9)
#define AR  (1 << 10)

/* ==================== */

typedef struct {
    int pass;
    uint16_t org;
    int bt;
    int labn;
    char labs[10][255];
    uint16_t labo[10];
} prog_data_t;

typedef struct {
    uint8_t regn;
    int regs[10];
    uint16_t n[10];
    uint8_t o;
    char db[255];
    char lab[255];
} reg_data_t;

int org(prog_data_t *, reg_data_t *, uint8_t *);
int db(prog_data_t *, reg_data_t *, uint8_t *);
int call(prog_data_t *, reg_data_t *, uint8_t *);
int bit(prog_data_t *, reg_data_t *, uint8_t *);
int xor(prog_data_t *, reg_data_t *, uint8_t *);
int sub(prog_data_t *, reg_data_t *, uint8_t *);
int srl(prog_data_t *, reg_data_t *, uint8_t *);
int sra(prog_data_t *, reg_data_t *, uint8_t *);
int sla(prog_data_t *, reg_data_t *, uint8_t *);
int set(prog_data_t *, reg_data_t *, uint8_t *);
int sbc(prog_data_t *, reg_data_t *, uint8_t *);
int rst(prog_data_t *, reg_data_t *, uint8_t *);
int rrc(prog_data_t *, reg_data_t *, uint8_t *);
int rr(prog_data_t *, reg_data_t *, uint8_t *);
int rl(prog_data_t *, reg_data_t *, uint8_t *);
int ret(prog_data_t *, reg_data_t *, uint8_t *);
int res(prog_data_t *, reg_data_t *, uint8_t *);
int push(prog_data_t *, reg_data_t *, uint8_t *);
int pop(prog_data_t *, reg_data_t *, uint8_t *);
int out(prog_data_t *, reg_data_t *, uint8_t *);
int or(prog_data_t *, reg_data_t *, uint8_t *);
int jr(prog_data_t *, reg_data_t *, uint8_t *);
int inc(prog_data_t *, reg_data_t *, uint8_t *);
int in(prog_data_t *, reg_data_t *, uint8_t *);
int im(prog_data_t *, reg_data_t *, uint8_t *);
int ex(prog_data_t *, reg_data_t *, uint8_t *);
int dec(prog_data_t *, reg_data_t *, uint8_t *);
int cp(prog_data_t *, reg_data_t *, uint8_t *);
int and(prog_data_t *, reg_data_t *, uint8_t *);
int add(prog_data_t *, reg_data_t *, uint8_t *);
int adc(prog_data_t *, reg_data_t *, uint8_t *);
int jp(prog_data_t *, reg_data_t *, uint8_t *);
int ld(prog_data_t *, reg_data_t *, uint8_t *);

int parse(char *, reg_data_t *, prog_data_t *, int (**)(prog_data_t *, reg_data_t*, uint8_t *));

#endif
