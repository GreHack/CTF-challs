#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "key.h"

char flag[46];
char *flk = flk_byte;

extern int chall(char *flag, char *key, int len);

int main(int argc, char *argv[]) {
	int ret, len;
	const long page = sysconf(_SC_PAGESIZE);
	void *start = (char *)chall - ((long)chall % page);

	ret = mprotect(start, page, PROT_EXEC | PROT_READ | PROT_WRITE);
	if (ret) {
		printf("Error: This challenge cannot run properly!\n");
		return -1;
	}

	len = fread(flag, 1, sizeof(flag), stdin);

	if (!len) {
		printf("Are your stupid or what ?\n");
		return -1;
	}

	ret = chall(flag, key, len);

	if (ret == 0) {
		printf("Success ! %s\n", flag);
	} else {
		printf("Try again.\n");
	}

	return 0;
}
