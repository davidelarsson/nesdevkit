#include <stdlib.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkentry.h>
#include <gdk/gdkkeysyms.h>
#include "disasm_widget.h"

/**
 * Property constants.
 */
enum {
	PROP_0,
	PROP_RAM,
	PROP_ROM,
	PROP_CPU,
	PROP_RAM_READ_CALLBACK,
	PROP_ROM_READ_CALLBACK,
	PROP_READ_PC_CALLBACK,
	PROP_DISASM_CALLBACK,
	PROP_ADDRESS,
	N_PROPERTIES
};

/**
 * Set properties of theclass. This is mandatory if properties are
 * used.
 */
static void disasm_widget_set_property(GObject *obj,
							guint prop_id,
							const GValue *val,
							GParamSpec *pspec);

/**
 * Mandatory function. When first called, it tells GLib of 
 * the new class, and gets a unique ID that identifies the class.
 *
 * Subsequent calls just returns the ID of the class.
 */
GType disasm_widget_get_type();

/**
 * Mandatory function. Usually sets the fields of the structure to default values.
 * For our composite widget, we also create our component widgets.
 */
static void disasm_widget_init(DisasmWidget *daw);

/**
 * Sets up the class structure and also the signals for the class.
 * This function is not mandatory if no signals are used. Then a NULL must be
 * provided in the class_init field of the GTypeInfo struct used by 
 * g_type_register_static() in disasm_widget_get_type() below.
 */
static void disasm_widget_class_init(DisasmWidget *daw);

/**
 * Not mandatory, but very useful. Creates and returns a new widget.
 */
GtkWidget *disasm_widget_new();

/**
 * Handles the input of the offset spinner. Only hex digits are allowed.
 */
static gboolean spinner_key_handler(GtkEntry *entry, GdkEvent *ev, DisasmWidget *mw);

/**
 * Handles hex-to-decimal conversion of the spin button.
 */
static gboolean spinner_input_handler(GtkSpinButton *spinner, gdouble *new_value, gpointer dummy);

/**
 * Handles value-to-hexadecimal conversion of the spin button.
 */
static gboolean spinner_output_handler(GtkSpinButton *spinner, gpointer dummy);

static void spinner_value_changed_handler(GtkSpinButton *spinner, DisasmWidget *daw);

static void check_button_toggled(GtkToggleButton *button, DisasmWidget *daw);

GType disasm_widget_get_type()
{
	static GType daw_type = 0;

	if(!daw_type)
	{
		const GTypeInfo daw_info=
			{
				sizeof(DisasmWidgetClass), /* class_size */
				NULL,						/* base_init */
				NULL,						/* base_finalize */
				(GClassInitFunc) disasm_widget_class_init, /* class_init */
				NULL,						/* class_finalize */
				NULL,						/* class_data */
				sizeof(DisasmWidget),		/* instance_size */
				0,							/* n_preallocs */
				(GInstanceInitFunc) disasm_widget_init, /* instance_init */
				NULL						/* value_table */
			};
		daw_type = g_type_register_static(GTK_TYPE_TABLE, /* parent_type */
			"disasm_widget",							/* type_name */
			&daw_info,									/* info */
			0);											/* flags */
	}
	return daw_type;
}

void disasm_widget_class_init(DisasmWidget *daw)
{
    GObjectClass *c = G_OBJECT_CLASS(daw);

    c->set_property = disasm_widget_set_property;

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
                        PROP_CPU,
                        g_param_spec_pointer("cpu",
                            "CPU",
                            "Pointer to the NES CPU",
                            G_PARAM_WRITABLE));

    g_object_class_install_property(c,
                    PROP_ADDRESS,
                    g_param_spec_uint("address",
                            "Address",
                            "Address to begin disassebly",
                            0,
                            0xFFFF,
                            0,
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
                    PROP_DISASM_CALLBACK,
                    g_param_spec_pointer("disasm_callback",
                            "Disassembly callback",
                            "Callback to function that does the actual disassembly",
                            G_PARAM_WRITABLE));

    g_object_class_install_property(c,
                        PROP_READ_PC_CALLBACK,
                        g_param_spec_pointer("read_pc_callback",
                            "Read the program counter callback",
                            "Callback function for reading the program counter from the CPU",
                            G_PARAM_WRITABLE));
}

