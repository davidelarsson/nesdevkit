#include <gtk/gtk.h>
#include <gtk/gtksignal.h>
#include <gtk/gtktable.h>
#include <gtk/gtkentry.h>
#include <gdk/gdkkeysyms.h>
#include "cpu_widget.h"

/**
 * Property constants.
 */
enum {
	PROP_0,
	PROP_EDITABLE,
	PROP_CPU,
	PROP_READ_A_CALLBACK,
	PROP_READ_X_CALLBACK,
	PROP_READ_Y_CALLBACK,
	PROP_READ_SP_CALLBACK,
	PROP_READ_P_CALLBACK,
	PROP_READ_PC_CALLBACK,
	PROP_READ_CYCLES_CALLBACK,
	PROP_WRITE_A_CALLBACK,
	PROP_WRITE_X_CALLBACK,
	PROP_WRITE_Y_CALLBACK,
	PROP_WRITE_SP_CALLBACK,
	PROP_WRITE_P_CALLBACK,
	PROP_WRITE_PC_CALLBACK,
	N_PROPERTIES
};

/**
 * Signal constants
 */
enum {
    SIGNAL_UPDATE_GUI,
    SIGNAL_LAST
};

static guint cpu_widget_signals[SIGNAL_LAST] = { 0 };

/**
 * Set properties of the class. This is mandatory if properties are
 * used.
 */
static void cpu_widget_set_property(GObject *obj,
							guint prop_id,
							const GValue *val,
							GParamSpec *pspec);

/**
 * Mandatory function. When first called, it tells GLib of 
 * the new class, and gets a unique ID that identifies the class.
 *
 * Subsequent calls just returns the ID of the class.
 */
GType cpu_widget_get_type();

/**
 * Mandatory function. Usually sets the fields of the structure to default values.
 * For our composite widget, we also create our component widgets.
 */
static void cpu_widget_init(CpuWidget *cw);

/**
 * Sets up the class structure and also the signals for the class.
 * This function is not mandatory if no signals are used. Then a NULL must be
 * provided in the class_init field of the GTypeInfo struct used by 
 * g_type_register_static() in cpu_widget_get_type() below.
 */
static void cpu_widget_class_init(CpuWidget *cw);

/**
 * Not mandatory, but very useful. Creates and returns a new widget.
 */
GtkWidget *cpu_widget_new();

/**
 * Handles the input of entries, makes sure only hex digits are allowed. Also,
 * if tab or Shift-Tab is pressed, the entry is updated. This is not default
 * behavior for a GtkEntry.
 */
static gboolean entry_a_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw);
static gboolean entry_x_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw);
static gboolean entry_y_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw);
static gboolean entry_p_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw);
static gboolean entry_sp_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw);
static gboolean entry_pc_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw);

/**
 * Called whenever enter, tab or shift-tab is pressed in an entry
 */
void entry_a_activate_handler(GtkEntry *entry, CpuWidget *cw);
void entry_x_activate_handler(GtkEntry *entry, CpuWidget *cw);
void entry_y_activate_handler(GtkEntry *entry, CpuWidget *cw);
void entry_p_activate_handler(GtkEntry *entry, CpuWidget *cw);
void entry_sp_activate_handler(GtkEntry *entry, CpuWidget *cw);
void entry_pc_activate_handler(GtkEntry *entry, CpuWidget *cw);

GType cpu_widget_get_type()
{
	static GType cw_type = 0;

	if(!cw_type)
	{
		const GTypeInfo cw_info=
			{
				sizeof(CpuWidgetClass), /* class_size */
				NULL,						/* base_init */
				NULL,						/* base_finalize */
				(GClassInitFunc) cpu_widget_class_init, /* class_init */
				NULL,						/* class_finalize */
				NULL,						/* class_data */
				sizeof(CpuWidget),		/* instance_size */
				0,							/* n_preallocs */
				(GInstanceInitFunc) cpu_widget_init, /* instance_init */
				NULL						/* value_table */
			};
		cw_type = g_type_register_static(GTK_TYPE_TABLE, /* parent_type */
			"cpu_widget",							/* type_name */
			&cw_info,									/* info */
			0);											/* flags */
	}
	return cw_type;
}

