#include <stdlib.h>
#include <gtk/gtk.h>
/*
#include <gtk/gtksignal.h>
#include <gtk/gtknotebook.h>
#include <gtk/gtk.h>
*/
#include <gdk/gdkkeysyms.h>
#include <glib/gprintf.h>
#include "ppu_widget.h"

/**
 * Properties
 */
enum {
	PROP_0,
	PROP_EDITABLE,
	PROP_PPU,
	PROP_PPU_READ_CALLBACK,
	PROP_PPU_WRITE_CALLBACK,
	N_PROPERTIES
};

/**
 * Signal constants
 */
/*
enum {
    SIGNAL_UPDATE_GUI,
    SIGNAL_DISABLE_GUI,
    SIGNAL_LAST
};

static guint ppu_widget_signals[SIGNAL_LAST] = { 0 };
*/
/**
 * Handles property settings
 */
static void ppu_widget_set_property(GObject *obj,
                            guint prop_id,
                            const GValue *val,
                            GParamSpec *pspec);

/*
 * Create a new memory widget
 */
GtkWidget *ppu_widget_new();

/**
 * Initializing the class
 */
static void ppu_widget_class_init(PpuWidgetClass *klass);

/**
 * Instantiating an object
 */
static void ppu_widget_init(PpuWidget *pw);

/**
 * Handles the input of entries, makes sure only hex digits are allowed. Also,
 * if tab or shift-tab is pressed, the entry is updated. This is not default
 * behavior for a GtkEntry.
 */
static gboolean entry_key_handler(GtkEntry *entry, GdkEvent *ev, PpuWidget *pw);

/**
 * Called whenever enter, tab or shift-tab is pressed in an entry.
 */
static void entry_activate_handler(GtkEntry *entry, PpuWidget *pw);

/**
 * Called whenever the mouse is scrolled on top of an entry. If so, the offset
 * is adjusted accordingly.
 */
static void table_scroll_handler(GtkEntry *entry, GdkEvent *ev, PpuWidget *pw);

/**
 * Handles the input of the offset spinner. Only hex digits are allowed.
 */
static gboolean spinner_key_handler(GtkEntry *entry, GdkEvent *ev, PpuWidget *pw);

/**
 * Handles hex-to-decimal conversion of the spin button.
 */
static gboolean spinner_input_handler(GtkSpinButton *spinner, gdouble *new_value, gpointer dummy);

/**
 * Handles value-to-hexadecimal conversion of the spin button.
 */
static gboolean spinner_output_handler(GtkSpinButton *spinner, gpointer dummy);

/**
 * Updates the offset.
 */
static void update_offset(PpuWidget *pw, guint value);

/**
 * Whenever the value of the spin button is changed, this function is called.
 * It just calles update_offset() above.
 */
static void spinner_value_changed_handler(GtkSpinButton *spinner, PpuWidget *pw);

/**
 * Scrolls the memory offset up
 */
static void scroll_up(PpuWidget *pw);

/**
 * Scrolls the memory offset down
 */
static void scroll_down(PpuWidget *pw);

void init_raw_page(PpuWidget *pw);
void init_tile_editor_page(PpuWidget *pw);
void ppu_widget_update_raw_page(PpuWidget *pw);
void ppw_widget_update_edit_page(PpuWidget *pw);

GType ppu_widget_get_type(void)
{
	static GType pw_type = 0;
	if(!pw_type)
	{
		const GTypeInfo pw_info = {
			sizeof (PpuWidgetClass),
			NULL, /* base_init */
			NULL, /* base_finalize */
			(GClassInitFunc) ppu_widget_class_init, /* (GClassInitFunc) ppu_widget_class_init,*/
			NULL, /* class_finalize */
			NULL, /* class_data */
			sizeof(PpuWidget),
			0,
			(GInstanceInitFunc) ppu_widget_init,
			NULL /*FIXME: What is this? */
		};

		pw_type = g_type_register_static(GTK_TYPE_NOTEBOOK, "PpuWidget", &pw_info, 0);

	}
	return pw_type;
}

