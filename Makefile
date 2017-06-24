#
# argopts
#
# file - Makefile
# author - Harlan J. Waldrop <waldroha@oregonstate.edu>
#

MAKEFLAGS += --no-builtin-rules

PROJECT = argopts

#
# Code Section
#
CC = gcc
CP = objcopy
DP = objdump

CDEFS_LIST = DEBUG 
CDEFS      = $(addprefix -D, $(CDEFS_LIST))

override CFLAGS  += -Wall -Werror $(CDEFS) -g -I./src -Wno-unused -std=c99
#override LDFLAGS += -Wl,-Map,$(PROJECT).map

SRC = $(shell find ./src -name "*.c")
OBJ = $(addsuffix .o, $(SRC))

BIN = $(PROJECT)

#default: $(BIN)
#	./$(BIN) --verbose --test --help

debug: $(BIN)
	valgrind --leak-check=full ./$(BIN) --tbool 0 1 0 --tint 123

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^

%.o: %
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJ) *.zip

deep-clean: clean
	rm -f *.dump

zip:
	zip $(PROJECT).zip *.c *.def *.h *.inc Makefile

.PHONY: all clean debug deep-clean default dump zip