static void cpu_widget_class_init(CpuWidget *cw)
{

	GObjectClass *c = G_OBJECT_CLASS(cw);

	c->set_property = cpu_widget_set_property;

    cpu_widget_signals[SIGNAL_UPDATE_GUI] = g_signal_new("update_gui",
                                                    G_TYPE_FROM_CLASS(c),
                                                    G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
                                                    G_STRUCT_OFFSET(CpuWidgetClass, cpu_widget),
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
						PROP_CPU,
						g_param_spec_pointer("cpu",
							"CPU",
							"Pointer to the NES CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_READ_A_CALLBACK,
						g_param_spec_pointer("read_a_callback",
							"Read accumulator callback",
							"Callback function for reading the accumulator from the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_READ_X_CALLBACK,
						g_param_spec_pointer("read_x_callback",
							"Read the X register callback",
							"Callback function for reading the X register from the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_READ_Y_CALLBACK,
						g_param_spec_pointer("read_y_callback",
							"Read the Y register callback",
							"Callback function for reading the Y register from the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_READ_SP_CALLBACK,
						g_param_spec_pointer("read_sp_callback",
							"Read the stack pointer callback",
							"Callback function for reading the stack pointer from the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_READ_P_CALLBACK,
						g_param_spec_pointer("read_p_callback",
							"Read the status register callback",
							"Callback function for reading the status register from the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_READ_PC_CALLBACK,
						g_param_spec_pointer("read_pc_callback",
							"Read the program counter callback",
							"Callback function for reading the program counter from the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_READ_CYCLES_CALLBACK,
						g_param_spec_pointer("read_cycles_callback",
							"Read clock cycles callback",
							"Callback function for reading the number of clock cycles that have been consumed by the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_WRITE_A_CALLBACK,
						g_param_spec_pointer("write_a_callback",
							"Write accumulator callback",
							"Callback function for writing the accumulator to the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_WRITE_X_CALLBACK,
						g_param_spec_pointer("write_x_callback",
							"Write the X register callback",
							"Callback function for writing the X register to the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_WRITE_Y_CALLBACK,
						g_param_spec_pointer("write_y_callback",
							"Write the Y register callback",
							"Callback function for writing the Y register to the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_WRITE_SP_CALLBACK,
						g_param_spec_pointer("write_sp_callback",
							"Write the stack pointer callback",
							"Callback function for writing the stack pointer to the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_WRITE_P_CALLBACK,
						g_param_spec_pointer("write_p_callback",
							"Write the status register callback",
							"Callback function for writing the status register to the CPU",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_WRITE_PC_CALLBACK,
						g_param_spec_pointer("write_pc_callback",
							"Write the program counter callback",
							"Callback function for writing the program counter to the CPU",
							G_PARAM_WRITABLE));

}

void cpu_widget_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	CpuWidget *cw = CPU_WIDGET(obj);

	switch(prop_id)
	{
        case PROP_EDITABLE:
            cw->editable = g_value_get_boolean(val);
            cpu_widget_update(cw);
            break;
		case PROP_CPU:
			cw->cpu = (cpu_t *) g_value_get_pointer(val);
			break;
		case PROP_READ_A_CALLBACK:
			cw->read_a_callback = ((byte (*)(cpu_t *)) g_value_get_pointer(val));
			break;
		case PROP_READ_X_CALLBACK:
			cw->read_x_callback = ((byte (*)(cpu_t *)) g_value_get_pointer(val));
			break;
		case PROP_READ_Y_CALLBACK:
			cw->read_y_callback = ((byte (*)(cpu_t *)) g_value_get_pointer(val));
			break;
		case PROP_READ_SP_CALLBACK:
			cw->read_sp_callback = ((byte (*)(cpu_t *)) g_value_get_pointer(val));
			break;
		case PROP_READ_P_CALLBACK:
			cw->read_p_callback = ((byte (*)(cpu_t *)) g_value_get_pointer(val));
			break;
		case PROP_READ_PC_CALLBACK:
			cw->read_pc_callback = ((word (*)(cpu_t *)) g_value_get_pointer(val));
			break;
		case PROP_READ_CYCLES_CALLBACK:
			cw->read_cycles_callback = ((gulong (*)(cpu_t *)) g_value_get_pointer(val));
			break;
		case PROP_WRITE_A_CALLBACK:
			cw->write_a_callback = ((void (*)(cpu_t *, byte val)) g_value_get_pointer(val));
			break;
		case PROP_WRITE_X_CALLBACK:
			cw->write_x_callback = ((void (*)(cpu_t *, byte val)) g_value_get_pointer(val));
			break;
		case PROP_WRITE_Y_CALLBACK:
			cw->write_y_callback = ((void (*)(cpu_t *, byte val)) g_value_get_pointer(val));
			break;
		case PROP_WRITE_SP_CALLBACK:
			cw->write_sp_callback = ((void (*)(cpu_t *, byte val)) g_value_get_pointer(val));
			break;
		case PROP_WRITE_P_CALLBACK:
			cw->write_p_callback = ((void (*)(cpu_t *, byte val)) g_value_get_pointer(val));
			break;
		case PROP_WRITE_PC_CALLBACK:
			cw->write_pc_callback = ((void (*)(cpu_t *, word val)) g_value_get_pointer(val));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void cpu_widget_init(CpuWidget *cw)
{

	GtkWidget *flag_bits_label= NULL;
	GtkWidget *p_label = NULL;
	GtkWidget *a_label = NULL;
	GtkWidget *x_label = NULL;
	GtkWidget *y_label = NULL;
	GtkWidget *sp_label = NULL;
	GtkWidget *pc_label = NULL;
	GtkWidget *clk_label = NULL;
	GtkWidget *align = NULL;

	cw->editable = TRUE;
	cw->flag_bits = NULL;
	cw->p_entry = NULL;
	cw->a_entry = NULL;
	cw->x_entry = NULL;
	cw->y_entry = NULL;

	cw->cpu = NULL;
	cw->read_a_callback = NULL;
	cw->read_x_callback = NULL;
	cw->read_y_callback = NULL;
	cw->read_sp_callback = NULL;
	cw->read_p_callback = NULL;
	cw->read_pc_callback = NULL;
	cw->read_cycles_callback = NULL;
	cw->write_a_callback = NULL;
	cw->write_x_callback = NULL;
	cw->write_y_callback = NULL;
	cw->write_sp_callback = NULL;
	cw->write_p_callback = NULL;
	cw->write_pc_callback = NULL;

	gtk_table_resize(GTK_TABLE(cw), CPU_WIDGET_ROWS, CPU_WIDGET_COLS);
	gtk_table_set_homogeneous(GTK_TABLE(cw), TRUE);

	/* The NV-BDIZC label */
	flag_bits_label = gtk_label_new_with_mnemonic("NV-BDIZC");
	gtk_widget_modify_font(GTK_WIDGET(flag_bits_label), pango_font_description_from_string("monospace"));

	gtk_table_attach_defaults(GTK_TABLE(cw), flag_bits_label, 0, 2, 0, 1);

	/* The 00100000 label */
	cw->flag_bits = gtk_label_new_with_mnemonic("00100000");
	gtk_widget_modify_font(GTK_WIDGET(cw->flag_bits), pango_font_description_from_string("monospace"));

	gtk_table_attach_defaults(GTK_TABLE(cw), cw->flag_bits, 0, 2, 1, 2);

	/* The P label */
	p_label = gtk_label_new_with_mnemonic("P: 0x");
	gtk_widget_modify_font(GTK_WIDGET(p_label), pango_font_description_from_string("monospace"));

	align = gtk_alignment_new(1, 0.5, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), p_label);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 0, 1, 2, 3);

	/* Entry for status register */
	cw->p_entry = gtk_entry_new();
	g_signal_connect(G_OBJECT(cw->p_entry), "key-press-event", G_CALLBACK(entry_p_key_handler), cw);
	g_signal_connect(G_OBJECT(cw->p_entry), "activate", G_CALLBACK(entry_p_activate_handler), cw);
	gtk_entry_set_max_length(GTK_ENTRY(cw->p_entry), 2);
	gtk_entry_set_width_chars(GTK_ENTRY(cw->p_entry), 2);

	align = gtk_alignment_new(0, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), cw->p_entry);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 1, 2, 2, 3);

	/* The A label */
	a_label = gtk_label_new_with_mnemonic("A: 0x");
	gtk_widget_modify_font(GTK_WIDGET(a_label), pango_font_description_from_string("monospace"));

	align = gtk_alignment_new(1, 0.5, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), a_label);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 0, 1, 3, 4);

	/* Entry for accumulator */
	cw->a_entry = gtk_entry_new();
	g_signal_connect(G_OBJECT(cw->a_entry), "key-press-event", G_CALLBACK(entry_a_key_handler), cw);
	g_signal_connect(G_OBJECT(cw->a_entry), "activate", G_CALLBACK(entry_a_activate_handler), cw);
	gtk_entry_set_max_length(GTK_ENTRY(cw->a_entry), 2);
	gtk_entry_set_width_chars(GTK_ENTRY(cw->a_entry), 2);

	align = gtk_alignment_new(0, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), cw->a_entry);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 1, 2, 3, 4);

	/* The X label */
	x_label = gtk_label_new_with_mnemonic("X: 0x");
	gtk_widget_modify_font(GTK_WIDGET(x_label), pango_font_description_from_string("monospace"));

	align = gtk_alignment_new(1, 0.5, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), x_label);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 0, 1, 4, 5);

	/* Entry for X register */
	cw->x_entry = gtk_entry_new();
	g_signal_connect(G_OBJECT(cw->x_entry), "key-press-event", G_CALLBACK(entry_x_key_handler), cw);
	g_signal_connect(G_OBJECT(cw->x_entry), "activate", G_CALLBACK(entry_x_activate_handler), cw);
	gtk_entry_set_max_length(GTK_ENTRY(cw->x_entry), 2);
	gtk_entry_set_width_chars(GTK_ENTRY(cw->x_entry), 2);

	align = gtk_alignment_new(0, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), cw->x_entry);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 1, 2, 4, 5);

	/* The Y label */
	y_label = gtk_label_new_with_mnemonic("Y: 0x");
	gtk_widget_modify_font(GTK_WIDGET(y_label), pango_font_description_from_string("monospace"));

	align = gtk_alignment_new(1, 0.5, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), y_label);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 0, 1, 5, 6);

	/* Entry for Y register */
	cw->y_entry = gtk_entry_new();
	g_signal_connect(G_OBJECT(cw->y_entry), "key-press-event", G_CALLBACK(entry_y_key_handler), cw);
	g_signal_connect(G_OBJECT(cw->y_entry), "activate", G_CALLBACK(entry_y_activate_handler), cw);
	gtk_entry_set_max_length(GTK_ENTRY(cw->y_entry), 2);
	gtk_entry_set_width_chars(GTK_ENTRY(cw->y_entry), 2);

	align = gtk_alignment_new(0, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), cw->y_entry);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 1, 2, 5, 6);

	/* The SP label */
	sp_label = gtk_label_new_with_mnemonic("SP: 0x");
	gtk_widget_modify_font(GTK_WIDGET(sp_label), pango_font_description_from_string("monospace"));

	align = gtk_alignment_new(1, 0.5, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), sp_label);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 0, 1, 6, 7);

	/* Entry for SP register */
	cw->sp_entry = gtk_entry_new();
	g_signal_connect(G_OBJECT(cw->sp_entry), "key-press-event", G_CALLBACK(entry_sp_key_handler), cw);
	g_signal_connect(G_OBJECT(cw->sp_entry), "activate", G_CALLBACK(entry_sp_activate_handler), cw);
	gtk_entry_set_max_length(GTK_ENTRY(cw->sp_entry), 2);
	gtk_entry_set_width_chars(GTK_ENTRY(cw->sp_entry), 2);

	align = gtk_alignment_new(0, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), cw->sp_entry);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 1, 2, 6, 7);

	/* The PC label */
	pc_label = gtk_label_new_with_mnemonic("PC: 0x");
	gtk_widget_modify_font(GTK_WIDGET(pc_label), pango_font_description_from_string("monospace"));

	align = gtk_alignment_new(1, 0.5, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), pc_label);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 0, 1, 7, 8);

	/* Entry for PC register */
	cw->pc_entry = gtk_entry_new();
	g_signal_connect(G_OBJECT(cw->pc_entry), "key-press-event", G_CALLBACK(entry_pc_key_handler), cw);
	g_signal_connect(G_OBJECT(cw->pc_entry), "activate", G_CALLBACK(entry_pc_activate_handler), cw);
	gtk_entry_set_max_length(GTK_ENTRY(cw->pc_entry), 4);
	gtk_entry_set_width_chars(GTK_ENTRY(cw->pc_entry), 4);

	align = gtk_alignment_new(0, 0, 0, 0);
	gtk_container_add(GTK_CONTAINER(align), cw->pc_entry);
	gtk_table_attach_defaults(GTK_TABLE(cw), align, 1, 2, 7, 8);

	/* The clock cycle labels */
	clk_label = gtk_label_new_with_mnemonic("Cycles:");
	gtk_widget_modify_font(GTK_WIDGET(clk_label), pango_font_description_from_string("monospace"));
	gtk_table_attach_defaults(GTK_TABLE(cw), clk_label, 0, 2, 8, 9);

	cw->cycles_label = gtk_label_new_with_mnemonic("0");
	gtk_widget_modify_font(GTK_WIDGET(cw->cycles_label), pango_font_description_from_string("monospace"));
	gtk_table_attach_defaults(GTK_TABLE(cw), cw->cycles_label, 0, 2, 9, 10);

	cpu_widget_update(cw);
}