void ppu_widget_class_init(PpuWidgetClass *pw)
{
	GObjectClass *c = G_OBJECT_CLASS(pw);

	c->set_property = ppu_widget_set_property;
/*
    ppu_widget_signals[SIGNAL_UPDATE_GUI] = g_signal_new("update_gui",
                                                    G_TYPE_FROM_CLASS(c),
                                                    G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
                                                    G_STRUCT_OFFSET(PpuWidgetClass, ppu_widget),
                                                    NULL,
                                                    NULL,
                                                    g_cclosure_marshal_VOID__VOID,
                                                    G_TYPE_NONE,
                                                    0);
*/
	g_object_class_install_property(c,
					PROP_EDITABLE,
					g_param_spec_boolean("editable",
							"Editable",
							"Memory entries can be edited",
							TRUE,
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_PPU,
					g_param_spec_pointer("ppu",
							"PPU",
							"Pointer to the PPU",
							G_PARAM_WRITABLE));
			
	g_object_class_install_property(c,
					PROP_PPU_READ_CALLBACK,
					g_param_spec_pointer("ppu_read_callback",
							"PPU Read Callback",
							"Function that reads from PPU memory",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_PPU_WRITE_CALLBACK,
					g_param_spec_pointer("ppu_write_callback",
							"PPU Write Callback",
							"Function that writes to PPU memory",
							G_PARAM_WRITABLE));

}

void ppu_widget_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	PpuWidget *pw = PPU_WIDGET(obj);
	(void) pspec;

	switch(prop_id)
	{
		case PROP_EDITABLE:
			pw->editable = g_value_get_boolean(val);
			ppu_widget_update(pw);
			break;
		case PROP_PPU:
			pw->ppu = (ppu_t *) g_value_get_pointer(val);
			break;
		case PROP_PPU_READ_CALLBACK:
			pw->ppu_read_callback = ((byte (*)(ppu_t *ppu, word address)) g_value_get_pointer(val));
			break;
		case PROP_PPU_WRITE_CALLBACK:
			pw->ppu_write_callback = ((void (*)(ppu_t *ppu, word address, byte val)) g_value_get_pointer(val));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}

}

GtkWidget *ppu_widget_new()
{
	return GTK_WIDGET(g_object_new(ppu_widget_get_type(), NULL));
}

static void ppu_widget_init(PpuWidget *pw)
{
	pw->editable = TRUE;
	pw->offset = 0x0000;
	pw->address = 0x0000;
	pw->ppu = NULL;
	pw->ppu_read_callback = NULL;
	pw->ppu_read_callback = NULL;

	GtkWidget *label = NULL;

	init_raw_page(pw);
	init_tile_editor_page(pw);

	/* Setup the notebook */
	gtk_notebook_append_page(GTK_NOTEBOOK(pw), GTK_WIDGET(pw->raw_table), gtk_label_new_with_mnemonic("raw"));
	gtk_notebook_append_page(GTK_NOTEBOOK(pw), GTK_WIDGET(pw->edit_table), gtk_label_new_with_mnemonic("Tile editor"));

	label = gtk_label_new_with_mnemonic("Here will be something else");
	gtk_notebook_append_page(GTK_NOTEBOOK(pw), GTK_WIDGET(label), gtk_label_new_with_mnemonic("Something else"));
	ppu_widget_update(pw);
}

void init_tile_editor_page(PpuWidget *pw)
{
	GtkWidget *table;

	table = gtk_table_new(16, 16, FALSE);
	pw->edit_table = table;
}

void ppu_widget_update_edit_page(PpuWidget *pw)
{
	GdkPixbuf *buf;
	GtkWidget *table;
	GtkWidget *image;
	tile_t *tile;
	GtkAttachOptions attach_options;
	attach_options = GTK_SHRINK;

	if(!pw->ppu)
		return;

	g_print("Updating shit...\n");

	table = pw->edit_table;
	int size = 4*8*8*1000;
	char data[size];
	for(int i = 0; i < size; i++)
		data[i] = 0x55;

	for(int i = 0; i < 32; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			tile = ppu_get_tile(pw->ppu, i * 16 + j);
			buf = gdk_pixbuf_new_from_data(tile->pixels, GDK_COLORSPACE_RGB, TRUE, 8, 8, 8, 8*4, NULL, NULL);
			image = gtk_image_new_from_pixbuf(buf);
			gtk_table_attach(GTK_TABLE(table), image, j, j + 1, i, i + 1, attach_options, attach_options, 0, 0);
		}
	}

	gtk_widget_show_all(table);


}

