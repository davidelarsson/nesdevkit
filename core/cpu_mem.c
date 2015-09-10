#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
#include "cpu_mem.h"

cpu_ram_t *cpu_ram_init()
{
	cpu_ram_t *cpu_ram;
	cpu_ram = malloc(sizeof(cpu_ram_t));
	cpu_ram->bytes = malloc(sizeof(byte) * CPU_RAM_SIZE);
	return cpu_ram;
}

byte cpu_ram_read(cpu_ram_t *ram, word address)
{
	if(address >= 0x0800)
		printf("cpu_ram_read(): invalid address!");
	address &= 0x07FF;
	return ram->bytes[address];
}

void cpu_ram_write(cpu_ram_t *ram, word address, byte val)
{
	if(address >= 0x0800)
		printf("cpu_ram_write(): invalid address!");
	address &= 0x07FF;
	ram->bytes[address] = val;
}

cpu_rom_t *cpu_rom_init()
{
	cpu_rom_t *cpu_rom;
	cpu_rom = malloc(sizeof(cpu_rom_t));
	cpu_rom->bytes = malloc(sizeof(byte) * 0x8000);
	return cpu_rom;
}

byte cpu_rom_read(cpu_rom_t *rom, word address)
{
	if(address >= 0x8000)
		printf("cpu_rom_read(): invalid address!");
	address &= 0x7FFF;
	return rom->bytes[address];
}

void cpu_rom_write(cpu_rom_t *rom, word address, byte val)
{
	if(address >= 0x8000)
		printf("cpu_rom_write(): invalid address!");
	address &= 0x7FFF;
	rom->bytes[address] = val;
}

