#ifndef CPU_WIDGET_H
#define CPU_WIDGET_H

#include <glib.h>
#include "../core/cpu.h"

/* Keep C++ programs that use this header file happy */
G_BEGIN_DECLS

/* Standard cast macros */
#define CPU_WIDGET_TYPE            (cpu_widget_get_type())
#define CPU_WIDGET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), CPU_WIDGET_TYPE, CpuWidget))
#define CPU_WIDGET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CPU_WIDGET_TYPE, CpuWidgetClass))
#define IS_CPU_WIDGET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CPU_WIDGET_TYPE))
#define IS_CPU_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CPU_WIDGET_TYPE))

#define CPU_WIDGET_ROWS 8
#define CPU_WIDGET_COLS 4

typedef struct _CpuWidget		CpuWidget;
typedef struct _CpuWidgetClass	CpuWidgetClass;

/* Object structure, one new instance of this structure is created for each
   widget created of the CpuWidget class */
struct _CpuWidget
{
	GtkTable 	table; 		/* Parent */

	gboolean	editable;
	GtkWidget	*flag_bits;		/* 00100000 */
	GtkWidget	*p_entry;
	GtkWidget	*a_entry;
	GtkWidget	*x_entry;
	GtkWidget	*y_entry;
	GtkWidget	*sp_entry;
	GtkWidget	*pc_entry;
	GtkWidget	*cycles_label;

	/* The NES CPU */
	cpu_t		*cpu;

	/* Callbacks */
	byte		(* read_a_callback)(cpu_t *cpu);
	byte		(* read_x_callback)(cpu_t *cpu);
	byte		(* read_y_callback)(cpu_t *cpu);
	byte		(* read_sp_callback)(cpu_t *cpu);
	byte		(* read_p_callback)(cpu_t *cpu);
	word		(* read_pc_callback)(cpu_t *cpu);
	gulong		(* read_cycles_callback)(cpu_t *cpu);

	void		(* write_a_callback)(cpu_t *cpu, byte val);
	void		(* write_x_callback)(cpu_t *cpu, byte val);
	void		(* write_y_callback)(cpu_t *cpu, byte val);
	void		(* write_sp_callback)(cpu_t *cpu, byte val);
	void		(* write_p_callback)(cpu_t *cpu, byte val);
	void		(* write_pc_callback)(cpu_t *cpu, word val);

};

/* Class structure, information here is common to all widgets of the class */
struct _CpuWidgetClass
{
	GtkTableClass	parent_class;	/* Parent */
	void 			(* cpu_widget)(CpuWidget *ew);
};

/* Not mandatory, but very useful function */
GtkWidget	*cpu_widget_new();

void cpu_widget_update(CpuWidget *cw);

/* Keep C++ programs that use this header file happy */
G_END_DECLS

#endif /* CPU_WIDGET_H */