GtkWidget *cpu_widget_new()
{
	return GTK_WIDGET(g_object_new(CPU_WIDGET_TYPE, NULL));
}

void cpu_widget_update(CpuWidget *cw)
{
	char *str;

	if(!cw->cpu)
	{
		str = g_strdup_printf("NA");
		gtk_entry_set_text(GTK_ENTRY(cw->a_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->a_entry), FALSE);
		gtk_entry_set_text(GTK_ENTRY(cw->x_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->x_entry), FALSE);
		gtk_entry_set_text(GTK_ENTRY(cw->y_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->y_entry), FALSE);
		gtk_entry_set_text(GTK_ENTRY(cw->p_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->p_entry), FALSE);
		gtk_entry_set_text(GTK_ENTRY(cw->sp_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->sp_entry), FALSE);
		gtk_entry_set_text(GTK_ENTRY(cw->pc_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->pc_entry), FALSE);
		g_free(str);
		return;
	}

	if(cw->read_a_callback)
	{
		str = g_strdup_printf("%02X", cw->read_a_callback(cw->cpu));
		gtk_entry_set_text(GTK_ENTRY(cw->a_entry), str);
		if(cw->editable)
			gtk_widget_set_sensitive(GTK_WIDGET(cw->a_entry), TRUE);
		else
			gtk_widget_set_sensitive(GTK_WIDGET(cw->a_entry), FALSE);
		g_free(str);
	} else
	{
		str = g_strdup_printf("NA");
		gtk_entry_set_text(GTK_ENTRY(cw->a_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->a_entry), FALSE);
		g_free(str);
	}


	if(cw->read_x_callback)
	{
		str = g_strdup_printf("%02X", cw->read_x_callback(cw->cpu));
		gtk_entry_set_text(GTK_ENTRY(cw->x_entry), str);
		if(cw->editable)
			gtk_widget_set_sensitive(GTK_WIDGET(cw->x_entry), TRUE);
		else
			gtk_widget_set_sensitive(GTK_WIDGET(cw->x_entry), FALSE);
		g_free(str);
	} else {
		str = g_strdup_printf("NA");
		gtk_entry_set_text(GTK_ENTRY(cw->x_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->x_entry), FALSE);
		g_free(str);
	}

	if(cw->read_y_callback)
	{
		str = g_strdup_printf("%02X", cw->read_y_callback(cw->cpu));
		gtk_entry_set_text(GTK_ENTRY(cw->y_entry), str);
		if(cw->editable)
			gtk_widget_set_sensitive(GTK_WIDGET(cw->y_entry), TRUE);
		else
			gtk_widget_set_sensitive(GTK_WIDGET(cw->y_entry), FALSE);
		g_free(str);
	} else {
		str = g_strdup_printf("NA");
		gtk_entry_set_text(GTK_ENTRY(cw->y_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->y_entry), FALSE);
		g_free(str);
	}

	if(cw->read_p_callback)
	{
		byte val = cw->read_p_callback(cw->cpu);
		str = g_strdup_printf("%02X", val);
		gtk_entry_set_text(GTK_ENTRY(cw->p_entry), str);
		if(cw->editable)
			gtk_widget_set_sensitive(GTK_WIDGET(cw->p_entry), TRUE);
		else
			gtk_widget_set_sensitive(GTK_WIDGET(cw->p_entry), FALSE);
		g_free(str);

		/* Update the flag bit string */
		str = g_strdup_printf("%d%d1%d%d%d%d%d",
					(val & 0x80) ? 1 : 0,
					(val & 0x40) ? 1 : 0,
					(val & 0x10) ? 1 : 0,
					(val & 0x08) ? 1 : 0,
					(val & 0x04) ? 1 : 0,
					(val & 0x02) ? 1 : 0,
					(val & 0x01) ? 1 : 0);
		gtk_label_set_text(GTK_LABEL(cw->flag_bits), str);
		g_free(str);
		
	} else {
		str = g_strdup_printf("NA");
		gtk_entry_set_text(GTK_ENTRY(cw->p_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->p_entry), FALSE);
		g_free(str);
	}

	if(cw->read_sp_callback)
	{
		str = g_strdup_printf("%02X", cw->read_sp_callback(cw->cpu));
		gtk_entry_set_text(GTK_ENTRY(cw->sp_entry), str);
		if(cw->editable)
			gtk_widget_set_sensitive(GTK_WIDGET(cw->sp_entry), TRUE);
		else
			gtk_widget_set_sensitive(GTK_WIDGET(cw->sp_entry), FALSE);
		g_free(str);
	} else {
		str = g_strdup_printf("NA");
		gtk_entry_set_text(GTK_ENTRY(cw->sp_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->sp_entry), FALSE);
		g_free(str);
	}

	if(cw->read_pc_callback)
	{
		str = g_strdup_printf("%04X", cw->read_pc_callback(cw->cpu));
		gtk_entry_set_text(GTK_ENTRY(cw->pc_entry), str);
		if(cw->editable)
			gtk_widget_set_sensitive(GTK_WIDGET(cw->pc_entry), TRUE);
		else
			gtk_widget_set_sensitive(GTK_WIDGET(cw->pc_entry), FALSE);
		g_free(str);
	} else {
		str = g_strdup_printf("NA");
		gtk_entry_set_text(GTK_ENTRY(cw->pc_entry), str);
		gtk_widget_set_sensitive(GTK_WIDGET(cw->pc_entry), FALSE);
		g_free(str);
	}

	if(cw->read_cycles_callback)
	{
		str = g_strdup_printf("%lu", (gulong) cw->read_cycles_callback(cw->cpu));
		gtk_label_set_text(GTK_LABEL(cw->cycles_label), str);
		g_free(str);
	} else {
		str = g_strdup_printf("NA");
		gtk_label_set_text(GTK_LABEL(cw->cycles_label), str);
		g_free(str);
	}
}

