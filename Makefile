CC=cc
CFLAGS=-Werror -Wall -Wextra -std=gnu89 $(OPT) -fPIC
LFLAGS=
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,obj/%.o,$(SRC))
INC=$(wildcard src/*.h)
EXTERN_INC=$(wildcard include/*.h)

.PHONY: release
release: kcc
release: OPT:=-Ofast -march=haswell -mtune=haswell -flto

.PHONY: native
native: kcc
native: OPT:=-Ofast -march=native -mtune=native -flto

.PHONY: debug
debug: kcc
debug: OPT:=-O0 -ggdb3

kcc: $(OBJ)
	$(CC) -o kcc $(CFLAGS) $(OBJ) $(LFLAGS)

obj/%.o: src/%.c $(INC) $(EXTERN_INC)
	$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f obj/*
	rm -f kcc
	rm -f core
