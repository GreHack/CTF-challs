SRC = yaknet.c glad.c math.c mesh.c camera.c qoi.c
BIN = yaknet
OBJ = $(SRC:.c=.o)

INCS := `pkg-config --cflags sdl2`
LIBS := `pkg-config --libs sdl2`
CFLAGS += -Wall -Wextra -O2 -g
CFLAGS += $(INCS)
LDFLAGS += $(LIBS) -lm -ldl

all: server $(BIN)

%.qoi.c: %.qoi
	xxd -i $^ > $@

$(OBJ): Makefile

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)

server: server.o qoi.o
	$(CC) $^ -o $@ -lpthread -lm

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
	@$(CC) -MP -MM $< -MT $@ -MF $(call namesubst,%,.%.mk,$@) $(CFLAGS)

clean:
	rm -f server $(BIN) $(OBJ)

.PHONY: all clean

define namesubst
	$(foreach i,$3,$(subst $(notdir $i),$(patsubst $1,$2,$(notdir $i)), $i))
endef
-include $(shell find . -name ".*.mk")