gboolean entry_a_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw)
{
	(void) entry;
    GdkEventKey *key = (GdkEventKey *) ev;
    guint keyval = key->keyval;

    switch(keyval)
    {
        /* Some allowed keys */
        case GDK_KEY_Left:
        case GDK_KEY_Right:
        case GDK_KEY_Return:
        case GDK_KEY_BackSpace:
        case GDK_KEY_Delete:
            return FALSE;

        /* Consider these keys as if the entry was updated */
        case GDK_KEY_Up:
        case GDK_KEY_Down:
        case GDK_KEY_Tab:
        case GDK_KEY_ISO_Left_Tab:
            entry_a_activate_handler(entry, cw);

        /* Restore entry's value (yeah, yeah, I'm lazy, the other ones' too) */
        case GDK_KEY_Escape:
            cpu_widget_update(cw);
            return FALSE;
    }

    /* Key must be hexadecimal digit */
    if(!g_ascii_isxdigit(keyval))
    {
        return TRUE;
    }

    /* We should really never end up here */
    return FALSE;
}

gboolean entry_x_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw)
{
	(void) entry;
    GdkEventKey *key = (GdkEventKey *) ev;
    guint keyval = key->keyval;

    switch(keyval)
    {
        /* Some allowed keys */
        case GDK_KEY_Left:
        case GDK_KEY_Right:
        case GDK_KEY_Return:
        case GDK_KEY_BackSpace:
        case GDK_KEY_Delete:
            return FALSE;

        /* Consider these keys as if the entry was updated */
        case GDK_KEY_Up:
        case GDK_KEY_Down:
        case GDK_KEY_Tab:
        case GDK_KEY_ISO_Left_Tab:
            entry_x_activate_handler(entry, cw);

        /* Restore entry's value (yeah, yeah, I'm lazy, the other ones' too) */
        case GDK_KEY_Escape:
            cpu_widget_update(cw);
            return FALSE;
    }

    /* Key must be hexadecimal digit */
    if(!g_ascii_isxdigit(keyval))
    {
        return TRUE;
    }

    /* We should really never end up here */
    return FALSE;
}

gboolean entry_y_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw)
{
	(void) entry;
    GdkEventKey *key = (GdkEventKey *) ev;
    guint keyval = key->keyval;

    switch(keyval)
    {
        /* Some allowed keys */
        case GDK_KEY_Left:
        case GDK_KEY_Right:
        case GDK_KEY_Return:
        case GDK_KEY_BackSpace:
        case GDK_KEY_Delete:
            return FALSE;

        /* Consider these keys as if the entry was updated */
        case GDK_KEY_Up:
        case GDK_KEY_Down:
        case GDK_KEY_Tab:
        case GDK_KEY_ISO_Left_Tab:
            entry_y_activate_handler(entry, cw);

        /* Restore entry's value (yeah, yeah, I'm lazy, the other ones' too) */
        case GDK_KEY_Escape:
            cpu_widget_update(cw);
            return FALSE;
    }

    /* Key must be hexadecimal digit */
    if(!g_ascii_isxdigit(keyval))
    {
        return TRUE;
    }

    /* We should really never end up here */
    return FALSE;
}

gboolean entry_p_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw)
{
	(void) entry;
    GdkEventKey *key = (GdkEventKey *) ev;
    guint keyval = key->keyval;

    switch(keyval)
    {
        /* Some allowed keys */
        case GDK_KEY_Left:
        case GDK_KEY_Right:
        case GDK_KEY_Return:
        case GDK_KEY_BackSpace:
        case GDK_KEY_Delete:
            return FALSE;

        /* Consider these keys as if the entry was updated */
        case GDK_KEY_Up:
        case GDK_KEY_Down:
        case GDK_KEY_Tab:
        case GDK_KEY_ISO_Left_Tab:
            entry_p_activate_handler(entry, cw);

        /* Restore entry's value (yeah, yeah, I'm lazy, the other ones' too) */
        case GDK_KEY_Escape:
            cpu_widget_update(cw);
            return FALSE;
    }

    /* Key must be hexadecimal digit */
    if(!g_ascii_isxdigit(keyval))
    {
        return TRUE;
    }

    /* We should really never end up here */
    return FALSE;
}

