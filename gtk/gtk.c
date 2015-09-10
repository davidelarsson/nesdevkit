#include <gtk/gtk.h>
#include <gio/gio.h>
#include <glib.h>
#include <stdlib.h>
#include "gtk.h"
#include "memory_widget.h"
#include "stack_widget.h"
#include "cpu_widget.h"
#include "disasm_widget.h"
#include "execute_widget.h"
#include "ppu_widget.h"
#include "../core/cpu_mem.h"
#include "../core/ppu.h"
#include "../core/disasm.h"
#include "../core/asm.h"


static void init_memory_widget(emu_context_t *emu_context);
static void init_stack_widget(emu_context_t *emu_context);
static void init_cpu_widget(emu_context_t *emu_context);
static void init_disasm_widget(emu_context_t *emu_context);
static void init_execute_widget(emu_context_t *emu_context);
static void init_ppu_widget(emu_context_t *emu_context);
static void debug_button_clicked(GtkButton *button, emu_context_t *emu_context);
static void cb_update_gui(GtkWidget *w, emu_context_t *emu_context);
static void cb_disable_gui(GtkButton *button, gboolean disable, emu_context_t *emu_context);
static GtkWidget *init_menu_bar(GtkWidget *main_window);

static void update_all_widgets(emu_context_t *emu_context);

void load_file(GtkWidget *w, gpointer emu_context)
{
	(void) w;

	g_print("Loading file...\n");

	GtkWidget *dialog = gtk_file_chooser_dialog_new("Select NES ROM...",
		NULL, GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		file_selected(dialog, emu_context);
	
	gtk_widget_destroy(dialog);
}

void destroy(GtkWidget *w, gpointer *data)
{
	(void) w;
	(void) data;
	g_print("Destroyed!\n");
	gtk_main_quit();
}

void file_selected(GtkWidget *fc, gpointer emu_context)
{
	(void) emu_context;
	char *filename = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(fc));
	g_print("File selected is: %s\n", filename);

	/* Remove everything in the main table */
	// ... Somehow.

	// Parse the NES file and start the emulation... somehow.
}

emu_context_t *emu_context_init(GApplication *app)
{
	g_print("Enabling GUI...\n");

	emu_context_t *emu_context = malloc(sizeof(emu_context_t));

	/* Not sure this is ever going to be needed */
	emu_context->app = app;

	/* Main window */
	if(0 != main_window_init(emu_context))
	{
		g_print("Could not init main window!\n");
		free(emu_context);
		return NULL;
	}

	return emu_context;
}

GtkWidget *init_menu_bar(GtkWidget *main_window)
{
	GtkWidget *menu_bar = NULL;
	GtkWidget *file_menu = NULL;
	GtkWidget *open_item = NULL;
	GtkWidget *file_item = NULL;
	GtkWidget *quit_item = NULL;

	menu_bar = gtk_menu_bar_new();
	file_menu = gtk_menu_new();

	file_item = gtk_menu_item_new_with_label("File"); 
	open_item = gtk_menu_item_new_with_label("Open");
	quit_item = gtk_menu_item_new_with_label("Quit");

	g_signal_connect(open_item, "activate", G_CALLBACK(load_file), NULL);
	g_signal_connect_swapped(quit_item, "activate", G_CALLBACK(destroy), main_window);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);

	return menu_bar;

}

