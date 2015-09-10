CC = cc
CCFLAGS += -g -Wall -pedantic -Wextra -std=c99 -I../core `pkg-config --cflags gtk+-2.0`
LDFLAGS += `pkg-config --libs gtk+-2.0`
#OBJECTS = emulator.o gtk.o ../core/libnescore.a
OBJECTS = ppu_widget.o memory_widget.o stack_widget.o gtk.o cpu_widget.o disasm_widget.o execute_widget.o ../core/libnescore.a
OUTPUT = ../bin/nesdevkit-gtk

$(OUTPUT): $(OBJECTS)
	$(CC) -o $(OUTPUT) $(OBJECTS) $(LDFLAGS)

emulator.o: emulator.c emulator.h
	$(CC) -c emulator.c $(CCFLAGS)

ppu_widget.o: ppu_widget.c ppu_widget.h
	$(CC) -c ppu_widget.c $(CCFLAGS)

memory_widget.o: memory_widget.c memory_widget.h
	$(CC) -c memory_widget.c $(CCFLAGS)

stack_widget.o: stack_widget.c stack_widget.h
	$(CC) -c stack_widget.c $(CCFLAGS)

cpu_widget.o: cpu_widget.c cpu_widget.h
	$(CC) -c cpu_widget.c $(CCFLAGS)

execute_widget.o: execute_widget.c execute_widget.h
	$(CC) -c execute_widget.c $(CCFLAGS)

disasm_widget.o: disasm_widget.c disasm_widget.h
	$(CC) -c disasm_widget.c $(CCFLAGS)
gtk.o: gtk.c gtk.h
	@$(CC) -c gtk.c $(CCFLAGS)

clean:
	@rm -rf *.o $(OUTPUT)

distclean:

