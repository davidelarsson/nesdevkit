#include <gtk/gtk.h>
#include <gtk/gtksignal.h>
#include <gtk/gtktable.h>
#include <gtk/gtkentry.h>
#include <gdk/gdkkeysyms.h>
#include "execute_widget.h"

/**
 * Property constants.
 */
enum {
	PROP_0,
	PROP_NES,
	PROP_EXECUTE_CALLBACK,
	PROP_STEP_CALLBACK,
	PROP_DEBUG_DISABLE_CALLBACK,
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

static guint execute_widget_signals[SIGNAL_LAST] = { 0 };

/**
 * Set properties of theclass. This is mandatory if properties are
 * used.
 */
static void execute_widget_set_property(GObject *obj,
							guint prop_id,
							const GValue *val,
							GParamSpec *pspec);

/**
 * Mandatory function. When first called, it tells GLib of 
 * the new class, and gets a unique ID that identifies the class.
 *
 * Subsequent calls just returns the ID of the class.
 */
GType execute_widget_get_type();

/**
 * Mandatory function. Usually sets the fields of the structure to default values.
 * For our composite widget, we also create our component widgets.
 */
static void execute_widget_init(ExecuteWidget *ew);

/**
 * Sets up the class structure and also the signals for the class.
 * This function is not mandatory if no signals are used. Then a NULL must be
 * provided in the class_init field of the GTypeInfo struct used by 
 * g_type_register_static() in execute_widget_get_type() below.
 */
static void execute_widget_class_init(ExecuteWidget *ew);

/**
 * Not mandatory, but very useful. Creates and returns a new widget.
 */
GtkWidget *execute_widget_new();

void on_step_button_clicked(GtkButton *widget, ExecuteWidget *ew);
void on_execute_button_clicked(GtkButton *widget, ExecuteWidget *ew);
void on_start_button_clicked(GtkButton *widget, ExecuteWidget *ew);
void on_stop_button_clicked(GtkButton *widget, ExecuteWidget *ew);

gpointer execute_thread(ExecuteWidget *ew);

GType execute_widget_get_type()
{
	static GType ew_type = 0;

	if(!ew_type)
	{
		const GTypeInfo ew_info=
			{
				sizeof(ExecuteWidgetClass), /* class_size */
				NULL,						/* base_init */
				NULL,						/* base_finalize */
				(GClassInitFunc) execute_widget_class_init, /* class_init */
				NULL,						/* class_finalize */
				NULL,						/* class_data */
				sizeof(ExecuteWidget),		/* instance_size */
				0,							/* n_preallocs */
				(GInstanceInitFunc) execute_widget_init, /* instance_init */
				NULL						/* value_table */
			};
		ew_type = g_type_register_static(GTK_TYPE_TABLE, /* parent_type */
			"execute_widget",							/* type_name */
			&ew_info,									/* info */
			0);											/* flags */
	}
	return ew_type;
}

static void execute_widget_class_init(ExecuteWidget *ew)
{

	GObjectClass *c = G_OBJECT_CLASS(ew);

	c->set_property = execute_widget_set_property;

	execute_widget_signals[SIGNAL_UPDATE_GUI] = g_signal_new("update_gui",
													G_TYPE_FROM_CLASS(c),
													G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
													G_STRUCT_OFFSET(ExecuteWidgetClass, execute_widget),
													NULL,
													NULL,
													g_cclosure_marshal_VOID__VOID,
													G_TYPE_NONE,
													0);

	execute_widget_signals[SIGNAL_UPDATE_GUI] = g_signal_new("disable_gui",
													G_TYPE_FROM_CLASS(c),
													G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
													G_STRUCT_OFFSET(ExecuteWidgetClass, execute_widget),
													NULL,
													NULL,
													g_cclosure_marshal_VOID__VOID,
													G_TYPE_NONE,
													1,
													G_TYPE_BOOLEAN);
	g_object_class_install_property(c,
						PROP_NES,
						g_param_spec_pointer("nes",
							"NES",
							"Pointer to a NES",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_EXECUTE_CALLBACK,
						g_param_spec_pointer("execute_callback",
							"Execute callback",
							"Pointer to a function that executes one instruction",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_STEP_CALLBACK,
						g_param_spec_pointer("step_callback",
							"Step callback",
							"Pointer to a function that steps the emulator",
							G_PARAM_WRITABLE));

	g_object_class_install_property(c,
						PROP_DEBUG_DISABLE_CALLBACK,
						g_param_spec_pointer("debugger_disable",
							"Disable the debugger",
							"Pointer to a function that enables or disables the debugger",
							G_PARAM_WRITABLE));

}

void execute_widget_set_property(GObject *obj, guint prop_id, const GValue *val, GParamSpec *pspec)
{
	ExecuteWidget *ew = EXECUTE_WIDGET(obj);

	switch(prop_id)
	{
		case PROP_NES:
			ew->nes = (nes_t *) g_value_get_pointer(val);
			break;
		case PROP_EXECUTE_CALLBACK:
			ew->execute_callback = ((void (*)(nes_t *)) g_value_get_pointer(val));
			break;
		case PROP_STEP_CALLBACK:
			ew->step_callback = ((void (*)(nes_t *)) g_value_get_pointer(val));
			break;
		case PROP_DEBUG_DISABLE_CALLBACK:
			ew->debug_disable_callback = ((void (*)()) g_value_get_pointer(val));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void execute_widget_init(ExecuteWidget *ew)
{

	ew->nes = NULL;
	ew->execute_callback = NULL;
	ew->step_callback = NULL;
	ew->debug_disable_callback = NULL;
	ew->running = FALSE;
	ew->thread = NULL;

	gtk_table_resize(GTK_TABLE(ew), EXECUTE_WIDGET_ROWS, EXECUTE_WIDGET_COLS);
	gtk_table_set_homogeneous(GTK_TABLE(ew), TRUE);

	ew->step_button = gtk_button_new_with_label("Step");
	g_signal_connect(ew->step_button, "clicked", G_CALLBACK(on_step_button_clicked), ew);
	gtk_table_attach_defaults(GTK_TABLE(ew), ew->step_button, 0, 1, 0, 1);

	ew->execute_button = gtk_button_new_with_label("Execute");
	g_signal_connect(ew->execute_button, "clicked", G_CALLBACK(on_execute_button_clicked), ew);
	gtk_table_attach_defaults(GTK_TABLE(ew), ew->execute_button, 1, 2, 0, 1);

	ew->start_button = gtk_button_new_with_label("Start");
	g_signal_connect(ew->start_button, "clicked", G_CALLBACK(on_start_button_clicked), ew);
	gtk_table_attach_defaults(GTK_TABLE(ew), ew->start_button, 2, 3, 0, 1);

	ew->stop_button = gtk_button_new_with_label("Stop");
	g_signal_connect(ew->stop_button, "clicked", G_CALLBACK(on_stop_button_clicked), ew);
	gtk_widget_set_sensitive(GTK_WIDGET(ew->stop_button), FALSE);
	gtk_table_attach_defaults(GTK_TABLE(ew), ew->stop_button, 3, 4, 0, 1);
}

GtkWidget *execute_widget_new()
{
	return GTK_WIDGET(g_object_new(EXECUTE_WIDGET_TYPE, NULL));
}

void on_step_button_clicked(GtkButton *button, ExecuteWidget *ew)
{
	(void) button;

	g_print("Step!\n");

	if(!ew->nes)
	{
		g_print("No NES loaded!\n");
		return;
	}
	if(!ew->step_callback)
	{
		g_print("No step callback loaded!\n");
		return;
	}
}

void on_execute_button_clicked(GtkButton *button, ExecuteWidget *ew)
{
	(void) button;

	g_print("Execute!\n");

	if(!ew->nes)
	{
		g_print("No NES loaded!\n");
		return;
	}
	if(!ew->execute_callback)
	{
		g_print("No execute callback loaded!\n");
		return;
	}
	ew->execute_callback(ew->nes);
	g_signal_emit_by_name(ew, "update_gui");
}

void on_start_button_clicked(GtkButton *button, ExecuteWidget *ew)
{
	(void) button;

	if(!ew->nes)
	{
		g_print("No NES loaded!\n");
		return;
	}
	if(!ew->execute_callback)
	{
		g_print("No execute callback loaded!\n");
		return;
	}

	ew->running = TRUE;
	gtk_widget_set_sensitive(GTK_WIDGET(ew->start_button), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(ew->stop_button), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(ew->execute_button), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(ew->step_button), FALSE);
	if(!ew->thread)
		ew->thread = g_thread_new(NULL, (GThreadFunc) execute_thread, ew);
	g_signal_emit_by_name(ew, "disable_gui", TRUE);

}

void on_stop_button_clicked(GtkButton *button, ExecuteWidget *ew)
{
	(void) button;

	ew->running = FALSE;

	if(!ew->nes)
	{
		g_print("No NES loaded!\n");
		return;
	}
	if(!ew->execute_callback)
	{
		g_print("No execute callback loaded!\n");
		return;
	}
	gtk_widget_set_sensitive(GTK_WIDGET(ew->start_button), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(ew->stop_button), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(ew->execute_button), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(ew->step_button), TRUE);
	g_signal_emit_by_name(ew, "disable_gui", FALSE);
}

gboolean update_widgets(ExecuteWidget *ew)
{
	g_signal_emit_by_name(ew, "update_gui");
	return G_SOURCE_REMOVE;
}

gpointer execute_thread(ExecuteWidget *ew)
{
	gboolean stop = FALSE;
	guint delay_widget_updates = 0;

	while(!stop)
	{
		if(ew->running)
		{
			ew->execute_callback(ew->nes);

			if(delay_widget_updates++ == 500000)
			{
				g_idle_add((GSourceFunc) update_widgets, ew);
				delay_widget_updates = 0;
			}
			continue;
		}
		usleep(1000);
	}

	return NULL;
}

