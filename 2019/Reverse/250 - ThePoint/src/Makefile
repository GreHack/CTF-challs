#CC=g++
CC=clang++
CFLAGS=-std=c++2a -O3 -s
LDFLAGS=

ThePoint: thepoint.cc
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f ThePoint
