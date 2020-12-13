hello: hello.s
	as -o hello.o hello.s
	ld -o hello -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc hello.o

.PHONY: clean
clean:
	rm -f *.o
	rm -f hello
	rm -f core
	rm -f vgcore*