int main_window_init(emu_context_t *emu_context)
{
	
	GtkWidget *vbox = NULL;
	GtkWidget *menu_bar = NULL;
	GtkWidget *debug_button;
	GtkWidget *frame;
    GtkAttachOptions attach_options;

	/* This applies to all widgets */
    attach_options = GTK_SHRINK;

	/* A box that contains the menu bar and the main table */
	vbox = gtk_vbox_new(FALSE, 0);

	/* Main window */
	emu_context->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect_swapped(emu_context->main_window, "destroy", G_CALLBACK(destroy), emu_context->main_window);
	/* This is not needed, since we use g_application_run ?? */
//	g_signal_connect(win, "delete-event", G_CALLBACK(delete_event), NULL);
	gtk_window_set_title(GTK_WINDOW(emu_context->main_window), "NES SDK");
	gtk_widget_set_size_request(emu_context->main_window, 1300, 700);

	/* The main table */
	emu_context->main_table = gtk_table_new(4, 3, FALSE);

	/* Menu bar */
	menu_bar = init_menu_bar(emu_context->main_window);

	/* Memory widget */
	frame = gtk_frame_new("CPU memory");
	emu_context->memory_widget = memory_widget_new();
	g_signal_connect(G_OBJECT(emu_context->memory_widget), "update_gui", G_CALLBACK(cb_update_gui), emu_context);
	gtk_container_add(GTK_CONTAINER(frame), emu_context->memory_widget);
	gtk_table_attach(GTK_TABLE(emu_context->main_table), frame, 0, 1, 0, 1, attach_options, attach_options, 5, 5);

	/* Stack widget */
	frame = gtk_frame_new("Stack");
	emu_context->stack_widget = stack_widget_new();
	gtk_container_add(GTK_CONTAINER(frame), emu_context->stack_widget);
	gtk_table_attach(GTK_TABLE(emu_context->main_table), frame, 1, 2, 0, 1, attach_options, attach_options, 5, 5);

	/* Cpu widget */
	frame = gtk_frame_new("CPU");
	emu_context->cpu_widget = cpu_widget_new();
	g_signal_connect(G_OBJECT(emu_context->cpu_widget), "update_gui", G_CALLBACK(cb_update_gui), emu_context);
	gtk_container_add(GTK_CONTAINER(frame), emu_context->cpu_widget);
	gtk_table_attach(GTK_TABLE(emu_context->main_table), frame, 2, 3, 0, 1, attach_options, attach_options, 5, 5);

	/* PPU widget */
	frame = gtk_frame_new("PPU");
	emu_context->ppu_widget = ppu_widget_new();
//	g_signal_connect(G_OBJECT(emu_context->cpu_widget), "update_gui", G_CALLBACK(cb_update_gui), emu_context);
	gtk_container_add(GTK_CONTAINER(frame), emu_context->ppu_widget);
	gtk_table_attach(GTK_TABLE(emu_context->main_table), frame, 3, 4, 0, 1, attach_options, attach_options, 5, 5);

	/* Disasm widget */
	frame = gtk_frame_new("Disassembly");
	emu_context->disasm_widget = disasm_widget_new();
	gtk_container_add(GTK_CONTAINER(frame), emu_context->disasm_widget);
	gtk_table_attach(GTK_TABLE(emu_context->main_table), frame, 0, 1, 1, 2, attach_options, attach_options, 0, 0);

	/* Execute widget */
	frame = gtk_frame_new("Execution");
	emu_context->execute_widget = execute_widget_new();
	g_signal_connect(G_OBJECT(emu_context->execute_widget), "update_gui", G_CALLBACK(cb_update_gui), emu_context);
	g_signal_connect(G_OBJECT(emu_context->execute_widget), "disable_gui", G_CALLBACK(cb_disable_gui), emu_context);
	gtk_container_add(GTK_CONTAINER(frame), emu_context->execute_widget);
//	gtk_table_attach_defaults(GTK_TABLE(emu_context->main_table), emu_context->execute_widget, 0, 1, 3, 4);
	gtk_table_attach(GTK_TABLE(emu_context->main_table), frame, 2, 3, 1, 2, attach_options, attach_options, 5, 5);

	/* Debug button */
	frame = gtk_frame_new("Stuff to be removed");
	debug_button = gtk_button_new_with_label("Debug");
	g_signal_connect(debug_button, "clicked", G_CALLBACK(debug_button_clicked), emu_context);
	gtk_container_add(GTK_CONTAINER(frame), debug_button);
	gtk_table_attach(GTK_TABLE(emu_context->main_table), frame, 0, 1, 2, 3, attach_options, attach_options, 5, 5);

	/* Add containers to the window and show everything */
	gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), emu_context->main_table, FALSE, FALSE, 3);
	gtk_container_add(GTK_CONTAINER(emu_context->main_window), vbox);
	gtk_widget_show_all(GTK_WIDGET(emu_context->main_window));
	return 0;
}

void nes_gui_destroy(emu_context_t *emu_context)
{
	/* FIXME: Free all other members of the nes_gui struct too */
	free(emu_context);
}

