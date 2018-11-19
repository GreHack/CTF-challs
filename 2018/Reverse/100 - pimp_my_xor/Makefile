LDFLAGS+=-no-pie -s
CFLAGS+=-O2

pimp_my_xor: pimp_my_xor.o key.h main.o

pimp_my_xor.o: pimp_my_xor.asm
	nasm -f elf64 $< -o $@

key.h: gen.py
	python gen.py > $@

solve: gen.py pimp_my_xor
	python gen.py solve | ./pimp_my_xor	

clean:
	rm -f *.o key.h pimp_my_xor
