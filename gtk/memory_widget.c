#include <stdlib.h>
#include <gtk/gtksignal.h>
#include <gtk/gtktable.h>
#include <gtk/gtkentry.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

#include "memory_widget.h"
#include "../core/cpu_mem.h"

/**
 * Properties
 */
enum {
	PROP_0,
	PROP_EDITABLE,
	PROP_RAM,
	PROP_ROM,
	PROP_ASSEMBLY_CALLBACK,
	PROP_RAM_READ_CALLBACK,
	PROP_ROM_READ_CALLBACK,
	PROP_RAM_WRITE_CALLBACK,
	PROP_ROM_WRITE_CALLBACK,
	PROP_PC,
	PROP_AP,
	N_PROPERTIES
};

/**
 * Signal constants
 */
enum {
    SIGNAL_UPDATE_GUI,
    SIGNAL_DISABLE_GUI,
    SIGNAL_LAST
};

static guint memory_widget_signals[SIGNAL_LAST] = { 0 };

/**
 * Handles property settings
 */
static void memory_widget_set_property(GObject *obj,
                            guint prop_id,
                            const GValue *val,
                            GParamSpec *pspec);

/*
 * Create a new memory widget
 */
GtkWidget *memory_widget_new();

/**
 * Initializing the class
 */
static void memory_widget_class_init(MemoryWidgetClass *klass);

/**
 * Instantiating an object
 */
static void memory_widget_init(MemoryWidget *mw);

/**
 * Handles the input of entries, makes sure only hex digits are allowed. Also,
 * if tab or shift-tab is pressed, the entry is updated. This is not default
 * behavior for a GtkEntry.
 */
static gboolean entry_key_handler(GtkEntry *entry, GdkEvent *ev, MemoryWidget *mw);

/**
 * Called whenever enter, tab or shift-tab is pressed in an entry.
 */
static void entry_activate_handler(GtkEntry *entry, MemoryWidget *mw);

/**
 * Called whenever the mouse is scrolled on top of an entry. If so, the offset
 * is adjusted accordingly.
 */
static void table_scroll_handler(GtkEntry *entry, GdkEvent *ev, MemoryWidget *mw);

/**
 * Handles the input of the offset spinner. Only hex digits are allowed.
 */
static gboolean spinner_key_handler(GtkEntry *entry, GdkEvent *ev, MemoryWidget *mw);

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
static void update_offset(MemoryWidget *mw, guint value);

/**
 * Whenever the value of the spin button is changed, this function is called.
 * It just calles update_offset() above.
 */
static void spinner_value_changed_handler(GtkSpinButton *spinner, MemoryWidget *mw);

/**
 * Scrolls the memory offset up
 */
static void scroll_up(MemoryWidget *mw);

/**
 * Scrolls the memory offset down
 */
static void scroll_down(MemoryWidget *mw);

/**
 * Populates the context menu with an extra item option to assemble an
 * instruction at the current location.
 */
static void entry_popup_menu_handler(GtkEntry *entry, GtkMenu *menu, MemoryWidget *mw);

/**
 * Argument for the assemble_instruction callback function
 */
typedef struct assemble_job_s {
	MemoryWidget	*mw;
	word			address;
} assemble_job_t;

/**
 * Assembles an instruction
 */
static void assemble_instruction(GtkMenuItem *menu_item, assemble_job_t *aj);

/**
 * Help function for assemble_instruction that creates a dialog box that asks
 * the user for a string to assemble
 *
 * @param entry		The entry where the string to assemble will be entered
 * @param parent	The widget on which to center the dialog
 *
 * @return			The dialog
 */
static GtkWidget *assembly_dialog(GtkWidget *entry, GtkWidget *parent);

