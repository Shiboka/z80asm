#ifndef _Z80_UTIL_H_
#define _Z80_UTIL_H_

#include <ctype.h>
#include <string.h>
#include <math.h>

void to_upper(char *);
int is_num(char *);
int is_hex(char *);
short htoi(char *);
char *trim(char *);

#endif
