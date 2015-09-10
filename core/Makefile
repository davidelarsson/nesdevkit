CC = cc
CCFLAGS += -g -Wall -pedantic -Wextra -std=c99
LDFLAGS +=

OBJECTS = cpu_addressing.o cpu_instructions.o cpu_mem.o cpu.o ppu.o nes.o disasm.o asm.o

libnescore.a: $(OBJECTS)
	ar rcs libnescore.a $(OBJECTS)

cpu_addressing.o: cpu_addressing.c cpu_addressing.h
	$(CC) -c cpu_addressing.c $(CCFLAGS)

cpu_instructions.o: cpu_instructions.c cpu_instructions.h
	$(CC) -c cpu_instructions.c $(CCFLAGS)

cpu_mem.o: cpu_mem.c cpu_mem.h
	$(CC) -c cpu_mem.c $(CCFLAGS)

cpu.o: cpu.c cpu.h
	$(CC) -c cpu.c $(CCFLAGS)

ppu.o: ppu.c ppu.h
	$(CC) -c ppu.c $(CCFLAGS)

disasm.o: disasm.c disasm.h
	$(CC) -c disasm.c $(CCFLAGS)

asm.o: asm.c asm.h
	$(CC) -c asm.c $(CCFLAGS)

nes.o: nes.c nes.h
	$(CC) -c nes.c $(CCFLAGS)

clean:
	@rm -rf *.o libnescore.a

distclean:
