#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
    seteuid(0);
    setuid(0);
    system("cat /flag.txt");
    return 0;
}