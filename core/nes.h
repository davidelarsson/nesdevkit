#ifndef NES_H
#define NES_H

#include <stdlib.h>
#include "cpu.h"
#include "ppu.h"
#include "cpu_mem.h"

typedef struct nes_s {
	cpu_ram_t	*cpu_ram;
	cpu_rom_t	*cpu_rom;
	cpu_t		*cpu;
	ppu_t		*ppu;
/*
	byte *cpu_ram;
*/
	byte *ppu_ram;
	byte *sprite_ram;
	byte *palette;
//	byte *prg_rom;
	byte *chr_rom;

	unsigned prg_banks;
	unsigned chr_banks;
	mirroring_t mirroring;
	bool sram;
	unsigned ram_banks;
	unsigned mapper;
	bool vs_system;
} nes_t;

unsigned nes_get_prg_banks(nes_t *nes);
unsigned nes_get_chr_banks(nes_t *nes);
mirroring_t nes_get_mirroring(nes_t *nes);
bool nes_get_sram_present(nes_t *nes);
bool nes_get_trainer_present(nes_t *nes);
int nes_get_mapper(nes_t *nes);

byte nes_cpu_mem_read(nes_t *nes, word address);
void nes_cpu_mem_write(nes_t *nes, word address, byte val);

/**
 * Instantiate a NES
 */
nes_t *nes_init(char* rom, size_t size);

/**
 * Parse a NES ROM
 */
int parse_rom(nes_t *nes, char* rom, size_t size);

/*
 * Free memory used by the NES.
 */
void nes_destroy(nes_t *nes);

/**
 * Get the CPU
 */
cpu_t *nes_get_cpu(nes_t *nes);

/**
 * Get the PPU
 */
ppu_t *nes_get_ppu(nes_t *nes);

/**
 * Get the CPU RAM
 */
cpu_ram_t *nes_get_cpu_ram(nes_t *nes);

/**
 * Get the CPU ROM
 */
cpu_rom_t *nes_get_cpu_rom(nes_t *nes);

/**
 * Execute one instruction
 */
void nes_execute(nes_t *nes);

/**
 * Step the emulation
 */
void nes_step(nes_t *nes);

#endif /* NES_H */
