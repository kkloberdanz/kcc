# set architecture specific code to the arch of the current machine
ln -sf arch/arch.`uname -m`.c src/arch.c
