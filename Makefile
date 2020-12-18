CC=cc
CFLAGS=-Werror -Wall -Wextra -std=gnu89 $(OPT) -fPIC
LFLAGS=
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,obj/%.o,$(SRC))
INC=$(wildcard src/*.h)
EXTERN_INC=$(wildcard include/*.h)

.PHONY: release
release: kcc
release: OPT:=-Os -s -flto -fvisibility=hidden

.PHONY: debug
debug: kcc
debug: OPT:=-O0 -ggdb3

.PHONY: sanitize
sanitize: kcc
sanitize: OPT:=-O0 -ggdb3 -fsanitize=address

.PHONY: warn-everything
warn-everything: kcc
warn-everything: CC=clang -Weverything -Werror -Wno-padded -Wno-switch-enum

kcc: $(OBJ) Makefile
	$(CC) -o kcc $(CFLAGS) $(OBJ) $(LFLAGS)

obj/%.o: src/%.c $(INC) $(EXTERN_INC) Makefile
	$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f obj/*
	rm -f kcc
	rm -f core
	rm -f src/arch.c