gboolean entry_sp_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw)
{
	(void) entry;
    GdkEventKey *key = (GdkEventKey *) ev;
    guint keyval = key->keyval;

    switch(keyval)
    {
        /* Some allowed keys */
        case GDK_KEY_Left:
        case GDK_KEY_Right:
        case GDK_KEY_Return:
        case GDK_KEY_BackSpace:
        case GDK_KEY_Delete:
            return FALSE;

        /* Consider these keys as if the entry was updated */
        case GDK_KEY_Up:
        case GDK_KEY_Down:
        case GDK_KEY_Tab:
        case GDK_KEY_ISO_Left_Tab:
            entry_sp_activate_handler(entry, cw);

        /* Restore entry's value (yeah, yeah, I'm lazy, the other ones' too) */
        case GDK_KEY_Escape:
            cpu_widget_update(cw);
            return FALSE;
    }

    /* Key must be hexadecimal digit */
    if(!g_ascii_isxdigit(keyval))
    {
        return TRUE;
    }

    /* We should really never end up here */
    return FALSE;
}

gboolean entry_pc_key_handler(GtkEntry *entry, GdkEvent *ev, CpuWidget *cw)
{
	(void) entry;
    GdkEventKey *key = (GdkEventKey *) ev;
    guint keyval = key->keyval;

    switch(keyval)
    {
        /* Some allowed keys */
        case GDK_KEY_Left:
        case GDK_KEY_Right:
        case GDK_KEY_Return:
        case GDK_KEY_BackSpace:
        case GDK_KEY_Delete:
            return FALSE;

        /* Consider these keys as if the entry was updated */
        case GDK_KEY_Up:
        case GDK_KEY_Down:
        case GDK_KEY_Tab:
        case GDK_KEY_ISO_Left_Tab:
            entry_pc_activate_handler(entry, cw);

        /* Restore entry's value (yeah, yeah, I'm lazy, the other ones' too) */
        case GDK_KEY_Escape:
            cpu_widget_update(cw);
            return FALSE;
    }

    /* Key must be hexadecimal digit */
    if(!g_ascii_isxdigit(keyval))
    {
        return TRUE;
    }

    /* We should really never end up here */
    return FALSE;
}

