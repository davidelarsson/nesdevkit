#ifndef EXECUTE_WIDGET_H
#define EXECUTE_WIDGET_H

#include <glib.h>
#include "../core/nes.h"

/* Keep C++ programs that use this header file happy */
G_BEGIN_DECLS

/* Standard cast macros */
#define EXECUTE_WIDGET_TYPE            (execute_widget_get_type())
#define EXECUTE_WIDGET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), EXECUTE_WIDGET_TYPE, ExecuteWidget))
#define EXECUTE_WIDGET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), EXECUTE_WIDGET_TYPE, ExecuteWidgetClass))
#define IS_EXECUTE_WIDGET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EXECUTE_WIDGET_TYPE))
#define IS_EXECUTE_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), EXECUTE_WIDGET_TYPE))

#define EXECUTE_WIDGET_ROWS 1
#define EXECUTE_WIDGET_COLS 3

/* FIXME: WTF is this? */
typedef struct _ExecuteWidget		ExecuteWidget;
typedef struct _ExecuteWidgetClass	ExecuteWidgetClass;

/* Object structure, one new instance of this structure is created for each
   widget created of the ExecuteWidget class */
struct _ExecuteWidget
{
	GtkTable 	table;
	GtkWidget	*start_button;
	GtkWidget	*stop_button;
	GtkWidget	*step_button;
	GtkWidget	*execute_button;
	nes_t		*nes;
	gboolean	running;
	GThread		*thread;
	void		(* execute_callback)(nes_t *nes);
	void		(* step_callback)(nes_t *nes);
	void		(* debug_disable_callback)();
};

/* Class structure, information here is common to all widgets of the class */
struct _ExecuteWidgetClass
{
	GtkTableClass	parent_class;	/* Parent */
	/* Function pointer to... FIXME: what, exactly? */
	void 			(* execute_widget)(ExecuteWidget *ew);
};

/* Mandatory function for a custom widget */
GType		execute_widget_get_type();

/* Not mandatory, but very useful function */
GtkWidget	*execute_widget_new();

/* Keep C++ programs that use this header file happy */
G_END_DECLS

#endif /* EXECUTE_WIDGET_H */
