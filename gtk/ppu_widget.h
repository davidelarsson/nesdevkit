#ifndef __PPU_WIDGET_H__
#define __PPU_WIDGET_H__

G_BEGIN_DECLS
#include <glib.h>
#include "../core/ppu.h"

/* GTK required */
#define PPU_WIDGET_TYPE (ppu_widget_get_type())
#define PPU_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), PPU_WIDGET_TYPE, PpuWidget))
#define PPU_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), PPU_WIDGET_TYPE, PpuWidgetClass))
#define IS_PPU_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), PPU_WIDGET_TYPE))
#define IS_PPU_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), PPU_WIDGET_TYPE))

typedef struct _PpuWidget PpuWidget;
typedef struct _PpuWidgetClass PpuWidgetClass;

#define PPU_WIDGET_ROWS		16
#define PPU_WIDGET_COLS		16
#define NES_RAM_OFFSET			0
#define NES_RAM_SIZE			0x2000
#define NES_ROM_OFFSET			0x8000
#define NES_ROM_SIZE			0x8000

struct _PpuWidget
{
	GtkNotebook	parent;
	gboolean	editable;
	GtkWidget	*raw_table;
	GtkWidget	*edit_table;
	ppu_t		*ppu;
	guint		offset;
	GtkWidget	*top_box;
	GtkWidget	*offset_spinner;
	GtkWidget	*entries[PPU_WIDGET_ROWS][PPU_WIDGET_COLS];
	GtkWidget	*top_locations[PPU_WIDGET_COLS];
	GtkWidget	*left_locations[PPU_WIDGET_ROWS];
	uint8_t		(* ppu_read_callback)(ppu_t *ppu, word address);
	void		(* ppu_write_callback)(ppu_t *ppu, word address, byte val);
	word		address;	/* Highlighted address pointer */
};

struct _PpuWidgetClass
{
	GtkNotebookClass parent_class;
	void (* ppu_widget) (PpuWidget *pw);
};

/**
 * Required by GTK
 */ 
GType ppu_widget_get_type();

/**
 * Create a new memory widget
 */
GtkWidget *ppu_widget_new();

/**
 * Updates the values in each entry.
 */
void ppu_widget_update(PpuWidget *pw);

G_END_DECLS
#endif /* __PPU_WIDGET_H__ */