static void application_start_suggest_open(GApplication *app, gpointer data)
{
	(void) data;
	GtkWidget *label;
	GtkWidget *fcbutton;
	emu_context_t *emu_context = emu_context_init(app);

	g_print("Suggesting to load file...\n");

	/* Put a convenience button in the middle of the main table and suggest
	   to the user that she should load a file */
	label = gtk_label_new("No ROM loaded.\nPlease select one.");
	fcbutton = gtk_file_chooser_button_new("Select NES ROM...", GTK_FILE_CHOOSER_ACTION_OPEN);
	g_signal_connect(fcbutton, "file-set", G_CALLBACK(file_selected), emu_context);
	gtk_table_attach_defaults(GTK_TABLE(emu_context->main_table), label, 1, 2, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(emu_context->main_table), fcbutton, 1, 2, 2, 3);
	gtk_widget_show_all(GTK_WIDGET(emu_context->main_window));

	gtk_main();
}

static void application_start_open(GApplication *app, GFile **files,
		gint no_files, gchar *hint)
{
	(void) hint;

	emu_context_t *emu_context = emu_context_init(app);

	if(no_files > 1)
	{
		g_print("You can only open one file!\n");
		gtk_exit(1);
	}

	/* Get the name of the selected file */
	char *file_uri;
	file_uri = g_file_get_uri(files[0]);
	g_print("File selected is: %s\n", file_uri);
	g_free(file_uri);

	g_print("Opening file...\n");
	gtk_widget_show_all((GtkWidget *) emu_context->main_window);

	g_print("Instantiating a NES...\n");
	GFileInputStream *stream = g_file_read(files[0], NULL, NULL);
	if(!stream)
	{
		g_print("Couldn not open file!\n");
		gtk_exit(1);
	}
	/* Don't follow symlinks */
	GFileInfo *info = g_file_query_info(files[0], "*", G_FILE_QUERY_INFO_NONE,
						NULL, NULL);
	unsigned file_size = g_file_info_get_size(info);
	char *file_buffer = malloc(file_size * sizeof(char));
	/* FIXME: Is this cast really correct?? */
	int bytes_read = g_input_stream_read(G_INPUT_STREAM(stream), file_buffer, file_size, NULL, NULL);
	g_print("Read %d of %d bytes.\n", bytes_read, file_size);

	emu_context->nes = nes_init(file_buffer, file_size);
	free(file_buffer);
	if(!emu_context->nes)
	{
		g_print("NES ROM parse failed!\n");
		gtk_exit(1);
	}	

	printf("PRG banks: %d\n", nes_get_prg_banks(emu_context->nes));
	printf("CHR banks: %d\n", nes_get_chr_banks(emu_context->nes));

	if(nes_get_mirroring(emu_context->nes) == VERTICAL_MIRRORING)
		printf("vertical mirroring\n");
	else if(nes_get_mirroring(emu_context->nes) == HORIZONTAL_MIRRORING)
		printf("horizontal mirroring\n");
	else if(nes_get_mirroring(emu_context->nes) == FOUR_SCREEN)
		printf("four screen\n");

	if(nes_get_sram_present(emu_context->nes))
		printf("SRAM is present.\n");
	else
		printf("SRAM is not present.\n");

	g_print("Mapper %03d.\n", nes_get_mapper(emu_context->nes));

	init_memory_widget(emu_context);
	init_stack_widget(emu_context);
	init_cpu_widget(emu_context);
	init_ppu_widget(emu_context);
	init_disasm_widget(emu_context);
	init_execute_widget(emu_context);

	update_all_widgets(emu_context);

	gtk_main();

}

