#ifndef DRAW_h
#define DRAW_h 0

#include <gtk/gtk.h>

#include "register.h"


GtkWidget* create_instruction_box(struct Compiler* comp, GtkWidget* parent); 
GtkWidget* init_window(struct Compiler* comp);
void update_all(struct Compiler* comp, GtkWidget* window);
#endif