void entry_a_activate_handler(GtkEntry *entry, CpuWidget *cw)
{

	guchar val;

	if(!cw->cpu)
		return;

	if(cw->write_a_callback)
	{
		val = g_ascii_strtoll(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 16);
		cw->write_a_callback(cw->cpu, val);
	}
}

void entry_x_activate_handler(GtkEntry *entry, CpuWidget *cw)
{

	guchar val;

	if(!cw->cpu)
		return;

	if(cw->write_x_callback)
	{
		val = g_ascii_strtoll(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 16);
		cw->write_x_callback(cw->cpu, val);
	}
}

void entry_y_activate_handler(GtkEntry *entry, CpuWidget *cw)
{

	guchar val;

	if(!cw->cpu)
		return;

	if(cw->write_y_callback)
	{
		val = g_ascii_strtoll(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 16);
		cw->write_y_callback(cw->cpu, val);
	}
}

void entry_p_activate_handler(GtkEntry *entry, CpuWidget *cw)
{

	guchar val;

	if(!cw->cpu)
		return;

	if(cw->write_p_callback)
	{
		val = g_ascii_strtoll(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 16);
		cw->write_p_callback(cw->cpu, val);
	}
}

void entry_sp_activate_handler(GtkEntry *entry, CpuWidget *cw)
{

	guchar val;

	if(!cw->cpu)
		return;

	if(cw->write_sp_callback)
	{
		val = g_ascii_strtoll(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 16);
		cw->write_sp_callback(cw->cpu, val);
		g_signal_emit_by_name(cw, "update_gui");
	}
}

void entry_pc_activate_handler(GtkEntry *entry, CpuWidget *cw)
{

	guint val;

	if(!cw->cpu)
		return;

	if(cw->write_pc_callback)
	{
		val = g_ascii_strtoll(gtk_entry_get_text(GTK_ENTRY(entry)), NULL, 16);
		cw->write_pc_callback(cw->cpu, val);
		g_signal_emit_by_name(cw, "update_gui");
	}
}

