#ifndef PPU_H
#define PPU_H

#include <stdbool.h>
#include <inttypes.h>
#include "defines.h"

typedef struct tile_s {
	uint8_t *pixels;
} tile_t;

typedef enum {
	PPU_TYPE_MASTER,
	PPU_TYPE_SLAVE
} ppu_type_t;

typedef enum {
	SPRITE_SIZE_SMALL,
	SPRITE_SIZE_LARGE
} sprite_size_t;

typedef struct ppu_s {
	byte			*ppu_ram;
	byte			*oam_ram;
	bool			nmi_on_vblank;
	ppu_type_t		ppu_type;
	sprite_size_t	sprite_size;
	word			background_pattern_table_address;
	word			sprite_pattern_table_address;
	unsigned int	ppu_mem_address_increment;
	unsigned int	scroll_base_address;
	bool			background_visible;
	bool			sprites_visible;
	bool			in_vblank;
	bool			sprite_0_hit;
	bool			sprite_overflow;
	word			ppu_ram_address;
	byte			ppu_ram_latch;
	byte			oam_address;
	bool			first_access;
	byte			scroll_x;
	byte			scroll_y;
} ppu_t;

/** Initialize the CPU */
ppu_t *ppu_init();

/** Destroy the CPU */
void ppu_destroy(ppu_t *ppu);

/* Read a byte from PPU memory */
byte ppu_mem_read(ppu_t *ppu, word address);

/* Write a byte to PPU memory */
void ppu_mem_write(ppu_t *ppu, word address, byte val);

/* Get a tile from the pattern table of the PPU */
tile_t *ppu_get_tile(ppu_t *ppu, unsigned int index);

/* Free memory used by a tile */
void ppu_tile_destroy(tile_t *tile);

/*  */
uint32_t tile_get_pixel(tile_t *tile, unsigned int index);


#endif /* PPU_H */
