CC=cc
CFLAGS=-Werror -Wall -Wextra -std=gnu89 $(OPT) -fPIC
LFLAGS=
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,obj/%.o,$(SRC))
INC=$(wildcard src/*.h)

.PHONY: release
release: OPT:=-Os -s -flto -fvisibility=hidden
release: kcc

.PHONY: static
static: OPT:=-Os -s -flto -fvisibility=hidden -static
static: CC:=musl-gcc
static: kcc

.PHONY: debug
debug: kcc
debug: OPT:=-O0 -ggdb3

.PHONY: sanitize
sanitize: kcc
sanitize: OPT:=-O0 -ggdb3 -fsanitize=address

.PHONY: warn-everything
warn-everything: kcc
warn-everything: CC=clang -Weverything -Werror -Wno-padded -Wno-switch-enum

kcc: $(OBJ) Makefile y.tab.o
	$(CC) -o kcc $(CFLAGS) $(OBJ) y.tab.o $(LFLAGS)

y.tab.c: src/grammar.y $(INC)
	yacc -o y.tab.c src/grammar.y

y.tab.o: y.tab.c
	$(CC) -c -o y.tab.o $(CFLAGS) y.tab.c \
		-Wno-unused-macros -Wno-missing-variable-declarations

obj/%.o: src/%.c $(INC) Makefile
	$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f obj/*
	rm -f kcc
	rm -f core
	rm -f src/arch.c
	rm -f y.tab.*
