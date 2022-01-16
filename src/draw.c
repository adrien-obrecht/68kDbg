#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>

#include "register.h"
#include "constants.h"
#include "utils.h"


GtkWidget* create_dr_box() {
	GtkWidget* vbox;
	GtkWidget* button;
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, VERTICAL_PADDING);
	
	for (int i = 0; i < DR_SIZE; i++) {
		button = gtk_button_new();
		gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, VERTICAL_PADDING);
	}
	return vbox;
}


void update_dr_box(GtkWidget* vbox, struct DataRegister dr) {
	GList* list = gtk_container_get_children(GTK_CONTAINER(vbox));
	GtkButton* button;
	char buff[100];
	
	for (int i = 0; i < DR_SIZE; i++) {
		button = GTK_BUTTON(list->data);
		sprintf(buff, "Data register %d : %s", i, dr.DR[i]);
		gtk_button_set_label(button, buff);
		list = list->next;
	}
}


GtkWidget* create_ar_box() {
	GtkWidget* vbox;
	GtkWidget* button;
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, VERTICAL_PADDING);
	
	for (int i = 0; i < AR_SIZE; i++) {
		button = gtk_button_new();
		gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, VERTICAL_PADDING);
	}
	return vbox;
}


void update_ar_box(GtkWidget* vbox, struct AddressRegister ar) {
	GList* list = gtk_container_get_children(GTK_CONTAINER(vbox));
	GtkButton* button;
	char buff[100];
	
	for (int i = 0; i < AR_SIZE - 1; i++) {
		button = GTK_BUTTON(list->data);
		sprintf(buff, "Address register %d : %s", i, ar.AR[i]);
		gtk_button_set_label(button, buff);
		list = list->next;
	}
	
	button = GTK_BUTTON(list->data);
	sprintf(buff, "Stack pointer : %s", ar.stack_pointer);
	gtk_button_set_label(button, buff);
}

GtkWidget* create_mem_box() {
	GtkWidget* mem;
	GtkWidget* bu;
	GtkWidget* s_window;
	int i;
	char buff[100];
	
	s_window = gtk_scrolled_window_new(NULL, NULL);	
	
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(s_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
		
	mem = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(mem), VERTICAL_PADDING);
	
	for (i = 0; i < BYTES_PER_MEM_ROW; i++) {
		sprintf(buff, "%X", i);
		bu = gtk_label_new(buff);
		gtk_grid_attach(GTK_GRID(mem), GTK_WIDGET(bu), i + 1, 0, 1, 1);
	}
	
	for (i = 0; i < (MEM_SIZE- 1) / BYTES_PER_MEM_ROW + 1; i++) {
		sprintf(buff, "%X   ", MEM_OFFSET + i * BYTES_PER_MEM_ROW);
		bu = gtk_label_new(buff);
		gtk_grid_attach(GTK_GRID(mem), GTK_WIDGET(bu), 0, i + 1, 1, 1);
	}

	
	i = 0;
	while (i / 2 < MEM_SIZE){
		for (int j = 0; j < BYTES_PER_MEM_ROW; j++) {
			if (i / 2 >= MEM_SIZE)
				break;
			bu = gtk_label_new("");	
			gtk_grid_attach(GTK_GRID(mem), GTK_WIDGET(bu), j + 1, i / 2 / BYTES_PER_MEM_ROW + 1, 1, 1);	
			i = i + 2;
		}
	}
	
	gtk_container_add(GTK_CONTAINER(s_window), mem);
	
	return s_window;
}

void update_mem_box(GtkWidget* s_window, struct Memory mem) {
	GtkWidget* bu;

	GtkWidget* mem_box = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(s_window))));
	
	int i;
	char buff[100];

	i = 0;
	while (i / 2 < MEM_SIZE){
		for (int j = 0; j < BYTES_PER_MEM_ROW; j++) {
			if (i / 2 >= MEM_SIZE)
				return;
			sprintf(buff, "%c%c", mem.mem[i], mem.mem[i+1]);
			bu = gtk_grid_get_child_at(GTK_GRID(mem_box), j+1, i / 2 / BYTES_PER_MEM_ROW + 1);	
			gtk_label_set_text(GTK_LABEL(bu), buff);
			i = i + 2;
		}
	}
}

