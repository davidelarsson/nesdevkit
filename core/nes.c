#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "nes.h"
#include "cpu_mem.h"
#include "cpu_addressing.h"
#include "cpu_instructions.h"
#include "defines.h"
#include "cpu.h"
#include "ppu.h"

nes_t *nes_init(char* rom, size_t size)
{
	nes_t *nes = malloc(sizeof(nes_t));
	nes->cpu = cpu_init();
	nes->ppu = ppu_init();
	nes->cpu_ram = cpu_ram_init();
	nes->cpu_rom = cpu_rom_init();
	nes->ppu_ram = malloc(sizeof(byte) * PPU_RAM_SIZE);
	nes->sprite_ram = malloc(sizeof(byte) * SPRITE_RAM_SIZE);
	nes->palette = malloc(sizeof(byte) * SPRITE_RAM_SIZE);

	if(0 == parse_rom(nes, rom, size))
		return nes;
	else
	{
		nes_destroy(nes);
		return NULL;
	}
}

void nes_destroy(nes_t *nes)
{
	cpu_destroy(nes->cpu);
	ppu_destroy(nes->ppu);
	free(nes->cpu_ram);
	free(nes->ppu_ram);
	free(nes->palette);
	/*
	if(nes->prg_rom)
		free(nes->prg_rom);
	if(nes->chr_rom)
		free(nes->chr_rom);
	*/
	free(nes);
}

cpu_ram_t *nes_get_cpu_ram(nes_t *nes)
{
	return nes->cpu_ram;
}

cpu_rom_t *nes_get_cpu_rom(nes_t *nes)
{
	return nes->cpu_rom;
}

int parse_rom(nes_t *nes, char* rom, size_t size)
{
	const unsigned header_size = 16;
	const unsigned trainer_size = 256;
	bool has_trainer;

	/* The smallest NES ROM there are, have 16 KB of PRG + 8 KB of CHR ROM  + header */
	if(size < header_size + PRG_BANK_SIZE + CHR_BANK_SIZE)
		return -1;

	/* First four bytes have to be "NES", followed by MS-DOS end-of-file */
	if(!(rom[0] == 'N' && rom [1] == 'E' && rom[2] == 'S' && rom[3] == 0x1A))
	{
		return -1;
	}

	nes->prg_banks = rom[4];
	nes->chr_banks = rom[5];
	byte flag = rom[6];

	if(flag & 0x08)
		nes->mirroring = FOUR_SCREEN;
	else if(flag & 0x01)
		nes->mirroring = VERTICAL_MIRRORING;
	else
		nes->mirroring = HORIZONTAL_MIRRORING;

	if(flag & 0x02)
		nes->sram = true;
	else
		nes->sram = false;

	if(flag & 0x04)
		has_trainer = true;
	else
		has_trainer = false;

	nes->mapper = flag >> 4;

	flag = rom[7];
	
	if(flag & 0x01)
		nes->vs_system = true;
	else
		nes->vs_system = false;

	/* FIXME: Add more checks for PlayChoice-10, et.c. */
	nes->mapper |= (flag & 0xF0);

	/* No mapper support is implemented! */
	if(nes->mapper != 0)
		return -1;

	unsigned prg_rom_size = nes->prg_banks * PRG_BANK_SIZE;
	unsigned chr_rom_size = nes->chr_banks * CHR_BANK_SIZE;

	/* Sanity check: make sure the rom array is large enough */
	if(size < prg_rom_size + chr_rom_size + header_size + has_trainer ? trainer_size : 0)
		return -1;

	unsigned offset = header_size;

	/* If a trainer exists, ignore it. It's always 256 bytes */
	if(has_trainer)
		offset += trainer_size;

	unsigned counter = 0;
	while(counter != prg_rom_size)
	{
		cpu_rom_write(nes_get_cpu_rom(nes), counter, *(rom + offset + counter));
		counter ++;
	}

	printf("Copied %d bytes of PRG ROM.\n", prg_rom_size);

	offset += prg_rom_size;

	printf("offset is now: %d\n", offset);

/*
	nes->chr_rom = malloc(sizeof(byte) * chr_rom_size);
	memcpy(nes->chr_rom, rom + offset, chr_rom_size);
*/
	counter = 0;

	while(counter != chr_rom_size)
	{
		ppu_mem_write(nes_get_ppu(nes), counter, *(rom + offset + counter));
		counter++;
	}

	printf("Copied %d bytes of CHR ROM.\n", chr_rom_size);

	/* All's fine, return the parsed file */
	return 0;
}