void init_disasm_widget(emu_context_t *emu_context)
{

	GValue v = G_VALUE_INIT;
	GValue v2 = G_VALUE_INIT;

	g_value_init(&v, G_TYPE_POINTER);
	g_value_set_pointer(&v, (gpointer) nes_get_cpu_ram(emu_context->nes));
	g_object_set_property(G_OBJECT(emu_context->disasm_widget), "ram", &v);

	g_value_set_pointer(&v, (gpointer) nes_get_cpu_rom(emu_context->nes));
	g_object_set_property(G_OBJECT(emu_context->disasm_widget), "rom", &v);

	g_value_set_pointer(&v, (gpointer) nes_get_cpu(emu_context->nes));
	g_object_set_property(G_OBJECT(emu_context->disasm_widget), "cpu", &v);

	g_value_set_pointer(&v, (gpointer) cpu_get_pc);
	g_object_set_property(G_OBJECT(emu_context->disasm_widget), "read_pc_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_ram_read);
	g_object_set_property(G_OBJECT(emu_context->disasm_widget), "ram_read_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_rom_read);
	g_object_set_property(G_OBJECT(emu_context->disasm_widget), "rom_read_callback", &v);
	
	g_value_set_pointer(&v, (gpointer) nes_disasm);
	g_object_set_property(G_OBJECT(emu_context->disasm_widget), "disasm_callback", &v);

	g_value_init(&v2, G_TYPE_UINT);
	g_value_set_uint(&v2, 0x8000);
	g_object_set_property(G_OBJECT(emu_context->disasm_widget), "address", &v2);

	disasm_widget_update((DisasmWidget *) emu_context->disasm_widget);
}

void init_memory_widget(emu_context_t *emu_context)
{
	GValue v = G_VALUE_INIT;
	g_value_init(&v, G_TYPE_POINTER);

	g_value_set_pointer(&v, (gpointer) nes_get_cpu_ram(emu_context->nes));
	g_object_set_property(G_OBJECT(emu_context->memory_widget), "ram", &v);

	g_value_set_pointer(&v, (gpointer) nes_get_cpu_rom(emu_context->nes));
	g_object_set_property(G_OBJECT(emu_context->memory_widget), "rom", &v);

	g_value_set_pointer(&v, (gpointer) nes_asm);
	g_object_set_property(G_OBJECT(emu_context->memory_widget), "assembly_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_ram_read);
	g_object_set_property(G_OBJECT(emu_context->memory_widget), "ram_read_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_rom_read);
	g_object_set_property(G_OBJECT(emu_context->memory_widget), "rom_read_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_ram_write);
	g_object_set_property(G_OBJECT(emu_context->memory_widget), "ram_write_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_rom_write);
	g_object_set_property(G_OBJECT(emu_context->memory_widget), "rom_write_callback", &v);
}

void init_stack_widget(emu_context_t *emu_context)
{
	GValue v = G_VALUE_INIT;
	g_value_init(&v, G_TYPE_POINTER);

	g_value_set_pointer(&v, (gpointer) nes_get_cpu_ram(emu_context->nes));
	g_object_set_property(G_OBJECT(emu_context->stack_widget), "ram", &v);

	g_value_set_pointer(&v, (gpointer) nes_get_cpu(emu_context->nes));
	g_object_set_property(G_OBJECT(emu_context->stack_widget), "cpu", &v);

	g_value_set_pointer(&v, (gpointer) cpu_ram_read);
	g_object_set_property(G_OBJECT(emu_context->stack_widget), "ram_read_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_get_sp);
	g_object_set_property(G_OBJECT(emu_context->stack_widget), "read_sp_callback", &v);
}

void init_ppu_widget(emu_context_t *emu_context)
{
	GValue v = G_VALUE_INIT;
	g_value_init(&v, G_TYPE_POINTER);

	g_value_set_pointer(&v, (gpointer) nes_get_ppu(emu_context->nes));
	g_object_set_property(G_OBJECT(emu_context->ppu_widget), "ppu", &v);

	g_value_set_pointer(&v, (gpointer) ppu_mem_read);
	g_object_set_property(G_OBJECT(emu_context->ppu_widget), "ppu_read_callback", &v);

	g_value_set_pointer(&v, (gpointer) ppu_mem_write);
	g_object_set_property(G_OBJECT(emu_context->ppu_widget), "ppu_write_callback", &v);

}

void init_cpu_widget(emu_context_t *emu_context)
{
	GValue v = G_VALUE_INIT;
	g_value_init(&v, G_TYPE_POINTER);

	g_value_set_pointer(&v, (gpointer) nes_get_cpu(emu_context->nes));
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "cpu", &v);

	g_value_set_pointer(&v, (gpointer) cpu_get_a);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "read_a_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_get_x);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "read_x_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_get_y);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "read_y_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_get_sp);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "read_sp_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_get_p);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "read_p_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_get_pc);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "read_pc_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_get_cycles);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "read_cycles_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_set_a);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "write_a_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_set_x);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "write_x_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_set_y);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "write_y_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_set_sp);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "write_sp_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_set_p);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "write_p_callback", &v);

	g_value_set_pointer(&v, (gpointer) cpu_set_pc);
	g_object_set_property(G_OBJECT(emu_context->cpu_widget), "write_pc_callback", &v);
}

void init_execute_widget(emu_context_t *emu_context)
{
	GValue v = G_VALUE_INIT;
	g_value_init(&v, G_TYPE_POINTER);

	g_value_set_pointer(&v, (gpointer) emu_context->nes);
	g_object_set_property(G_OBJECT(emu_context->execute_widget), "nes", &v);

	g_value_set_pointer(&v, (gpointer) nes_execute);
	g_object_set_property(G_OBJECT(emu_context->execute_widget), "execute_callback", &v);

	g_value_set_pointer(&v, (gpointer) nes_step);
	g_object_set_property(G_OBJECT(emu_context->execute_widget), "step_callback", &v);

}

int main(int argc, char *argv[])
{
	GApplication *app = g_application_new("se.dose.nesdevkit", G_APPLICATION_HANDLES_OPEN);

	/* The user can specify a file to be opened from the command line.
	   There's probably a more beautifully way of enabling that very
	   functionality and the --help argument, but this works. */
	static GOptionEntry command_options[] = {
		{ NULL, 0, 0, 0, NULL, NULL, NULL}
	};
	GError *error = NULL;
	GOptionContext *context = g_option_context_new("[NES ROM]");
	g_option_context_add_main_entries(context, command_options, NULL);
	g_option_context_add_group(context, gtk_get_option_group(TRUE));
	if(!g_option_context_parse(context, &argc, &argv, &error))
	{
		g_print("Usage: %s --help\n", argv[0]);
		gtk_exit(1);
	}
	g_signal_connect(app, "open", G_CALLBACK(application_start_open), NULL);
	g_signal_connect(app, "activate", G_CALLBACK(application_start_suggest_open), NULL);

	/* That's it, run application */
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}

static void debug_button_clicked(GtkButton *button, emu_context_t *emu_context)
{
	(void) button;
	(void) emu_context;

	tile_t *tile = ppu_get_tile(nes_get_ppu(emu_context->nes), 3);

	for(int i = 0; i < 0x8; i++)
	{
		for(int j = 0; j < 0x8; j++)
		{
			g_print("0x%08X ", tile_get_pixel(tile, i * 0x8 + j));
		}
		g_print("\n");
	}
	ppu_tile_destroy(tile);

}

static void cb_update_gui(GtkWidget *w, emu_context_t *emu_context)
{
	(void) w;
	update_all_widgets(emu_context);
}

void cb_disable_gui(GtkButton *button, gboolean disable, emu_context_t *emu_context)
{
	(void) button;

	GValue v = G_VALUE_INIT;
	g_value_init(&v, G_TYPE_BOOLEAN);

	if(disable)
	{
		g_value_set_boolean(&v, FALSE);
		g_object_set_property(G_OBJECT(emu_context->cpu_widget), "editable", &v);
		g_object_set_property(G_OBJECT(emu_context->memory_widget), "editable", &v);
	} else
	{
		g_value_set_boolean(&v, TRUE);
		g_object_set_property(G_OBJECT(emu_context->cpu_widget), "editable", &v);
		g_object_set_property(G_OBJECT(emu_context->memory_widget), "editable", &v);
	}

}
void update_all_widgets(emu_context_t *emu_context)
{
	GValue v = G_VALUE_INIT;
	g_value_init(&v, G_TYPE_UINT);
	g_value_set_uint(&v, cpu_get_pc(nes_get_cpu(emu_context->nes)));
	g_object_set_property(G_OBJECT(emu_context->disasm_widget), "address", &v);
	g_object_set_property(G_OBJECT(emu_context->memory_widget), "pc", &v);

	memory_widget_update(MEMORY_WIDGET(emu_context->memory_widget));
	stack_widget_update(STACK_WIDGET(emu_context->stack_widget));
	cpu_widget_update((CpuWidget *) emu_context->cpu_widget);
	disasm_widget_update((DisasmWidget *) emu_context->disasm_widget);
	ppu_widget_update(PPU_WIDGET(emu_context->ppu_widget));

	/* FIXME: Why on Earth is the one below not working, but the above is ?! */
//	cpu_widget_update(CPU_WIDGET(emu_context->cpu_widget));

}

