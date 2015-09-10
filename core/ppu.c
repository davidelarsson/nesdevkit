#include "defines.h"
#include "ppu.h"
#include <stdlib.h>

ppu_t *ppu_init()
{
	ppu_t *ppu = malloc(sizeof(ppu_t));
	ppu->ppu_ram = malloc(sizeof(byte) * PPU_RAM_SIZE);
	ppu->oam_ram = malloc(sizeof(byte) * OAM_RAM_SIZE);
	ppu->nmi_on_vblank = false;
	ppu->ppu_type = PPU_TYPE_MASTER;
	ppu->sprite_size = SPRITE_SIZE_SMALL;
	ppu->background_pattern_table_address = 0x0000;
	ppu->sprite_pattern_table_address = 0x0000;
	ppu->ppu_mem_address_increment = 1;
	ppu->scroll_base_address = 0x2000;
	ppu->background_visible = false;
	ppu->sprites_visible = false;
	ppu->in_vblank = false;
	ppu->sprite_0_hit = false;
	ppu->sprite_overflow = false;
	ppu->first_access = true;
	ppu->ppu_ram_address = 0x0000;
	ppu->oam_address = 0x00;

	return ppu;
}

void ppu_destroy(ppu_t *ppu)
{
	free(ppu->oam_ram);
	free(ppu);
}

byte ppu_mem_read(ppu_t *ppu, word address)
{
	address &= 0x3FFF;
	return ppu->ppu_ram[address];
}

void ppu_mem_write(ppu_t *ppu, word address, byte val)
{
	address &= 0x3FFF;
	ppu->ppu_ram[address] = val;
}

tile_t *ppu_get_tile(ppu_t *ppu, unsigned int index)
{
	int i;
	byte b1;
	
	/* Only tiles within the PPU memory are supported so far */
	if(index >= 512)
		return NULL;

	tile_t *tile = malloc(sizeof(tile_t));
	tile->pixels = calloc(8 * 8, sizeof(uint32_t));
	uint8_t *pixels = tile->pixels;

	/* Get bit 0 into BLUE byte */
	for(i = 0; i < 8; i++)
	{
		b1 = ppu_mem_read(ppu, index * 0x10 + i);
		pixels[4 * 8 * i + 2] = ((b1 >> 7) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 1 * 4 + 2] = ((b1 >> 6) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 2 * 4 + 2] = ((b1 >> 5) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 3 * 4 + 2] = ((b1 >> 4) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 4 * 4 + 2] = ((b1 >> 3) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 5 * 4 + 2] = ((b1 >> 2) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 6 * 4 + 2] = ((b1 >> 1) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 7 * 4 + 2] = (b1 & 0x01) ? 0xFF : 0x00;
	}

	/* Get bit 1 into GREEN byte */
	for(i = 0; i < 8; i++)
	{
		b1 = ppu_mem_read(ppu, index * 0x10 + i + 8);
		pixels[4 * 8 * i + 1] = ((b1 >> 7) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 1 * 4 + 1] = ((b1 >> 6) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 2 * 4 + 1] = ((b1 >> 5) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 3 * 4 + 1] = ((b1 >> 4) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 4 * 4 + 1] = ((b1 >> 3) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 5 * 4 + 1] = ((b1 >> 2) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 6 * 4 + 1] = ((b1 >> 1) & 0x01) ? 0xFF : 0x00;
		pixels[4 * 8 * i + 7 * 4 + 1] = (b1 & 0x01) ? 0xFF : 0x00;
	}

	/* Add 0xFF to alpha channel */
	for(i = 3; i < 8 * 8 * 4; i+= 4)
	{
		pixels[i] = 0xFF;
	}

	/* FIXME: Convert index to RGB according to PPU palette */

	return tile;
}

uint32_t tile_get_pixel(tile_t *tile, unsigned int index)
{
	if(index >= 64)
		return 0xFFFFFFFF;

	uint32_t pixel;
	uint8_t *p = (uint8_t *) &pixel;

	p[0] = tile->pixels[index * 4];
	p[1] = tile->pixels[index * 4 + 1];
	p[2] = tile->pixels[index * 4 + 2];
	p[3] = tile->pixels[index * 4 + 3];

	return pixel;
}

/*
ppu_tile_set_pixel(tile_t *tile, ...)
{
}
*/

void ppu_tile_destroy(tile_t *tile)
{
	free(tile->pixels);
	free(tile);
}

