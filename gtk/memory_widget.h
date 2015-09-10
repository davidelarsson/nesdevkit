#ifndef __MEMORY_WIDGET_H__
#define __MEMORY_WIDGET_H__

G_BEGIN_DECLS
#include <glib.h>
#include "../core/cpu_mem.h"

/* GTK required */
#define MEMORY_WIDGET_TYPE (memory_widget_get_type())
#define MEMORY_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MEMORY_WIDGET_TYPE, MemoryWidget))
#define MEMORY_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MEMORY_WIDGET_TYPE, MemoryWidgetClass))
#define IS_MEMORY_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MEMORY_WIDGET_TYPE))
#define IS_MEMORY_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MEMORY_WIDGET_TYPE))

typedef struct _MemoryWidget MemoryWidget;
typedef struct _MemoryWidgetClass MemoryWidgetClass;

#define MEMORY_WIDGET_ROWS		16
#define MEMORY_WIDGET_COLS		16
#define NES_RAM_OFFSET			0
#define NES_RAM_SIZE			0x2000
#define NES_ROM_OFFSET			0x8000
#define NES_ROM_SIZE			0x8000

struct _MemoryWidget
{
	GtkTable  table;												/* Parent widget */
	gboolean editable;
	guint     offset;												/* Active offset */
	GtkWidget *top_box;												/* Container for offset spinner and addresses */
	GtkWidget *offset_spinner;										/* Offset spinner */
	GtkWidget *entries[MEMORY_WIDGET_ROWS][MEMORY_WIDGET_COLS]; 	/* bytes */
	GtkWidget *top_locations[MEMORY_WIDGET_COLS]; 					/* addresses on top */
	GtkWidget *left_locations[MEMORY_WIDGET_ROWS]; 					/* Addresses to the left */
	cpu_ram_t *ram;													/* RAM of the NES' CPU */
	cpu_rom_t *rom;													/* ROM of the NES' CPU */
	uint8_t (* ram_read_callback)(cpu_ram_t *ram, word address);	/* Callback to read CPU RAM */
	uint8_t (* rom_read_callback)(cpu_rom_t *rom, word address);	/* Callback to read CPU ROM */
	void (* ram_write_callback)(cpu_ram_t *ram, word address, byte val); /* Callback to read CPU RAM */
	void (* rom_write_callback)(cpu_rom_t *rom, word address, byte val); /* Callback to read CPU ROM */
	gint (* nes_asm_callback)(char *to_asm, word pc, byte *out);
	word      pc;													/* Highlighted instruction pointer */
	word      ap;													/* Highlighted instruction's memory address */
};

struct _MemoryWidgetClass
{
	GtkTableClass parent_class;
	void (* memory_widget) (MemoryWidget *mw);
};

/**
 * Required by GTK
 */ 
GType memory_widget_get_type();

/**
 * Create a new memory widget
 */
GtkWidget *memory_widget_new();

/**
 * Updates the values in each entry.
 */
void memory_widget_update(MemoryWidget *mw);

G_END_DECLS
#endif /* __MEMORY_WIDGET_H__ */
