#ifndef __STACK_WIDGET_H__
#define __STACK_WIDGET_H__

G_BEGIN_DECLS
#include <glib.h>
#include "../core/cpu_mem.h"
#include "../core/cpu.h"

/* GTK required */
#define STACK_WIDGET_TYPE (stack_widget_get_type())
#define STACK_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), STACK_WIDGET_TYPE, StackWidget))
#define STACK_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), STACK_WIDGET_TYPE, StackWidgetClass))
#define IS_STACK_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), STACK_WIDGET_TYPE))
#define IS_STACK_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), STACK_WIDGET_TYPE))

typedef struct _StackWidget StackWidget;
typedef struct _StackWidgetClass StackWidgetClass;

#define STACK_WIDGET_ROWS		16

struct _StackWidget
{
	GtkTable  table;	/* parent */
	GtkWidget *top_box;
	GtkWidget *entries[STACK_WIDGET_ROWS];
	GtkWidget *top_location;
	GtkWidget *left_locations[STACK_WIDGET_ROWS];
	cpu_t	  *cpu;
	cpu_ram_t *ram;
	uint8_t (* ram_read_callback)(cpu_ram_t *ram, word address);
	uint8_t	(* read_sp_callback)(cpu_t *cpu);
	word      sp;
};

struct _StackWidgetClass
{
	GtkTableClass parent_class;
	void (* stack_widget) (StackWidget *sw);
};

/**
 * Required by GTK
 */ 
GType stack_widget_get_type();

/**
 * Create a new stack widget
 */
GtkWidget *stack_widget_new();

/**
 * Updates the values in each entry.
 */
void stack_widget_update(StackWidget *sw);

G_END_DECLS
#endif /* __STACK_WIDGET_H__ */
