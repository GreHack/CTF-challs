.PHONY: all clean debug

TARGET=cafebabe

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard inc/*.h)
OBJECTS = $(patsubst src/%.c,obj/%.o,$(SOURCES))

CFLAGS= -I./inc
LDFLAGS=

all: obj $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

obj:
	mkdir -p obj

clean:
	rm -rf obj
	rm -f $(TARGET)

debug: CFLAGS += -DDEBUG -g
debug: all