void disasm_widget_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	DisasmWidget *daw = DISASM_WIDGET(obj);

    switch(prop_id)
    {
        case PROP_RAM:
            daw->ram = (cpu_ram_t *) g_value_get_pointer(val);
            break;
        case PROP_ROM:
            daw->rom = (cpu_rom_t *) g_value_get_pointer(val);
            break;
        case PROP_CPU:
            daw->cpu = (cpu_t *) g_value_get_pointer(val);
            break;
        case PROP_RAM_READ_CALLBACK:
            daw->ram_read_callback = ((uint8_t (*)(cpu_ram_t *ram, word address)) g_value_get_pointer(val));
            break;
        case PROP_ROM_READ_CALLBACK:
            daw->rom_read_callback = ((uint8_t (*)(cpu_rom_t *rom, word address)) g_value_get_pointer(val));
            break;
        case PROP_DISASM_CALLBACK:
            daw->disasm_callback = ((guint (*)(byte *bytes, word address, gchar *out)) g_value_get_pointer(val));
            break;
        case PROP_ADDRESS:
			//daw->address = (word) g_value_get_uint(val);
            break;
        case PROP_READ_PC_CALLBACK:
            daw->read_pc_callback = ((word (*)(cpu_t *)) g_value_get_pointer(val));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
            break;
	}
}

void disasm_widget_init(DisasmWidget *daw)
{

	daw->ram = NULL;
	daw->rom = NULL;
	daw->ram_read_callback = NULL;
	daw->rom_read_callback = NULL;
	daw->address = 0x8000;
	daw->auto_read_pc = TRUE;

	gtk_table_set_homogeneous(GTK_TABLE(daw), FALSE);

	daw->text_view = gtk_text_view_new();

	/* Check button */
	daw->check_button = gtk_check_button_new_with_label("Auto");
	gtk_widget_modify_font(GTK_WIDGET(daw->check_button), pango_font_description_from_string("monospace"));
	g_signal_connect(GTK_TOGGLE_BUTTON(daw->check_button), "toggled", G_CALLBACK(check_button_toggled), daw);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(daw->check_button), TRUE);

	/* Spinner */
	daw->address_spinner = gtk_spin_button_new_with_range(0, 0xFFFF, 1);
	gtk_widget_modify_font(GTK_WIDGET(daw->address_spinner), pango_font_description_from_string("monospace"));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(daw->address_spinner), 0x8000);
    gtk_entry_set_width_chars(GTK_ENTRY(daw->address_spinner), 4);
    gtk_entry_set_max_length(GTK_ENTRY(daw->address_spinner), 4);
	gtk_widget_set_sensitive(GTK_WIDGET(daw->address_spinner), FALSE);
    gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(daw->address_spinner), FALSE); /* Allow hex input */

    g_signal_connect(GTK_ENTRY(daw->address_spinner), "key-press-event", G_CALLBACK(spinner_key_handler), daw);
    g_signal_connect(GTK_ENTRY(daw->address_spinner), "value-changed", G_CALLBACK(spinner_value_changed_handler), daw);
    g_signal_connect(GTK_ENTRY(daw->address_spinner), "input", G_CALLBACK(spinner_input_handler), NULL);
    g_signal_connect(GTK_ENTRY(daw->address_spinner), "output", G_CALLBACK(spinner_output_handler), NULL);


	/* Text view */
	gtk_text_view_set_editable(GTK_TEXT_VIEW(daw->text_view), FALSE);
	gtk_widget_modify_font(GTK_WIDGET(daw->text_view), pango_font_description_from_string("monospace"));

	gtk_table_attach_defaults(GTK_TABLE(daw), daw->check_button, 0, 1, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(daw), daw->address_spinner, 1, 2, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(daw), daw->text_view, 0, 2, 1, 2);
	gtk_widget_show_all(GTK_WIDGET(daw));

}

GtkWidget *disasm_widget_new()
{
	return GTK_WIDGET(g_object_new(DISASM_WIDGET_TYPE, NULL));
}