GType memory_widget_get_type(void)
{
	static GType mw_type = 0;
	if(!mw_type)
	{
		const GTypeInfo mw_info = {
			sizeof (MemoryWidgetClass),
			NULL, /* base_init */
			NULL, /* base_finalize */
			(GClassInitFunc) memory_widget_class_init, /* (GClassInitFunc) memory_widget_class_init,*/
			NULL, /* class_finalize */
			NULL, /* class_data */
			sizeof(MemoryWidget),
			0,
			(GInstanceInitFunc) memory_widget_init,
			NULL /*FIXME: What is this? */
		};

		mw_type = g_type_register_static(GTK_TYPE_TABLE, "MemoryWidget", &mw_info, 0);

	}
	return mw_type;
}

void memory_widget_class_init(MemoryWidgetClass *mw)
{
	GObjectClass *c = G_OBJECT_CLASS(mw);

	c->set_property = memory_widget_set_property;

    memory_widget_signals[SIGNAL_UPDATE_GUI] = g_signal_new("update_gui",
                                                    G_TYPE_FROM_CLASS(c),
                                                    G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
                                                    G_STRUCT_OFFSET(MemoryWidgetClass, memory_widget),
                                                    NULL,
                                                    NULL,
                                                    g_cclosure_marshal_VOID__VOID,
                                                    G_TYPE_NONE,
                                                    0);

	g_object_class_install_property(c,
					PROP_EDITABLE,
					g_param_spec_boolean("editable",
							"Editable",
							"Memory entries can be edited",
							TRUE,
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_RAM,
					g_param_spec_pointer("ram",
							"RAM",
							"Pointer to NES RAM",
							G_PARAM_WRITABLE));
			
	g_object_class_install_property(c,
					PROP_ROM,
					g_param_spec_pointer("rom",
							"ROM",
							"Pointer to NES ROM",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_ASSEMBLY_CALLBACK,
					g_param_spec_pointer("assembly_callback",
							"Assembly callback",
							"Function that assembles a string",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_RAM_READ_CALLBACK,
					g_param_spec_pointer("ram_read_callback",
							"RAM Read Callback",
							"Function that reads from NES RAM",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_ROM_READ_CALLBACK,
					g_param_spec_pointer("rom_read_callback",
							"ROM Read Callback",
							"Function that reads from NES ROM",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_RAM_WRITE_CALLBACK,
					g_param_spec_pointer("ram_write_callback",
							"RAM Write Callback",
							"Function that writes to NES RAM",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_ROM_WRITE_CALLBACK,
					g_param_spec_pointer("rom_write_callback",
							"ROM Write Callback",
							"Function that writes to NES ROM",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_PC,
					g_param_spec_uint("pc",
							"PC",
							"Address for PC highlighter",
							0,
							0xFFFF,
							0,
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
					PROP_AP,
					g_param_spec_uint("ap",
							"AP",
							"Address for address pointer highlighter",
							0,
							0xFFFF,
							0,
							G_PARAM_WRITABLE));
}

void memory_widget_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	MemoryWidget *mw = MEMORY_WIDGET(obj);
	(void) pspec;

	switch(prop_id)
	{
		case PROP_EDITABLE:
			mw->editable = g_value_get_boolean(val);
			memory_widget_update(mw);
			break;
		case PROP_RAM:
			mw->ram = (cpu_ram_t *) g_value_get_pointer(val);
			break;
		case PROP_ROM:
			mw->rom = (cpu_rom_t *) g_value_get_pointer(val);
			break;
		case PROP_ASSEMBLY_CALLBACK:
			mw->nes_asm_callback = ((gint (*)(char *to_asm, word pc, byte *out)) g_value_get_pointer(val));
			break;
		case PROP_RAM_READ_CALLBACK:
			mw->ram_read_callback = ((byte (*)(cpu_ram_t *ram, word address)) g_value_get_pointer(val));
			break;
		case PROP_ROM_READ_CALLBACK:
			mw->rom_read_callback = ((byte (*)(cpu_rom_t *rom, word address)) g_value_get_pointer(val));
			break;
		case PROP_RAM_WRITE_CALLBACK:
			mw->ram_write_callback = ((void (*)(cpu_ram_t *ram, word address, byte val)) g_value_get_pointer(val));
			break;
		case PROP_ROM_WRITE_CALLBACK:
			mw->rom_write_callback = ((void (*)(cpu_rom_t *rom, word address, byte val)) g_value_get_pointer(val));
			break;
		case PROP_PC:
			mw->pc = g_value_get_uint(val);
			break;
		case PROP_AP:
			mw->ap = g_value_get_uint(val);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}

}

GtkWidget *memory_widget_new()
{
	return GTK_WIDGET(g_object_new(memory_widget_get_type(), NULL));
}

static void memory_widget_init(MemoryWidget *mw)
{
	mw->editable = TRUE;
	mw->offset = 0x8000;
	mw->pc = 0x8000;
	mw->ap = 0x8001;
	mw->ram_read_callback = NULL;
	mw->rom_read_callback = NULL;
	GtkWidget *entry = NULL;
	gchar *tip = NULL;
	gint i,j;
	GtkAttachOptions attach_options;

	/* Setup the table */
	attach_options = GTK_SHRINK;
	gtk_table_set_homogeneous(GTK_TABLE(mw), FALSE);
	g_signal_connect(mw, "scroll-event", G_CALLBACK(table_scroll_handler), mw);

	/* The box at the top that contains the spinner and bookmarks */
	mw->top_box = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(mw), mw->top_box, 1, 17, 0, 1);
	gtk_widget_show(mw->top_box);

	/* Offset spin button */
	mw->offset_spinner = gtk_spin_button_new_with_range(0, 0xFF00, 0x10);
	gtk_widget_modify_font(GTK_WIDGET(mw->offset_spinner), pango_font_description_from_string("monospace"));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(mw->offset_spinner), mw->offset);
	gtk_entry_set_width_chars(GTK_ENTRY(mw->offset_spinner), 4);
	gtk_entry_set_max_length(GTK_ENTRY(mw->offset_spinner), 4);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(mw->offset_spinner), FALSE); /* Allow hex input */
	g_signal_connect(GTK_ENTRY(mw->offset_spinner), "key-press-event", G_CALLBACK(spinner_key_handler), mw);
	g_signal_connect(GTK_ENTRY(mw->offset_spinner), "value-changed", G_CALLBACK(spinner_value_changed_handler), mw);
	g_signal_connect(GTK_ENTRY(mw->offset_spinner), "input", G_CALLBACK(spinner_input_handler), NULL);
	g_signal_connect(GTK_ENTRY(mw->offset_spinner), "output", G_CALLBACK(spinner_output_handler), NULL);
	gtk_table_attach(GTK_TABLE(mw), mw->offset_spinner, 0, 1, 1, 2, attach_options, attach_options, 3, 3);
	gtk_widget_show(mw->offset_spinner);

	/* Labels at the top */
	for(i = 0; i < MEMORY_WIDGET_ROWS; i++)
	{
		char str[100];
		sprintf(str, "%X", i);
		mw->top_locations[i] = gtk_label_new(str);
		gtk_widget_modify_font(GTK_WIDGET(mw->top_locations[i]), pango_font_description_from_string("monospace"));
		gtk_table_attach(GTK_TABLE(mw), mw->top_locations[i], i + 1, i + 2, 1, 2, attach_options, attach_options, 3, 3);
		gtk_widget_show(mw->top_locations[i]);
	}

	/* Labels to the left */
	for(i = 0; i < MEMORY_WIDGET_ROWS; i++)
	{
		char str[100];
		sprintf(str, "0x%X", mw->offset + i * 0x10);
		mw->left_locations[i] = gtk_label_new(str);
		gtk_widget_modify_font(GTK_WIDGET(mw->left_locations[i]), pango_font_description_from_string("monospace"));
		gtk_table_attach(GTK_TABLE(mw), mw->left_locations[i], 0, 1, i + 2, i + 3, attach_options, attach_options, 3, 3);
		gtk_widget_show(mw->top_locations[i]);
	}

	/* The memory entries */
	for(i = 0; i < MEMORY_WIDGET_ROWS; i++)
	{
		for(j = 0; j < MEMORY_WIDGET_COLS; j++)
		{
			entry = gtk_entry_new();
			gtk_widget_modify_font(GTK_WIDGET(entry), pango_font_description_from_string("monospace"));
			mw->entries[i][j] = entry;
			gtk_entry_set_max_length(GTK_ENTRY(entry), 2);
			gtk_entry_set_width_chars(GTK_ENTRY(entry), 2);
			g_signal_connect(GTK_ENTRY(entry), "key-press-event", G_CALLBACK(entry_key_handler), mw);
			g_signal_connect(GTK_ENTRY(entry), "activate", G_CALLBACK(entry_activate_handler), mw);
			g_signal_connect(GTK_ENTRY(entry), "populate-popup", G_CALLBACK(entry_popup_menu_handler), mw);
			tip = g_strdup_printf("0x%04X", mw->offset + i * 0x10 + j);
			gtk_widget_set_tooltip_text(entry, tip);
			g_free(tip);
			
			gtk_table_attach_defaults(GTK_TABLE(mw), entry, j + 1, j + 2, i + 2, i + 3);

			gtk_widget_show(entry);
		}
	}
	memory_widget_update(mw);
}

void table_scroll_handler(GtkEntry *entry, GdkEvent *ev, MemoryWidget *mw)
{
	(void) entry;
	(void) mw;

	GdkEventScroll *e = (GdkEventScroll *) ev;
	memory_widget_update(mw);

	if(e->direction == GDK_SCROLL_DOWN)
		scroll_up(mw);

	if(e->direction == GDK_SCROLL_UP)
		scroll_down(mw);

	update_offset(mw, mw->offset);
}

void scroll_up(MemoryWidget *mw)
{
	if(mw->offset <= 0x0000)
		return;
	mw->offset -= 0x10;
	update_offset(mw, mw->offset);
}

void scroll_down(MemoryWidget *mw)
{
	if(mw->offset >= 0xFF00)
		return;
	mw->offset += 0x10;
	update_offset(mw, mw->offset);
}

gboolean entry_key_handler(GtkEntry *entry, GdkEvent *ev, MemoryWidget *mw)
{
	guint i, j;
	guint row;

	GdkEventKey *key = (GdkEventKey *) ev;
	guint keyval = key->keyval;

	GtkWidget *top = gtk_widget_get_parent(GTK_WIDGET(entry));
	guint pos = gtk_editable_get_position(GTK_EDITABLE(entry));
	guint len = gtk_entry_get_text_length(entry);

	/* Find out the position of our entry */
	for(i = 0; i < MEMORY_WIDGET_ROWS; i++)
		for(j = 0; j < MEMORY_WIDGET_COLS; j++)
		{
			if(GTK_ENTRY(entry) == GTK_ENTRY(mw->entries[i][j]))
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
				memory_widget_update(mw);

			if(pos == 0)
			{
				gtk_widget_child_focus(top, GTK_DIR_LEFT);
				entry_activate_handler(entry, mw);
	    		g_signal_emit_by_name(mw, "update_gui");
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
					memory_widget_update(mw);
				gtk_widget_child_focus(top, GTK_DIR_RIGHT);
				entry_activate_handler(entry, mw);
				memory_widget_update(mw);
	    		g_signal_emit_by_name(mw, "update_gui");
				return FALSE;
			} else
				return FALSE;

		case GDK_KEY_Page_Up:
			scroll_up(mw);
    		g_signal_emit_by_name(mw, "update_gui");
			return FALSE;

		case GDK_KEY_Page_Down:
			scroll_down(mw);
    		g_signal_emit_by_name(mw, "update_gui");
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
				memory_widget_update(mw);
			entry_activate_handler(entry, mw);
			memory_widget_update(mw);
    		g_signal_emit_by_name(mw, "update_gui");
			return FALSE;

		/* Restore entry's value (yeah, yeah, I'm lazy, the other ones' too) */
		case GDK_KEY_Escape:
			memory_widget_update(mw);
			return FALSE;
	}

	/* Key must be hexadecimal digit */
	if(!g_ascii_isxdigit(keyval))
		return TRUE;

	/* We should really never end up here */
	return FALSE;
}

void entry_activate_handler(GtkEntry *entry, MemoryWidget *mw)
{
	guint i, j;
	guint row, col;
	guint address = 0;
	byte val;

	for(i = 0; i < MEMORY_WIDGET_ROWS; i++)
		for(j = 0; j < MEMORY_WIDGET_COLS; j++)
		{
			if(GTK_ENTRY(entry) == GTK_ENTRY(mw->entries[i][j]))
			{
				row = i;
				col = j;
			}
		}

	address = mw->offset + row * 0x10 + col;
	val = g_ascii_strtoll(gtk_entry_get_text(entry), NULL, 16);
	
	if(address >= NES_RAM_OFFSET && address < NES_RAM_OFFSET + NES_RAM_SIZE)
	{ /* RAM space */
		if(mw->rom_write_callback)
		{
			mw->ram_write_callback(mw->ram, address - NES_RAM_OFFSET, val);
		}
	} else if((address >= NES_ROM_OFFSET) && ((uint32_t) address < (NES_ROM_OFFSET + NES_ROM_SIZE)))
	{ /* ROM space */
		if(mw->rom_write_callback)
		{
			mw->rom_write_callback(mw->rom, address - NES_ROM_OFFSET, val);
		}
	} else {
		/* Invalid memory address! This should never be reached! */
	}
}

gboolean spinner_key_handler(GtkEntry *spinner, GdkEvent *ev, MemoryWidget *mw)
{
	(void) spinner;
	(void) mw;
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
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinner), mw->offset);
		return FALSE;
	}

	/* Key must be hexadecimal digit */
	if(!g_ascii_isxdigit(keyval))
	{
		return TRUE;
	}
	return FALSE;
}

void spinner_value_changed_handler(GtkSpinButton *spinner, MemoryWidget *mw)
{
	(void) spinner;
	update_offset(mw, gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(mw->offset_spinner)));
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

void memory_widget_update(MemoryWidget *mw)
{
	guint i, j;
	char *text;
	char *tip;
	uint16_t address;

	/* Uh-huh. Static variables. I'm so retro! */
	static word previous_pc_x = 0;
	static word previous_pc_y = 0;
	static word previous_ap_x = 0;
	static word previous_ap_y = 0;

	/* Labels to the left */
	for(i = 0; i < MEMORY_WIDGET_ROWS; i++)
	{
		text = g_strdup_printf("0x%04X", mw->offset + i * 0x10);
		gtk_label_set_text(GTK_LABEL(mw->left_locations[i]), text);
		g_free(text);
	}

	/* Restore normal color to the entries that
	   previously contained highlighted pointers */
	gtk_widget_modify_text(GTK_WIDGET(mw->entries[previous_pc_y][previous_pc_x]), GTK_STATE_NORMAL, NULL);
	gtk_widget_modify_text(GTK_WIDGET(mw->entries[previous_pc_y][previous_pc_x]), GTK_STATE_INSENSITIVE, NULL);
	gtk_widget_modify_text(GTK_WIDGET(mw->entries[previous_ap_y][previous_ap_x]), GTK_STATE_NORMAL, NULL);
	gtk_widget_modify_text(GTK_WIDGET(mw->entries[previous_ap_y][previous_ap_x]), GTK_STATE_INSENSITIVE, NULL);

	/* Update the entries */
	for(i = 0; i < MEMORY_WIDGET_ROWS; i++)
	{
		for(j = 0; j < MEMORY_WIDGET_COLS; j++)
		{
			address = mw->offset + i * 0x10 + j;
			guint val;
			GtkEntry *entry = GTK_ENTRY(mw->entries[i][j]);

			if(address >= NES_RAM_OFFSET && address < NES_RAM_OFFSET + NES_RAM_SIZE) {
				/* RAM space */
				if(!mw->ram_read_callback) {
					text = g_strdup_printf("Er");
					gtk_widget_set_sensitive(GTK_WIDGET(entry), FALSE);
				} else {
					val = mw->ram_read_callback(mw->ram, address - NES_RAM_OFFSET);
					text = g_strdup_printf("%02X", val);
					if(mw->editable)
						gtk_widget_set_sensitive(GTK_WIDGET(entry), TRUE);
					else
						gtk_widget_set_sensitive(GTK_WIDGET(entry), FALSE);
				}
				/* The cast below is just to get rid of a compiler warning, ignore it! */
			} else if((address >= NES_ROM_OFFSET) && ((uint32_t) address < (NES_ROM_OFFSET + NES_ROM_SIZE))) { 
				/* ROM space */
				if(!mw->rom_read_callback) {
					text = g_strdup_printf("Er");
					gtk_widget_set_sensitive(GTK_WIDGET(entry), FALSE);
				} else {
					val = mw->rom_read_callback(mw->rom, address - NES_ROM_OFFSET);
					text = g_strdup_printf("%02X", val);
					if(mw->editable)
						gtk_widget_set_sensitive(GTK_WIDGET(entry), TRUE);
					else
						gtk_widget_set_sensitive(GTK_WIDGET(entry), FALSE);
				}
			} else {
				/* Not a valid memory address */
				text = g_strdup_printf("NA");
				gtk_widget_set_sensitive(GTK_WIDGET(entry), FALSE);
			}
			gtk_entry_set_text(entry, text);
			g_free(text);

			if(mw->ap == address && mw->pc == address) {
				/* Both PC and address point to this entry */
				GdkColor cyan = { 0, 0, 0xFFFF, 0xFFFF };
				gtk_widget_modify_text(GTK_WIDGET(entry), GTK_STATE_NORMAL, &cyan);
				gtk_widget_modify_text(GTK_WIDGET(entry), GTK_STATE_INSENSITIVE, &cyan);
				previous_pc_x = j;
				previous_pc_y = i;
			} else if(mw->pc == address) {
				/* Handle PC highligting */
				GdkColor blue = { 0, 0, 0, 0xFFFF };
				gtk_widget_modify_text(GTK_WIDGET(entry), GTK_STATE_NORMAL, &blue);
				gtk_widget_modify_text(GTK_WIDGET(entry), GTK_STATE_INSENSITIVE, &blue);
				previous_pc_x = j;
				previous_pc_y = i;
			} else  if(mw->ap == address) {
				/* Handle address pointer highligting */
				GdkColor green = { 0, 0, 0xFFFF, 0 };
				gtk_widget_modify_text(GTK_WIDGET(entry), GTK_STATE_NORMAL, &green);
				gtk_widget_modify_text(GTK_WIDGET(entry), GTK_STATE_INSENSITIVE, &green);
				previous_ap_x = j;
				previous_ap_y = i;
			}
			tip = g_strdup_printf("0x%04X", mw->offset + i * 0x10 + j);
			gtk_widget_set_tooltip_text(GTK_WIDGET(entry), tip);
			g_free(tip);
		}
	}
}

void update_offset(MemoryWidget *mw, guint value)
{
	mw->offset = (0xFFF0 & value);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(mw->offset_spinner), mw->offset);
	memory_widget_update(mw);
}

