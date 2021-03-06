#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>

#include "register.h"
#include "parser.h"
#include "constants.h"
#include "draw.h"
#include "commands.h"
#include "parser.h"
#include "execute.h"


int main(int argc, char** argv) {
	GtkWidget *window;
	struct Compiler comp;
	struct Command* cl;
	int size;

	time_t current_time, former_time = 0;
	
	// This main function runs the main loop which keeps the Dbg windows open

	// Initialise variables
	reset_compiler(&comp);
	cl = parse_file("", &size);
	comp.command_list = cl;
	comp.command_len = size;
	comp.file = "";
	window = init_window(&comp);

	while (GTK_IS_WIDGET(window)) {
		// Runs an iteration each second while giving time to render
		current_time = time(NULL);
		if (current_time - former_time > 0) {
			former_time = current_time;
			do_one_iteration(&comp);
			update_all(&comp, window);
		}
		gtk_widget_show_all(window);
		gtk_main_iteration_do(FALSE);
	}

	return 0;
}
