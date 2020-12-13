CC=cc
CFLAGS=-Werror -Wall -Wextra -std=gnu89 $(OPT) -fPIC
LFLAGS=
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,obj/%.o,$(SRC))
INC=$(wildcard src/*.h)
EXTERN_INC=$(wildcard include/*.h)

.PHONY: release
release: kcc
release: OPT:=-O2

.PHONY: debug
debug: kcc
debug: OPT:=-O0 -ggdb3

.PHONY: warn-everything
warn-everything: kcc
warn-everything: CC=clang -Weverything -Werror

kcc: $(OBJ) Makefile
	$(CC) -o kcc $(CFLAGS) $(OBJ) $(LFLAGS)

obj/%.o: src/%.c $(INC) $(EXTERN_INC) Makefile
	$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f obj/*
	rm -f kcc
	rm -f core
