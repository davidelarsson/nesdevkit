#include <stdlib.h>
#include <gtk/gtksignal.h>
#include <gtk/gtktable.h>
#include <gtk/gtkentry.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

#include "stack_widget.h"
#include "../core/cpu_mem.h"

/**
 * Properties
 */
enum {
	PROP_0,
	PROP_RAM,
	PROP_CPU,
	PROP_RAM_READ_CALLBACK,
	PROP_READ_SP_CALLBACK,
	N_PROPERTIES
};

/**
 * Handles property settings
 */
static void stack_widget_set_property(GObject *obj,
                            guint prop_id,
                            const GValue *val,
                            GParamSpec *pspec);

/*
 * Create a new memory widget
 */
GtkWidget *stack_widget_new();

/**
 * Initializing the class
 */
static void stack_widget_class_init(StackWidgetClass *klass);

/**
 * Instantiating an object
 */
static void stack_widget_init(StackWidget *sw);

/**
 * Called whenever the mouse is scrolled on top of an entry. If so, the offset
 * is adjusted accordingly.
 */
static void table_scroll_handler(GtkTable *table, GdkEvent *ev, StackWidget *sw);

/**
 * Scrolls the memory offset up
 */
static void scroll_up(StackWidget *sw);

/**
 * Scrolls the memory offset down
 */
static void scroll_down(StackWidget *sw);

/**
 * Acually update the widget.
 */
void actual_update(StackWidget *sw);

GType stack_widget_get_type(void)
{
	static GType sw_type = 0;
	if(!sw_type)
	{
		const GTypeInfo sw_info = {
			sizeof (StackWidgetClass),
			NULL, /* base_init */
			NULL, /* base_finalize */
			(GClassInitFunc) stack_widget_class_init, /* (GClassInitFunc) stack_widget_class_init,*/
			NULL, /* class_finalize */
			NULL, /* class_data */
			sizeof(StackWidget),
			0,
			(GInstanceInitFunc) stack_widget_init,
			NULL /*FIXME: What is this? */
		};

		sw_type = g_type_register_static(GTK_TYPE_TABLE, "StackWidget", &sw_info, 0);

	}
	return sw_type;
}

void stack_widget_class_init(StackWidgetClass *sw)
{
	GObjectClass *c = G_OBJECT_CLASS(sw);

	c->set_property = stack_widget_set_property;

	g_object_class_install_property(c,
					PROP_CPU,
					g_param_spec_pointer("cpu",
							"CPU",
							"Pointer to NES CPU",
							G_PARAM_WRITABLE));
			
	g_object_class_install_property(c,
					PROP_RAM,
					g_param_spec_pointer("ram",
							"RAM",
							"Pointer to NES' RAM",
							G_PARAM_WRITABLE));
			
	g_object_class_install_property(c,
					PROP_RAM_READ_CALLBACK,
					g_param_spec_pointer("ram_read_callback",
							"RAM Read Callback",
							"Function that reads from NES RAM",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_READ_SP_CALLBACK,
					g_param_spec_pointer("read_sp_callback",
							"SP read callback",
							"Callback to read the stack pointer",
							G_PARAM_WRITABLE));

}

void stack_widget_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	StackWidget *sw = STACK_WIDGET(obj);
	(void) pspec;

	switch(prop_id)
	{
		case PROP_CPU:
			sw->cpu = (cpu_t *) g_value_get_pointer(val);
			break;
		case PROP_RAM:
			sw->ram = (cpu_ram_t *) g_value_get_pointer(val);
			break;
		case PROP_RAM_READ_CALLBACK:
			sw->ram_read_callback = ((byte (*)(cpu_ram_t *ram, word address)) g_value_get_pointer(val));
			break;
		case PROP_READ_SP_CALLBACK:
			sw->read_sp_callback= ((byte (*)(cpu_t *)) g_value_get_pointer(val));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}

}

GtkWidget *stack_widget_new()
{
	return GTK_WIDGET(g_object_new(stack_widget_get_type(), NULL));
}

