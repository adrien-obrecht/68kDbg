#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>

#include "register.h"
#include "parser.h"
#include "constants.h"
#include "draw.h"
#include "commands.h"
#include "parser.h"


int main(int argc, char** argv) {
	GtkWidget *window;
	struct Compiler comp;
	struct Operand source, dest;

	time_t current_time, former_time = 0;
	
	reset_compiler(&comp);

	window = init_window(comp);
	
	struct CommandList* cl = parse_file("test.s");
	
	while (GTK_IS_WIDGET(window)) {
		current_time = time(NULL);
		if (current_time - former_time > 0) {
			update_all(comp, window);
			former_time = current_time;
			if (cl) {
				source = val_of_op(&comp, cl->source, cl -> format);
				dest = val_of_op(&comp, cl->destination, cl -> format);
			
				if (strcmp("move", cl -> instruction) == 0) {
					printf("move.%d %s %s \n", cl -> format, cl -> source, cl -> destination);
					move(&comp, cl -> format, source, dest);
				}

				else if (strcmp("add", cl -> instruction) == 0) {
					printf("add.%d %s %s \n", cl -> format, cl -> source, cl -> destination);
					add(&comp, cl -> format, source, dest);
				}
			
				else if (strcmp("sub", cl -> instruction) == 0) {
					printf("sub.%d %s %s \n", cl -> format, cl -> source, cl -> destination);
					sub(&comp, cl -> format, source, dest);
				}
			
				else if (strcmp("swap", cl -> instruction) == 0) {
					if (cl -> format != -1) {
						printf("Swap doesn't support custom format\n");
					}
					else if (cl -> destination) {
						printf("Swap only has one operand\n");
					}
					else {
						printf("swap %s \n", cl -> source);
						swap(&comp, source);
					}
				}
				
				else if (strcmp("exg", cl -> instruction) == 0) {
					printf("exg.%d %s %s \n", cl -> format, cl -> source, cl -> destination);
					exchange(&comp, cl -> format, source, dest);
				}
				
				else if (strcmp("clr", cl -> instruction) == 0) {
					if (cl -> destination) {
						printf("Clear only has one operand\n");
					}
					else {
						printf("clr.%d %s\n", cl -> format, cl -> source);
						clear(&comp, cl -> format, source);
					}
				}
			
				else {
					printf("Unknown instruction %s\n", cl->instruction);
				}
				cl = cl -> next;
			}
		}
		gtk_widget_show_all(window);
		gtk_main_iteration_do(FALSE);
	}

	return 0;
}
