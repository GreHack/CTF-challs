OFFSET=0x01e000

xordump: xordump.c chall.h chall.o
	gcc -o $@ $^

# utility tools
xor2: xor2.c
ddb: ddb.c
	gcc -o $@ $<

flag.bin: flag.png
	cp $< $@

func.o: func.S
	gcc -c -o $@ $<

func.bin: func.o
	(printf "#include <stdio.h>\n#include <stdint.h>\n" ; \
	r2 -qqc'pc 24' func.o ; printf "int main(void){fwrite(buffer, 1, _BUFFER_SIZE, stdout);return 0;}") |\
	gcc -xc - -o a.out
	./a.out > func.bin
	rm a.out

key.bin: xor2 flag.bin func.bin
	dd if=flag.bin skip=`printf '%d' $(OFFSET)` count=24 bs=1 | ./xor2 func.bin /dev/stdin > key.bin

chall.bin: xor2 key.bin flag.bin
	./xor2 key.bin flag.bin > chall.bin

chall.S: ddb chall.bin
	echo ".section        .chall,\"wax\",@progbits" > $@
	echo ".align 8" >> $@
	echo ".globl start" >> $@
	echo "start:" >> $@
	./ddb < chall.bin >> $@
	sed 's/#.*01e000:/.globl chall\n.type chall,@function\nchall:/' -i $@
	echo ".size start, .-start" >> $@

chall.h: key.bin
	xxd -i key.bin > $@

chall.o: chall.S

clean:
	rm -f xordump
	rm -f xor2 ddb
	rm -f chall.h chall.S *.o *.bin
