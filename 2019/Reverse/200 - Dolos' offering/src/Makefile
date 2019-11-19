CC=gcc
CFLAGS=-Wall -static
LDFLAGS=-lm
EXEC=dolos

all: $(EXEC)

dolos: dolos.c
	$(CC) -o $(EXEC) $(CFLAGS) dolos.c $(LDFLAGS) 
	objcopy --redefine-sym "memfrob"="memfrob_copy" dolos
	objcopy --redefine-sym "strchr"="memfrob" dolos
	objcopy --redefine-sym "memfrob_copy"="strchr" dolos
	objcopy --redefine-sym "__pow"="frexp" dolos
	objcopy --redefine-sym "sqrt"="sqrt_copy" dolos
	objcopy --redefine-sym "exp"="sqrt" dolos
	objcopy --redefine-sym "sqrt_copy"="exp" dolos


clean:
	rm -f core $(EXEC)
