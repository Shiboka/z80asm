#ifndef _Z80_ASM_H_
#define _Z80_ASM_H_

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
	unsigned short org;
	int bt;
	int labn;
	char labs[10][255];
	short labo[10];
} prog_data_t;

typedef struct {
	char regn;
	int regs[10];
	short n[10];
	char o;
	char db[255];
	char lab[255];
} reg_data_t;

char org(prog_data_t *, reg_data_t *, char *);
char db(prog_data_t *, reg_data_t *, char *);
char call(prog_data_t *, reg_data_t *, char *);
char bit(prog_data_t *, reg_data_t *, char *);
char xor(prog_data_t *, reg_data_t *, char *);
char sub(prog_data_t *, reg_data_t *, char *);
char srl(prog_data_t *, reg_data_t *, char *);
char sra(prog_data_t *, reg_data_t *, char *);
char sla(prog_data_t *, reg_data_t *, char *);
char set(prog_data_t *, reg_data_t *, char *);
char sbc(prog_data_t *, reg_data_t *, char *);
char rst(prog_data_t *, reg_data_t *, char *);
char rrc(prog_data_t *, reg_data_t *, char *);
char rr(prog_data_t *, reg_data_t *, char *);
char rl(prog_data_t *, reg_data_t *, char *);
char ret(prog_data_t *, reg_data_t *, char *);
char res(prog_data_t *, reg_data_t *, char *);
char push(prog_data_t *, reg_data_t *, char *);
char pop(prog_data_t *, reg_data_t *, char *);
char out(prog_data_t *, reg_data_t *, char *);
char or(prog_data_t *, reg_data_t *, char *);
char jr(prog_data_t *, reg_data_t *, char *);
char inc(prog_data_t *, reg_data_t *, char *);
char in(prog_data_t *, reg_data_t *, char *);
char im(prog_data_t *, reg_data_t *, char *);
char ex(prog_data_t *, reg_data_t *, char *);
char dec(prog_data_t *, reg_data_t *, char *);
char cp(prog_data_t *, reg_data_t *, char *);
char and(prog_data_t *, reg_data_t *, char *);
char add(prog_data_t *, reg_data_t *, char *);
char adc(prog_data_t *, reg_data_t *, char *);
char jp(prog_data_t *, reg_data_t *, char *);
char ld(prog_data_t *, reg_data_t *, char *);

int parse(char *, reg_data_t *, prog_data_t *, char (**)(prog_data_t *, reg_data_t*, char *));

#endif