void init_raw_page(PpuWidget *pw)
{
	GtkWidget *table = NULL;
	GtkWidget *entry = NULL;
	gchar *tip = NULL;
	gint i,j;
	GtkAttachOptions attach_options;

	/* Setup the table */
	table = gtk_table_new(17, 17, FALSE);
	attach_options = GTK_SHRINK;
	g_signal_connect(table, "scroll-event", G_CALLBACK(table_scroll_handler), pw);

	/* The box at the top that contains the spinner and bookmarks */
	pw->top_box = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), pw->top_box, 1, 17, 0, 1);
	gtk_widget_show(pw->top_box);

	/* Offset spin button */
	pw->offset_spinner = gtk_spin_button_new_with_range(0, 0x3F00, 0x10);
	gtk_widget_modify_font(GTK_WIDGET(pw->offset_spinner), pango_font_description_from_string("monospace"));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(pw->offset_spinner), pw->offset);
	gtk_entry_set_width_chars(GTK_ENTRY(pw->offset_spinner), 4);
	gtk_entry_set_max_length(GTK_ENTRY(pw->offset_spinner), 4);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(pw->offset_spinner), FALSE); /* Allow hex input */
	g_signal_connect(GTK_ENTRY(pw->offset_spinner), "key-press-event", G_CALLBACK(spinner_key_handler), pw);
	g_signal_connect(GTK_ENTRY(pw->offset_spinner), "value-changed", G_CALLBACK(spinner_value_changed_handler), pw);
	g_signal_connect(GTK_ENTRY(pw->offset_spinner), "input", G_CALLBACK(spinner_input_handler), NULL);
	g_signal_connect(GTK_ENTRY(pw->offset_spinner), "output", G_CALLBACK(spinner_output_handler), NULL);
	gtk_table_attach(GTK_TABLE(table), pw->offset_spinner, 0, 1, 1, 2, attach_options, attach_options, 3, 3);
	gtk_widget_show(pw->offset_spinner);

	/* Labels at the top */
	for(i = 0; i < PPU_WIDGET_ROWS; i++)
	{
		char str[100];
		sprintf(str, "%X", i);
		pw->top_locations[i] = gtk_label_new(str);
		gtk_widget_modify_font(GTK_WIDGET(pw->top_locations[i]), pango_font_description_from_string("monospace"));
		gtk_table_attach(GTK_TABLE(table), pw->top_locations[i], i + 1, i + 2, 1, 2, attach_options, attach_options, 3, 3);
		gtk_widget_show(pw->top_locations[i]);
	}

	/* Labels to the left */
	for(i = 0; i < PPU_WIDGET_ROWS; i++)
	{
		char str[100];
		sprintf(str, "0x%X", pw->offset + i * 0x10);
		pw->left_locations[i] = gtk_label_new(str);
		gtk_widget_modify_font(GTK_WIDGET(pw->left_locations[i]), pango_font_description_from_string("monospace"));
		gtk_table_attach(GTK_TABLE(table), pw->left_locations[i], 0, 1, i + 2, i + 3, attach_options, attach_options, 3, 3);
		gtk_widget_show(pw->top_locations[i]);
	}

	/* The memory entries */
	for(i = 0; i < PPU_WIDGET_ROWS; i++)
	{
		for(j = 0; j < PPU_WIDGET_COLS; j++)
		{
			entry = gtk_entry_new();
			gtk_widget_modify_font(GTK_WIDGET(entry), pango_font_description_from_string("monospace"));
			pw->entries[i][j] = entry;
			gtk_entry_set_max_length(GTK_ENTRY(entry), 2);
			gtk_entry_set_width_chars(GTK_ENTRY(entry), 2);
			g_signal_connect(GTK_ENTRY(entry), "key-press-event", G_CALLBACK(entry_key_handler), pw);
			g_signal_connect(GTK_ENTRY(entry), "activate", G_CALLBACK(entry_activate_handler), pw);
			tip = g_strdup_printf("0x%04X", pw->offset + i * 0x10 + j);
			gtk_widget_set_tooltip_text(entry, tip);
			g_free(tip);
			
			gtk_table_attach_defaults(GTK_TABLE(table), entry, j + 1, j + 2, i + 2, i + 3);

			gtk_widget_show(entry);
		}
	}
	pw->raw_table = table;
}

void table_scroll_handler(GtkEntry *entry, GdkEvent *ev, PpuWidget *pw)
{
	(void) entry;
	(void) pw;

	GdkEventScroll *e = (GdkEventScroll *) ev;
	ppu_widget_update(pw);

	if(e->direction == GDK_SCROLL_DOWN)
		scroll_up(pw);

	if(e->direction == GDK_SCROLL_UP)
		scroll_down(pw);

	update_offset(pw, pw->offset);
}

void scroll_up(PpuWidget *pw)
{
	if(pw->offset <= 0x0000)
		return;
	pw->offset -= 0x10;
	update_offset(pw, pw->offset);
}

