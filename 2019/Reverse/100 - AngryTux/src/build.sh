nasm angry_tux.asm -f elf64;
ld -s -N -o angry_tux angry_tux.o;
truncate angry_tux -s 636;