unsigned nes_get_prg_banks(nes_t *nes)
{
	return nes->prg_banks;
}

unsigned nes_get_chr_banks(nes_t *nes)
{
	return nes->chr_banks;
}

mirroring_t nes_get_mirroring(nes_t *nes)
{
	return nes->mirroring;
}

bool nes_get_sram_present(nes_t *nes)
{
	return nes->sram;
}

int nes_get_mapper(nes_t *nes)
{
	return nes->mapper;
}

cpu_t *nes_get_cpu(nes_t *nes)
{
	return nes->cpu;
}

ppu_t *nes_get_ppu(nes_t *nes)
{
	return nes->ppu;
}

byte nes_cpu_mem_read(nes_t *nes, word address)
{
    cpu_ram_t *ram = nes_get_cpu_ram(nes);
    cpu_rom_t *rom = nes_get_cpu_rom(nes);
	ppu_t *ppu = nes_get_ppu(nes);
	(void) ppu;

    /* RAM is 0x0000 - 0x07FF, but mirrored four times at
              0x0800 - 0x0FFF,
              0x1000 - 0x17FF and
              0x1800 - 0x1FFF
              FIXME: Is this correct?
    */
    if(address < CPU_RAM_SIZE * 4)
        return cpu_ram_read(ram, address & 0x07FF);

    /* I/O Area */
    if(address < 0x8000)
	{
		/* PPU Status register */
		if(address == 0x2002)
		{
			if(ppu->first_access)
				ppu->first_access = false;
			else
				ppu->first_access = true;

			byte status_byte = 0;

			/* Byte 7 signals that we are in VBLank */
			if(ppu->in_vblank)
				status_byte |= 0x80;
			else
				status_byte &= ~0x80;

			/* Byte 6 singals a sprite #0 hit */
			if(ppu->sprite_0_hit)
				status_byte |= 0x40;
			else
				status_byte &= ~0x40;

			/* Byte 5 singals a sprite overflow */
			if(ppu->sprite_overflow)
				status_byte |= 0x20;
			else
				status_byte &= ~0x20;

			return status_byte;
		}
    //    return 0;

		/* OAM data register */
		if(address == 0x2004)
		{
			return ppu->oam_ram[ppu->oam_address];
		}

		/* PPU RAM data register */
		if(address == 0x2007)
		{
			word address = ppu->ppu_ram_address;
			ppu->ppu_ram_address += ppu->ppu_mem_address_increment;
			return ppu->ppu_ram[address];
		}

	} /* I/O area */

    /* address is 0x8000 or above, which is ROM area */
    return cpu_rom_read(rom, address & 0x7FFF);
}
    
