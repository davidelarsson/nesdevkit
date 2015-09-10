#ifndef CPU_MEM_H
#define CPU_MEM_H

#include "defines.h"
/* 16,384 bytes per PRG bank */
#define PRG_BANK_SIZE 0x4000
/* 8,192 bytes per CHR bank */
#define CHR_BANK_SIZE 0x2000

/**
 * CPU RAM
 */
typedef struct cpu_ram_s {
	byte *bytes;
} cpu_ram_t;

/**
 * CPU ROM
 */
typedef struct cpu_rom_s {
	byte *bytes;
} cpu_rom_t;

/**
 * CPU RAM functions
 */
cpu_ram_t *cpu_ram_init();
byte cpu_ram_read(cpu_ram_t *ram, word address);
void cpu_ram_write(cpu_ram_t *ram, word address, byte val);

/**
 * CPU ROM functions
 */
cpu_rom_t *cpu_rom_init();
byte cpu_rom_read(cpu_rom_t *rom, word address);
void cpu_rom_write(cpu_rom_t *rom, word address, byte val);

/**
 * CPU memory functions
 * FIXME: Should these be in nes.h instead?
 */
//byte nes_cpu_mem_read(nes_t *nes, word address);
//void nes_cpu_mem_write(nes_t *nes, word address, byte val);

/**
 * Read a byte from current ROM bank
 * Note that the address is AND'ed with 0x7FFF, so you do not have to take into
 * consideration that ROM starts at 0x8000 in CPU memory space.
 */
//byte nes_prg_rom_read(nes_t *nes, word address);

/**
 * Write a byte to current ROM bank
 * Note that the address is AND'ed with 0x7FFF, so you do not have to take into
 * consideration that ROM starts at 0x8000 in CPU memory space.
 */
//void nes_prg_rom_write(nes_t *nes, word address, byte val);

#endif /* CPU_MEM_H */