void entry_popup_menu_handler(GtkEntry *entry, GtkMenu *menu, MemoryWidget *mw)
{

	GtkWidget *assembly_item;
	GtkWidget *item_separator;
	assemble_job_t *aj = NULL;
	guint i, j;
	guint row, col;
	guint address;

	/* Find out the position of the entry in which the menu item was selected */
	for(i = 0; i < MEMORY_WIDGET_ROWS; i++)
		for(j = 0; j < MEMORY_WIDGET_COLS; j++)
		{
			if(GTK_ENTRY(entry) == GTK_ENTRY(mw->entries[i][j]))
			{
				row = i;
				col = j;
			}
		}

	address = mw->offset + row * 0x10 + col;

	/* FIXME: Oh, nooes! This is a MEMORY LEAK! The struct is only freed
	   if the menu item is actually clicked! */
	aj = malloc(sizeof(assemble_job_t));
	aj->mw = mw;
	aj->address = address;

	assembly_item = gtk_menu_item_new_with_label("Assemble instruction here");
	g_signal_connect(assembly_item, "activate", G_CALLBACK(assemble_instruction), aj);
	
	item_separator = gtk_separator_menu_item_new();
	gtk_menu_shell_insert(GTK_MENU_SHELL(menu), item_separator, 0);
	gtk_menu_shell_insert(GTK_MENU_SHELL(menu), assembly_item, 0);
	gtk_widget_show(assembly_item);
	gtk_widget_show(item_separator);
}

