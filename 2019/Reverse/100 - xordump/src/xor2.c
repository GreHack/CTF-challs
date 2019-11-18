#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

void
die(const char *errstr, ...)
{
	va_list ap;

	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(1);
}

int main(int argc, char **argv)
{
	FILE *f, *k;
	unsigned int len = 0;
	char *key = NULL;
	char *chk;
	int n, i = 0;

	if (argc < 3)
		printf("usage: xor2 [keyfile] [file]\n");

	k = fopen(argv[1], "r");
	if (!k)
		die("fopen: '%s' %s\n", argv[1], strerror(errno));

	f = fopen(argv[2], "r");
	if (!f)
		die("fopen: '%s' %s\n", argv[2], strerror(errno));

	while (!feof(k)) {
		key = realloc(key, len + 1024);
		len += fread(key + len, 1, 1024, k);
	}
	fclose(k);

	chk = malloc(len);
	while (!feof(f)) {
		n = fread(chk, 1, len, f);
		for (i = 0; i < n; i++)
			chk[i] ^= key[i];
		fwrite(chk, 1, n, stdout);
	}
	free(chk);
	free(key);
	fclose(f);
	fflush(stdout);
	return 0;
}
