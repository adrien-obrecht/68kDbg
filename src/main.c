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
	int size;

	time_t current_time, former_time = 0;
	
	reset_compiler(&comp);

	window = init_window(comp);
	
	struct Command* cl = parse_file("test.s", &size);
	int i = 0;	
	while (GTK_IS_WIDGET(window)) {
		current_time = time(NULL);
		if (current_time - former_time > 0) {
			update_all(comp, window);
			former_time = current_time;
			if (i < size) {
				source = val_of_op(&comp, cl[i].source, cl[i].format);
				dest = val_of_op(&comp, cl[i].destination, cl[i].format);
			
				if (strcmp("move", cl[i].instruction) == 0) {
					printf("move.%d %s %s \n", cl[i].format, cl[i].source, cl[i].destination);
					move(&comp, cl[i].format, source, dest);
				}

				else if (strcmp("add", cl[i].instruction) == 0) {
					printf("add.%d %s %s \n", cl[i].format, cl[i].source, cl[i].destination);
					add(&comp, cl[i].format, source, dest);
				}
			
				else if (strcmp("sub", cl[i].instruction) == 0) {
					printf("sub.%d %s %s \n", cl[i].format, cl[i].source, cl[i].destination);
					sub(&comp, cl[i].format, source, dest);
				}
			
				else if (strcmp("swap", cl[i].instruction) == 0) {
					if (cl[i].format != -1) {
						printf("Swap doesn't support custom format\n");
					}
					else if (cl[i].destination) {
						printf("Swap only has one operand\n");
					}
					else {
						printf("swap %s \n", cl[i].source);
						swap(&comp, source);
					}
				}
				
				else if (strcmp("exg", cl[i].instruction) == 0) {
					printf("exg.%d %s %s \n", cl[i].format, cl[i].source, cl[i].destination);
					exchange(&comp, cl[i].format, source, dest);
				}
				
				else if (strcmp("clr", cl[i].instruction) == 0) {
					if (cl[i].destination) {
						printf("Clear only has one operand\n");
					}
					else {
						printf("clr.%d %s\n", cl[i].format, cl[i].source);
						clear(&comp, cl[i].format, source);
					}
				}
			
				else {
					printf("Unknown instruction %s\n", cl[i].instruction);
				}
				i++;;
			}
		}
		gtk_widget_show_all(window);
		gtk_main_iteration_do(FALSE);
	}

	return 0;
}
