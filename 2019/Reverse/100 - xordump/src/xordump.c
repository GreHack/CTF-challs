#include "chall.h"

extern char start;
extern int chall(char *p, char *k, unsigned int len) __attribute__ ((section (".chall")));

#ifdef TEST
#include <stdio.h>
int solve(char *p, char *k, unsigned int len) {
	int i = 0;
	for (;i < 454284;p++) {
		*p ^= k[i++ % len];
		putchar(*p);
	}
}
#define chall(a,b,c) solve(a,b,c)
#endif

int main(void) {
	return chall(&start, key_bin, key_bin_len);
}
