# Project global settingsHEXT
EXE     := guessing
CC      := gcc
CFLAGS  := -Wall -Werror -Wuninitialized -Wunused -m32
LDFLAGS := -m32 --static
LIBS    := 
CEXT 	:= c
HEXT 	:= h

# Project directory structure
BIN_DIR := ./
OBJ_DIR := build
SRC_DIR := src

# Construct OBJ_DIR output and dependency filenames
SRCS := $(shell find $(SRC_DIR) -name *.$(CEXT))
$(HEXT)S := $(shell find $(SRC_DIR) -name *.$(HEXT))
OBJS := $(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(addsuffix .o,$(basename $(SRCS))))

.PHONY: info clean mrproper all test

# Main task
all: $(BIN_DIR)/$(EXE)

# Build target
$(BIN_DIR)/$(EXE): $(OBJS) $($(HEXT)S) Makefile
	@echo "[*] Project compilation ..."
	@mkdir -p $(dir $@)
	@$(CC) -s $(OBJS) $(LDFLAGS) $(LIBS) -o $@
	@echo "Done"

# Compile all $(CEXT) files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(CEXT) Makefile
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC_FLAGS) -c -o $@ $<

test: $(BIN_DIR)/$(EXE)
	@python test.py $(BIN_DIR)/$(EXE)

# clean project
clean:
	@echo "[*] Clean project"
	@rm -rf $(OBJ_DIR)

# remove executable
mrproper: clean
	@echo "[*] Delete all generated files"
	@rm -rf $(BIN_DIR)/$(EXE)

# Get informations about MAKEFILE configuration
info:
	@echo "[*] Executable name: ${EXE}"
	@echo "[*] Application dir: ${BIN_DIR}"
	@echo "[*] Object dir:      ${OBJ_DIR}"
	@echo "[*] Sources dir:     ${SRC_DIR}"
	@echo "[*] Sources:         ${SRCS}"
	@echo "[*] Headers:         ${$(HEXT)S}"
	@echo "[*] Objects:         ${OBJS}"
	@echo "[*] CFLAGS:          ${CFLAGS}"
	@echo "[*] LDFLAGS:         ${LDFLAGS}"
	@echo "[*] LIBS:            ${LIBS}"