GtkWidget* create_instruction_box(struct Compiler comp) {
	GtkWidget* ibox;
	GtkWidget* label;
	GtkWidget* s_window;
	GtkWidget* vbox;
	GtkWidget* toolbar; 
	GtkWidget* image;
	GtkToolItem* reload_item;
	GtkToolItem* new_file_item;
	GtkToolItem* play_item;
	GtkToolItem* pause_item;

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, VERTICAL_PADDING);

	s_window = gtk_scrolled_window_new(NULL, NULL);	
	
	toolbar = gtk_toolbar_new();	
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

	image = gtk_image_new_from_file("src/reload.png");
	reload_item = gtk_tool_button_new(image,"image");
	
	image = gtk_image_new_from_file("src/new-page.png");
	new_file_item = gtk_tool_button_new(image,"image");
	
	image = gtk_image_new_from_file("src/play.png");
	play_item = gtk_tool_button_new(image,"image");
	
	image = gtk_image_new_from_file("src/pause.png");
	pause_item = gtk_tool_button_new(image,"image");

	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), reload_item, 1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new_file_item, 2);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), play_item, 3);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), pause_item, 4);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(s_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
		
	ibox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	
	for (int i = 0; i < comp.command_len; i++) {
		label = gtk_label_new("");
		gtk_label_set_xalign(GTK_LABEL(label), 0);
		gtk_box_pack_start(GTK_BOX(ibox), label, TRUE, TRUE, 0);
	}
	
	gtk_container_add(GTK_CONTAINER(s_window), ibox);

	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), s_window, TRUE, TRUE, 0);

	return vbox;
}


void update_instruction_box(GtkWidget* vbox, struct Compiler comp) {
	GList* list = gtk_container_get_children(GTK_CONTAINER(vbox));

   	list = list -> next;	
	GtkWidget* s_window = list->data;
	
	GtkWidget* ibox = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(s_window))));
	list = gtk_container_get_children(GTK_CONTAINER(ibox));
	GtkWidget* label;
	char buff[100];

	
	for (int i = 0; i < comp.command_len; i++) {
		label = list->data;
		if (i == comp.command_pointer)
			sprintf(buff, "<span bgcolor='#00FF004F'>%d. %s</span>", i+1, comp.command_list[i].line);
		else
			sprintf(buff, "%d. %s", i+1, comp.command_list[i].line);
		
		gtk_label_set_markup(GTK_LABEL(label), buff);
		list = list->next;
	}
}


void update_all(struct Compiler comp, GtkWidget* window) {
	GList* list;
	GtkWidget* hbox;
   	GtkWidget* dr_box;
	GtkWidget* ar_box; 
	GtkWidget* mem_s_window; 
	GtkWidget* i_s_window; 


	list = gtk_container_get_children(GTK_CONTAINER(window));
	hbox = list->data;	
	list = gtk_container_get_children(GTK_CONTAINER(hbox));
	dr_box = GTK_WIDGET(list->data);
	list = list->next;
	ar_box = GTK_WIDGET(list->data);
	list = list->next;
	mem_s_window = list->data;
	list = list -> next;
	i_s_window = list->data;
	
	
	update_dr_box(dr_box, comp.data_register);
	update_ar_box(ar_box, comp.address_register);
	update_mem_box(mem_s_window, comp.memory);
	update_instruction_box(i_s_window, comp);
}


GtkWidget* init_window(struct Compiler comp) {
	GtkWidget* window;
	GtkWidget* hbox; 
	GtkWidget* dr_box;
	GtkWidget* ar_box;
	GtkWidget* mem_s_window;
	GtkWidget* i_s_window;

	gtk_init(0, NULL);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	dr_box = create_dr_box();
	ar_box = create_ar_box();
	mem_s_window = create_mem_box();
	i_s_window = create_instruction_box(comp);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, HORIZONTAL_PADDING);
	gtk_container_add(GTK_CONTAINER(hbox), dr_box);	
	gtk_container_add(GTK_CONTAINER(hbox), ar_box);
	gtk_container_add(GTK_CONTAINER(hbox), mem_s_window);
	gtk_container_add(GTK_CONTAINER(hbox), i_s_window);
	
	gtk_container_add(GTK_CONTAINER(window), hbox);	
			
	gtk_window_set_default_size(GTK_WINDOW(window), WINDOWS_WIDTH, WINDOWS_HEIGHT);	
	gtk_window_set_title(GTK_WINDOW(window), "Motorola 68k debugger"); 
	gtk_window_set_default_icon_from_file("src/icon-dbg.png", NULL);
	return window;
}