static void stack_widget_init(StackWidget *sw)
{
	sw->ram_read_callback = NULL;
	sw->read_sp_callback = NULL;
	sw->cpu = NULL;
	GtkWidget *entry = NULL;
	gint i;
	GtkAttachOptions attach_options;
	gtk_table_set_homogeneous(GTK_TABLE(sw), FALSE);

	g_signal_connect(sw, "scroll-event", G_CALLBACK(table_scroll_handler), sw);
	attach_options = GTK_SHRINK;

	/* Labels to the left */
	for(i = 0; i < STACK_WIDGET_ROWS; i++)
	{
		char *str;
		str = g_strdup_printf("0x%04X", 0x100 + i);
		sw->left_locations[i] = gtk_label_new(str);
		gtk_widget_modify_font(GTK_WIDGET(sw->left_locations[i]), pango_font_description_from_string("monospace"));
		g_free(str);
		gtk_table_attach(GTK_TABLE(sw), sw->left_locations[i], 0, 1, i, i + 1, attach_options, attach_options, 3, 0);
		gtk_widget_show(sw->left_locations[i]);
	}

	/* The memory entries */
	for(i = 0; i < STACK_WIDGET_ROWS; i++)
	{
		entry = gtk_entry_new();
		sw->entries[i] = entry;
		gtk_widget_modify_font(GTK_WIDGET(entry), pango_font_description_from_string("monospace"));
		gtk_entry_set_max_length(GTK_ENTRY(entry), 2);
		gtk_entry_set_width_chars(GTK_ENTRY(entry), 2);
		gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
		gtk_table_attach(GTK_TABLE(sw), entry, 1, 2, i, i + 1, attach_options, attach_options, 3, 0);
		gtk_widget_show(entry);
	}
	stack_widget_update(sw);
}

void table_scroll_handler(GtkTable *table, GdkEvent *ev, StackWidget *sw)
{
    (void) table;

    GdkEventScroll *e = (GdkEventScroll *) ev;

    if(e->direction == GDK_SCROLL_UP)
        scroll_up(sw);

    if(e->direction == GDK_SCROLL_DOWN)
        scroll_down(sw);
}

void scroll_up(StackWidget *sw)
{
	/* 
	 * Yeah, this might be unclear, but the stack pointer of the CPU is of
	 * course not updated, only the internal one of this widget
	 */
	if(sw->sp >= 0x100 - (STACK_WIDGET_ROWS / 2))
		sw->sp = 0xFF - (STACK_WIDGET_ROWS / 2);
	else if(sw->sp < STACK_WIDGET_ROWS / 2)
		sw->sp = STACK_WIDGET_ROWS / 2;
	else
		sw->sp += 1;
	actual_update(sw);
}

void scroll_down(StackWidget *sw)
{
	/* See scroll_up() */
	if(sw->sp <= (STACK_WIDGET_ROWS / 2))
		sw->sp = STACK_WIDGET_ROWS / 2 - 1;
	else if(sw->sp >= 0x100 - (STACK_WIDGET_ROWS / 2))
		sw->sp = 0xFF -( STACK_WIDGET_ROWS / 2);
	else
		sw->sp -= 1;
	actual_update(sw);
}

void stack_widget_update(StackWidget *sw)
{
	/* We need all of it in order to update the widget! */
	if(!sw->read_sp_callback || !sw->ram_read_callback || !sw->cpu || !sw->ram)
		return;
	
	sw->sp = sw->read_sp_callback(sw->cpu);
	actual_update(sw);
}

void actual_update(StackWidget *sw)
{
	guint i;
	char *text = NULL;
	uint16_t address, top_address;
	
	/* top_address is the top address to show in the widget */
	if(sw->sp > 0xFF - STACK_WIDGET_ROWS / 2)
		top_address = 0x1FF;
	else if(sw->sp < STACK_WIDGET_ROWS / 2)
		top_address = 0xFF + STACK_WIDGET_ROWS;
	else
		top_address = 0x100 + sw->sp + STACK_WIDGET_ROWS / 2;

	for(i = 0; i < STACK_WIDGET_ROWS; i++)
	{
		text = g_strdup_printf("0x%04X", top_address - i);
		gtk_label_set_text(GTK_LABEL(sw->left_locations[i]), text);
		g_free(text);
	}

	/* Update the entries */
	for(i = 0; i < STACK_WIDGET_ROWS; i++)
	{
		address = top_address - i;
		guint val;
		GtkEntry *entry = GTK_ENTRY(sw->entries[i]);

		if(!sw->ram_read_callback) {
			text = g_strdup_printf("Er");
		} else {
			val = sw->ram_read_callback(sw->ram, address);
			text = g_strdup_printf("%02X", val);
		}
		gtk_entry_set_text(entry, text);
		g_free(text);
	}
}

