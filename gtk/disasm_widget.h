#ifndef DISASM_WIDGET_H
#define DISASM_WIDGET_H

#include <glib.h>
#include <gtk/gtk.h>
#include "../core/cpu_mem.h"
#include "../core/cpu.h"

/* Keep C++ programs that use this header file happy */
G_BEGIN_DECLS

/* Standard cast macros */
#define DISASM_WIDGET_TYPE            (disasm_widget_get_type())
#define DISASM_WIDGET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), DISASM_WIDGET_TYPE, DisasmWidget))
#define DISASM_WIDGET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), DISASM_WIDGET_TYPE, DisasmWidgetClass))
#define IS_DISASM_WIDGET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DISASM_WIDGET_TYPE))
#define IS_DISASM_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DISASM_WIDGET_TYPE))

/* Just some convenience defines for this widget */
#define DISASM_WIDGET_LINES 8

/* FIXME: WTF is this? */
typedef struct _DisasmWidget		DisasmWidget;
typedef struct _DisasmWidgetClass	DisasmWidgetClass;

/* Object structure, one new instance of this structure is created for each
   widget created of the DisasmWidget class */
struct _DisasmWidget
{
	GtkTable	parent;
	GtkWidget	*text_view;
	GtkWidget	*check_button;
	GtkWidget	*address_spinner;
	cpu_ram_t	*ram;
	cpu_rom_t	*rom;
	cpu_t		*cpu;
	word		address;
	uint8_t		(* ram_read_callback)(cpu_ram_t *ram, word address);
	uint8_t		(* rom_read_callback)(cpu_rom_t *rom, word address);
	word		(* read_pc_callback)(cpu_t *cpu);
	gboolean	auto_read_pc;
	guint		(* disasm_callback)(byte *bytes, word address, gchar *out);
};

/* Class structure, information here is common to all widgets of the class */
struct _DisasmWidgetClass
{
	GtkTextViewClass	parent_class;	/* Parent */
	/* Function pointer to... FIXME: what, exactly? */
	void 				(* disasm_widget)(DisasmWidget *ew);
};

/* Not mandatory, but very useful function */
GtkWidget	*disasm_widget_new();

void disasm_widget_update(DisasmWidget *daw);

/* Keep C++ programs that use this header file happy */
G_END_DECLS

#endif /* DISASM_WIDGET_H */
