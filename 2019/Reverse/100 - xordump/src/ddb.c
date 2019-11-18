#include <stdio.h>

int main(int argc, char **argv) {
	unsigned char b[8];
	int n, i;
	int addr = 0;

	if (argc != 1) {
		printf("usage: ddb < infile > outfile\n");
	}

	while (!feof(stdin)) {
		n = fread(b, 1, sizeof(b), stdin);
		if (n == 0)
			break;

		printf("# %.8x:\n", addr);
		addr += n;
		printf("\t.byte 0x%.2x", b[0]);
		for (i = 1; i < n; i++)
			printf(", 0x%.2x", b[i]);
		puts("");
	}
	return 0;
}
