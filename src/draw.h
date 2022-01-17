#ifndef DRAW_h
#define DRAW_h 0

#include <gtk/gtk.h>

#include "register.h"


GtkWidget* init_window(struct Compiler* comp);
void update_all(struct Compiler comp, GtkWidget* window);
#endif