void disasm_widget_update(DisasmWidget *daw)
{
	byte to_asm[3];
	char string[12];
	char *str;
	guint address;
	guint bytes_assembled;
	guint line;
	guint len;

	GtkTextBuffer *buf;

	buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(daw->text_view));

	if(!(daw->ram) || !(daw->rom))
	{
		gtk_text_buffer_set_text(buf, "N/A", -1);
		return;
	}

	bytes_assembled = 0;

	gtk_text_buffer_set_text(buf, "", -1);

	if(daw->auto_read_pc)
	{
		daw->address = daw->read_pc_callback(daw->cpu);
	}

	for(line = 0; line < DISASM_WIDGET_LINES; line++)
	{
		address = daw->address + bytes_assembled;
		if(address < 0x2000)
		{
			/* We're in RAM */
			if(!daw->ram_read_callback)
			{
				gtk_text_buffer_set_text(buf, "N/A", -1);
				return;
			}
	
//			g_print("Reading 3 bytes from RAM...\n");
			/* FIXME: Is the address correct? */
			to_asm[0] = daw->ram_read_callback(daw->ram, (address & 0x1FFF));
			to_asm[1] = daw->ram_read_callback(daw->ram, (address & 0x1FFF) + 1);
			to_asm[2] = daw->ram_read_callback(daw->ram, (address & 0x1FFF) + 2);
		} else if(address >= 0x8000 && (guint) address < 0xFFFF)
		{
			/* We're in ROM */
			if(!daw->rom_read_callback)
			{
				gtk_text_buffer_set_text(buf, "N/A", -1);
				return;
			}
	
//			g_print("Reading 3 bytes from ROM...\n");
			to_asm[0] = daw->rom_read_callback(daw->rom, address - 0x8000);
			to_asm[1] = daw->rom_read_callback(daw->rom, address - 0x8000 + 1);
			to_asm[2] = daw->rom_read_callback(daw->rom, address - 0x8000 + 2);
		}
	
		/* Assemble the bytes */
		len = daw->disasm_callback(to_asm, address, string);
		bytes_assembled += len;
		switch(len)
		{
			case 3:
				str = g_strdup_printf("0x%04X: %02X %02X %02X %-15s",
							address,
							to_asm[0],
							to_asm[1],
							to_asm[2],
							string);
				gtk_text_buffer_insert_at_cursor(buf, str, -1);
				g_free(str);
				break;
	
			case 2:
				str = g_strdup_printf("0x%04X: %02X %02X    %-15s",
							address,
							to_asm[0],
							to_asm[1],
							string);
				gtk_text_buffer_insert_at_cursor(buf, str, -1);
				g_free(str);
				break;
			case 1:
			default:
				str = g_strdup_printf("0x%04X: %02X       %-15s",
							address,
							to_asm[0],
							string);
				gtk_text_buffer_insert_at_cursor(buf, str, -1);
				g_free(str);
				break;
		}

		if(line != DISASM_WIDGET_LINES -1)
			gtk_text_buffer_insert_at_cursor(buf, "\n", -1);
	}
}

gboolean spinner_key_handler(GtkEntry *spinner, GdkEvent *ev, DisasmWidget *daw)
{
	(void) spinner;
	(void) daw;
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
		case GDK_KEY_Escape:
            return FALSE;
    }

    /* Key must be hexadecimal digit */
    if(!g_ascii_isxdigit(keyval))
        return TRUE;

    return FALSE;
}

void spinner_value_changed_handler(GtkSpinButton *spinner, DisasmWidget *daw)
{
    (void) spinner;
	gint value = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(daw->address_spinner));
    daw->address = value;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(daw->address_spinner), daw->address);
	disasm_widget_update(daw);
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

static void check_button_toggled(GtkToggleButton *button, DisasmWidget *daw)
{
	gboolean a = !gtk_toggle_button_get_active(button);
	daw->auto_read_pc = !a;
	if(daw->address_spinner)
		gtk_widget_set_sensitive(GTK_WIDGET(daw->address_spinner), a);
	if(a)
		daw->address = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(daw->address_spinner));
	disasm_widget_update(daw);
}