void scroll_down(PpuWidget *pw)
{
	if(pw->offset >= 0x3F00)
		return;
	pw->offset += 0x10;
	update_offset(pw, pw->offset);
}

gboolean entry_key_handler(GtkEntry *entry, GdkEvent *ev, PpuWidget *pw)
{
	guint i, j;
	guint row;

	GdkEventKey *key = (GdkEventKey *) ev;
	guint keyval = key->keyval;

	GtkWidget *top = gtk_widget_get_parent(GTK_WIDGET(entry));
	guint pos = gtk_editable_get_position(GTK_EDITABLE(entry));
	guint len = gtk_entry_get_text_length(entry);

	/* Find out the position of our entry */
	for(i = 0; i < PPU_WIDGET_ROWS; i++)
		for(j = 0; j < PPU_WIDGET_COLS; j++)
		{
			if(GTK_ENTRY(entry) == GTK_ENTRY(pw->entries[i][j]))
			{
				row = i;
			}
		}

	switch(keyval)
	{
		/* If the cursor is in the left-most position in the entry and the
		 * user presses Left, shift focus to the widget to the left.
		 */
		case GDK_KEY_Left:
			if(len == 0)
				ppu_widget_update(pw);

			if(pos == 0)
			{
				gtk_widget_child_focus(top, GTK_DIR_LEFT);
				entry_activate_handler(entry, pw);
//	    		g_signal_emit_by_name(pw, "update_gui");
				return FALSE;
			} else
				return FALSE;
			break;

		/* If the cursor is in the right-most position in the entry and the
		 * user presses Right, shift focus to the widget to the right.
		 */
		case GDK_KEY_Right:

			if(pos == len)
			{
				if(len == 0)
					ppu_widget_update(pw);
				gtk_widget_child_focus(top, GTK_DIR_RIGHT);
				entry_activate_handler(entry, pw);
				ppu_widget_update(pw);
//	    		g_signal_emit_by_name(pw, "update_gui");
				return FALSE;
			} else
				return FALSE;

		case GDK_KEY_Page_Up:
			scroll_up(pw);
 //   		g_signal_emit_by_name(pw, "update_gui");
			return FALSE;

		case GDK_KEY_Page_Down:
			scroll_down(pw);
//    		g_signal_emit_by_name(pw, "update_gui");
			return FALSE;

		/* Some allowed keys */
		case GDK_KEY_Return:
		case GDK_KEY_BackSpace:
		case GDK_KEY_Delete:
			return FALSE;

		/* Consider these keys as if the entry was updated */
		case GDK_KEY_Up:
		case GDK_KEY_Down:
		case GDK_KEY_Tab:
		case GDK_KEY_ISO_Left_Tab:
			if(len == 0)
				ppu_widget_update(pw);
			entry_activate_handler(entry, pw);
			ppu_widget_update(pw);
//			g_signal_emit_by_name(pw, "update_gui");
			return FALSE;

		/* Restore entry's value (yeah, yeah, I'm lazy, the other ones' too) */
		case GDK_KEY_Escape:
			ppu_widget_update(pw);
			return FALSE;
	}

	/* Key must be hexadecimal digit */
	if(!g_ascii_isxdigit(keyval))
		return TRUE;

	/* We should really never end up here */
	return FALSE;
}

void entry_activate_handler(GtkEntry *entry, PpuWidget *pw)
{
	guint i, j;
	guint row, col;
	guint address = 0;
	byte val;

	for(i = 0; i < PPU_WIDGET_ROWS; i++)
		for(j = 0; j < PPU_WIDGET_COLS; j++)
		{
			if(GTK_ENTRY(entry) == GTK_ENTRY(pw->entries[i][j]))
			{
				row = i;
				col = j;
			}
		}

	address = pw->offset + row * 0x10 + col;
	val = g_ascii_strtoll(gtk_entry_get_text(entry), NULL, 16);
	
	if(pw->ppu_write_callback)
		pw->ppu_write_callback(pw->ppu, address - NES_RAM_OFFSET, val);
}

gboolean spinner_key_handler(GtkEntry *spinner, GdkEvent *ev, PpuWidget *pw)
{
	(void) spinner;
	GdkEventKey *key = (GdkEventKey *) ev;
	guint keyval = key->keyval;

	switch(keyval)
	{
		/* Some allowed keys */
		case GDK_KEY_Up:
		case GDK_KEY_Down:
		case GDK_KEY_Left:
		case GDK_KEY_Right:
		case GDK_KEY_Return:
		case GDK_KEY_BackSpace:
		case GDK_KEY_Delete:
		case GDK_KEY_Tab:
		case GDK_KEY_ISO_Left_Tab:
			return FALSE;
	}

	if(keyval == GDK_KEY_Escape)
	{
		/* Restore value of spinner */
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinner), pw->offset);
		return FALSE;
	}

	/* Key must be hexadecimal digit */
	if(!g_ascii_isxdigit(keyval))
	{
		return TRUE;
	}
	return FALSE;
}