GtkWidget *assembly_dialog(GtkWidget *entry, GtkWidget *parent)
{
	GtkWidget *dialog = NULL;
	GtkWidget *box;
	GtkWidget *label;

	dialog = gtk_dialog_new_with_buttons("Assemble string",
											GTK_WINDOW(parent),
											GTK_DIALOG_MODAL,
											GTK_STOCK_OK,
											GTK_RESPONSE_ACCEPT,
											GTK_STOCK_CANCEL,
											GTK_RESPONSE_REJECT,
											NULL);

	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);
	gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);

	box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	label = gtk_label_new_with_mnemonic("Enter string to assemble");

	gtk_container_add(GTK_CONTAINER(box), label);
	gtk_container_add(GTK_CONTAINER(box), entry);
	gtk_widget_show_all(dialog);

	return dialog;
}

void assemble_instruction(GtkMenuItem *menu_item, assemble_job_t *aj)
{
	(void) menu_item;
	guint address = aj->address;
	MemoryWidget *mw = aj->mw;
	GtkWidget *dialog = NULL;
	GtkWidget *entry = NULL;
	guint i = 0;
	gint res = 0;
	gchar *to_assemble = NULL;
	byte asm_bytes[3];

	free(aj);

	entry = gtk_entry_new();
	dialog = assembly_dialog(entry, gtk_widget_get_toplevel(GTK_WIDGET(mw)));
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	if(res != GTK_RESPONSE_ACCEPT)
	{
		gtk_widget_destroy(dialog);
		return;
	}

	/* FIXME: I know, this is uh-gly! */
	to_assemble = (char *) gtk_entry_get_text(GTK_ENTRY(entry));

	res = mw->nes_asm_callback(to_assemble, address, asm_bytes);

	if(res > 0 && res <= 3)
	{
		g_print("Assembling successful!\n");
		for(i = 0; i < (guint) res; i++)
		{
			if((address + i >= NES_RAM_OFFSET) && (address + i < NES_RAM_SIZE))
			{
				mw->ram_write_callback(mw->ram, address + i - NES_RAM_OFFSET, asm_bytes[i]);
			} else if((address + i >= NES_ROM_OFFSET) && (address + i < NES_ROM_OFFSET + NES_ROM_SIZE))
			{
				mw->rom_write_callback(mw->rom, address + i - NES_ROM_OFFSET, asm_bytes[i]);
			}
		}
	    g_signal_emit_by_name(mw, "update_gui");
	}

	gtk_widget_destroy(dialog);
}