void nes_cpu_mem_write(nes_t *nes, word address, byte val)
{   
    cpu_ram_t *ram = nes_get_cpu_ram(nes);
    cpu_rom_t *rom = nes_get_cpu_rom(nes);
	ppu_t *ppu = nes_get_ppu(nes);

	(void) rom;

   /**
 	* RAM is 0x0000 - 0x07FF, but mirrored four times at
    *        0x0800 - 0x0FFF,
    *        0x1000 - 0x17FF and
    *        0x1800 - 0x1FFF
    *        FIXME: Is this correct?
    */
    if(address < CPU_RAM_SIZE * 4)
	{
		cpu_ram_write(ram, address, val);
		return;
	}

    /* Between 0x2000 and 0x7FFF is the I/O area */
	if(address < 0x8000)
	{
		/* PPUCTRL register at 0x2000 */
		if(address == 0x2000)
		{
			if(val & 0x80)
				ppu->nmi_on_vblank = true;
			else
				ppu->nmi_on_vblank = false;

			if(val & 0x40)
				ppu->ppu_type = PPU_TYPE_MASTER;
			else
				ppu->ppu_type = PPU_TYPE_SLAVE;

			if(val & 0x20)
			{
				printf("FIXME: LARGE SPRITES ARE UNIMPLEMENTED!\n");
				ppu->sprite_size = SPRITE_SIZE_LARGE;
			}
			else
				ppu->sprite_size = SPRITE_SIZE_SMALL;

			if(val & 0x10)
				ppu->background_pattern_table_address = 0x1000;
			else
				ppu->background_pattern_table_address = 0x0000;

			if(val & 0x08)
				ppu->sprite_pattern_table_address = 0x1000;
			else
				ppu->sprite_pattern_table_address = 0x0000;

			if(val & 0x04)
				ppu->ppu_mem_address_increment = 32;
			else
				ppu->ppu_mem_address_increment = 1;

			/* Bit 0 and 1 determine the scroll base address */
			switch(val & 0x03)
			{
				case 3:
					ppu->scroll_base_address = 0x2C00;
					break;
				case 2:
					ppu->scroll_base_address = 0x2800;
					break;
				case 1:
					ppu->scroll_base_address = 0x2400;
					break;
				case 0:
					ppu->scroll_base_address = 0x2000;
					break;
			}

			return;
		} /* PPUCTRL register at 0x2000 */

		/* PPUMASK register at 0x2001 */
		if(address == 0x2001)
		{
			if(val & 0x08)
				ppu->background_visible = true;
			else
				ppu->background_visible = false;

			if(val & 0x10)
				ppu->sprites_visible = true;
			else
				ppu->sprites_visible = false;

			/* FIXME: There are more bits to check here! */
			return;
		} /* PPUMASK register at 0x2001 */

		/* OAM RAM address at 0x2002 */
		if(address == 0x2003)
		{
			ppu->oam_address = val;
			return;
		}

		/* OAM data register */
		if(address == 0x2004)
		{
			ppu->oam_ram[ppu->oam_address] = val;
			/* Only writes cause an increment of the OAM address pointer */
			ppu->oam_address += 1;
		}

		/* Scroll registers */
		if(address == 0x2005)
		{
			if(ppu->first_access)
			{
				ppu->first_access = false;
				ppu->scroll_x = val;
			}
			else
			{
				ppu->first_access = true;
				ppu->scroll_y = val;
			}
		}

		/* PPU RAM address register */
		if(address == 0x2006)
		{
			if(ppu->first_access)
			{
				ppu->first_access = false;
				ppu->ppu_ram_latch = val;
			}
			else
			{
				ppu->first_access = true;
				ppu->ppu_ram_address = ((ppu->ppu_ram_latch & 0x3F) << 8) | val;
			}
		}

		/* PPU RAM data register */
		if(address == 0x2007)
		{
			word address = ppu->ppu_ram_address;
			ppu->ppu_ram_address += ppu->ppu_mem_address_increment;
			ppu->ppu_ram[address] = val;
		}

	} /* End of I/O area */

    /* 0x8000 and above is ROM area. A write here usually
       means that we have a mapper. This is unimplemented! */
    return;
}


void nes_execute(nes_t *nes)
{
	/* Get opcode */
	word pc = cpu_get_pc(nes_get_cpu(nes));
	byte opcode = nes_cpu_mem_read(nes, pc);

//	printf("Executing instruction: 0x%02X at 0x%04X\n", opcode, pc);

	/* Get the address in memory according to the addressing mode of the opcode */
	word address = cpu_address_modes[opcode](nes);

	/* Execute opcode */
	cpu_opcodes[opcode](nes, address);

	/* Add clock cycles */
	cpu_add_cycles(nes_get_cpu(nes), cpu_cycles[opcode]);

	/* If opcode length is set to zero in the table, let the instruction itself handle PC */
	word length = cpu_instruction_length[opcode];
	if(length)
		cpu_set_pc(nes_get_cpu(nes), pc + length);
}

void nes_step(nes_t *nes)
{
	(void) nes;
}
