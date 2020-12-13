AS=as
LD=ld
LDFLAGS=-dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc

hello: hello.o again.o
	$(LD) -o hello $(LDFLAGS) hello.o again.o

hello.o: hello.s
	$(AS) -o hello.o hello.s

again.o: again.s
	$(AS) -o again.o again.s

.PHONY: clean
clean:
	rm -f *.o
	rm -f hello
	rm -f core
	rm -f vgcore*
