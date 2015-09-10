#ifndef GTK_H
#define GTK_H

#include <gtk/gtk.h>
#include "../core/defines.h"
#include "../core/nes.h"

void open_gtk_window(int argc, char *argv[]);
void destroy(GtkWidget *w, gpointer *data);
void file_selected(GtkWidget *w, gpointer fc);
int main(int argc, char *argv[]);
static void application_start_suggest_open(GApplication *app, gpointer data);
static void application_start_open(GApplication *app,
		GFile **files, gint no_files, gchar *hint);

/** Convenience function to get the main window's table from the main window */
GtkWidget *get_main_table(GtkWidget *win);

/* File has been picked, try to parse it */
void file_loaded();

/* A NES ROM has been successfully parsed! */
void start_emulation();

/* Start here */


/* NES GUI context */
typedef struct emu_context_s {
	GtkWidget *main_window;
	GtkWidget *main_table; /* FIXME: Is this ever used after initialization? */
	GtkWidget *memory_widget;
	GtkWidget *stack_widget;
	GtkWidget *cpu_widget;
	GtkWidget *ppu_widget;
	GtkWidget *disasm_widget;
	GtkWidget *execute_widget;
	GApplication *app;
	nes_t *nes;
} emu_context_t;

emu_context_t *nes_gui_init(GApplication *app);

int main_window_init(emu_context_t *emu_context);

#endif /* GTK_H */
