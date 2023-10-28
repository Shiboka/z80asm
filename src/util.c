#include "util.h"

void
to_upper(char *str)
{
    for(int i = 0; str[i]; i++)
        str[i] = toupper(str[i]);
}

int
is_num(char *str)
{
	if(!str)
		return 0;

	for(int i = 0; str[i]; i++) {
		switch(str[i]) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			default:
				return 0;
		}
	}

	return 1;
}

int
is_hex(char *str)
{
	size_t len;

	if(!str)
		return 0;

	len = strlen(str);

	if(str[len-1] != 'H')
		return 0;

	str[len-1] = 0;

	if(!is_num) {
		str[len-1] = 'H';
		return 0;
	}

	str[len-1] = 'H';
	return 1;
}

short
htoi(char *h)
{
	size_t len = strlen(h);
	int d, p, n = 0;

	if(len < 3)
		return -1;

	for(int i = len-2; i != -1; i--) {
		p = len - i - 2;

		if((d = h[i] - 0x30) > 9)
			if((d = h[i] - 0x37) > 16)
				return -1;

		n += d * pow(16, p);
	}

	return n;
}

char *
trim(char *s)
{
	int i = 0;

	for(; s[i] == ' ' || s[i] == '	'; i++)
		s[i] = 0;

	return s+i;
}