void spinner_value_changed_handler(GtkSpinButton *spinner, PpuWidget *pw)
{
	(void) spinner;
	update_offset(pw, gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pw->offset_spinner)));
}

gboolean spinner_input_handler(GtkSpinButton *spinner, gdouble *new_value, gpointer dummy)
{
	(void) dummy;

	gchar *text;
	text = g_strdup_printf("%s", gtk_entry_get_text(GTK_ENTRY(spinner)));
	*new_value = strtol(text, NULL, 16);
	g_free(text);
	return TRUE;
}

gboolean spinner_output_handler(GtkSpinButton *spinner, gpointer dummy)
{
	(void) dummy;

	GtkAdjustment *adj;
	gchar *text;
	int value;
	adj = gtk_spin_button_get_adjustment(spinner);
	value = (int) gtk_adjustment_get_value(adj);
	text = g_strdup_printf("%04X", value);
	gtk_entry_set_text(GTK_ENTRY(spinner), text);
	g_free(text);
	return TRUE;
}

void ppu_widget_update(PpuWidget *pw)
{
	ppu_widget_update_raw_page(pw);
	ppu_widget_update_edit_page(pw);
}

void ppu_widget_update_raw_page(PpuWidget *pw)
{
	guint i, j;
	char *text;
	char *tip;
	uint16_t address;

	/* Uh-huh. Static variables. I'm so retro! */
	static word previous_address_x = 0;
	static word previous_address_y = 0;

	/* Labels to the left */
	for(i = 0; i < PPU_WIDGET_ROWS; i++)
	{
		text = g_strdup_printf("0x%04X", pw->offset + i * 0x10);
		gtk_label_set_text(GTK_LABEL(pw->left_locations[i]), text);
		g_free(text);
	}

	/* Restore normal color to the entries that
	   previously contained highlighted pointers */
	gtk_widget_modify_text(GTK_WIDGET(pw->entries[previous_address_y][previous_address_x]), GTK_STATE_NORMAL, NULL);
	gtk_widget_modify_text(GTK_WIDGET(pw->entries[previous_address_y][previous_address_x]), GTK_STATE_INSENSITIVE, NULL);

	/* Update the entries */
	for(i = 0; i < PPU_WIDGET_ROWS; i++)
	{
		for(j = 0; j < PPU_WIDGET_COLS; j++)
		{
			address = pw->offset + i * 0x10 + j;
			guint val;
			GtkEntry *entry = GTK_ENTRY(pw->entries[i][j]);

			if(!pw->ppu_read_callback || !pw->ppu) {
				text = g_strdup_printf("Er");
				gtk_widget_set_sensitive(GTK_WIDGET(entry), FALSE);
			} else {
				val = pw->ppu_read_callback(pw->ppu, address);
				text = g_strdup_printf("%02X", val);
				if(pw->editable)
					gtk_widget_set_sensitive(GTK_WIDGET(entry), TRUE);
				else
					gtk_widget_set_sensitive(GTK_WIDGET(entry), FALSE);
			}
			/* FIXME: W-T-F?! Why does the third row by itself cause a bus error?! */
			/* Wait, does this work as intended now? */
//			GtkEntryBuffer *b = gtk_entry_get_buffer(entry);
//			gtk_entry_buffer_set_text(b, text, 2);
			gtk_entry_set_text(entry, text);
			g_free(text);

			if(pw->address == address) {
				GdkColor blue = { 0, 0, 0, 0xFFFF };
				gtk_widget_modify_text(GTK_WIDGET(entry), GTK_STATE_NORMAL, &blue);
				gtk_widget_modify_text(GTK_WIDGET(entry), GTK_STATE_INSENSITIVE, &blue);
				previous_address_x = j;
				previous_address_y = i;
			}
			tip = g_strdup_printf("0x%04X", pw->offset + i * 0x10 + j);
			gtk_widget_set_tooltip_text(GTK_WIDGET(entry), tip);
			g_free(tip);
		}
	}
}

void update_offset(PpuWidget *pw, guint value)
{
	pw->offset = (0xFFF0 & value);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(pw->offset_spinner), pw->offset);
	ppu_widget_update(pw);
}

