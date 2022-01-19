#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>

#include "register.h"
#include "constants.h"
#include "parser.h"
#include "utils.h"
#include "draw.h"


GtkWidget* create_dr_box() {
	// Initialises the box to display the data registers (vertical box made of buttons)
	GtkWidget* vbox;
	GtkWidget* button;
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, VERTICAL_PADDING);
	
	for (int i = 0; i < DR_SIZE; i++) {
		button = gtk_button_new();
		gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, VERTICAL_PADDING);
	}
	return vbox;
}


void update_dr_box(GtkWidget* vbox, struct DataRegister* dr) {
	// Runtime update of the labels to match data registers
	GList* list = gtk_container_get_children(GTK_CONTAINER(vbox));
	GtkButton* button;
	char buff[100];
	
	for (int i = 0; i < DR_SIZE; i++) {
		button = GTK_BUTTON(list->data);
		sprintf(buff, "Data register %d : %s", i, dr -> DR[i]);
		gtk_button_set_label(button, buff);
		list = list->next;
	}
}


GtkWidget* create_ar_box() {
	// Initialises the box to display the address registers (vertical box made of buttons)
	GtkWidget* vbox;
	GtkWidget* button;
	GtkWidget* label;
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, VERTICAL_PADDING);
	
	for (int i = 0; i < AR_SIZE; i++) {
		button = gtk_button_new();
		gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, VERTICAL_PADDING);
	}
	
	label = gtk_label_new("");
	// This is a place for the CCR
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, VERTICAL_PADDING);

	return vbox;
}


void update_ar_box(GtkWidget* vbox, struct Compiler* comp) {
	// Runtime update of the labels to match address registers
	GList* list = gtk_container_get_children(GTK_CONTAINER(vbox));
	GtkButton* button;
	GtkWidget* label;
	char buff[100];
	
	for (int i = 0; i < AR_SIZE - 1; i++) {
		button = GTK_BUTTON(list->data);
		sprintf(buff, "Address register %d : %s", i, comp -> address_register.AR[i]);
		gtk_button_set_label(button, buff);
		list = list->next;
	}
	
	button = GTK_BUTTON(list->data);
	sprintf(buff, "Stack pointer : %s", comp -> address_register.stack_pointer);
	gtk_button_set_label(button, buff);
	list = list->next;
		
	label = list->data;
	// Also update CCR
	sprintf(buff, "<b>          X  N  Z  V  C\nCCR: %d  %d  %d  %d  %d</b>", 
			comp -> ccr_x, 
			comp -> ccr_n, 
			comp -> ccr_z, 
			comp -> ccr_v, 
			comp -> ccr_c);
	gtk_label_set_markup(GTK_LABEL(label), buff);
}

GtkWidget* create_mem_box() {
	// Initialises the grid that will hold memory information
	GtkWidget* mem;
	GtkWidget* bu;
	GtkWidget* s_window;
	int i;
	char buff[100];
	
	s_window = gtk_scrolled_window_new(NULL, NULL);	
	
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(s_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	// Use a scrolled window to display as much memory as we want
		
	mem = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(mem), VERTICAL_PADDING);
	
	// First raw / column display the position info in the memory
	
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

void update_mem_box(GtkWidget* s_window, struct Memory* mem) {
	// Runtime update of the memory information
	GtkWidget* bu;

	GtkWidget* mem_box = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(s_window))));
	
	int i;
	char buff[100];

	i = 0;
	while (i / 2 < MEM_SIZE){
		for (int j = 0; j < BYTES_PER_MEM_ROW; j++) {
			if (i / 2 >= MEM_SIZE)
				return;
			if (mem -> modified[i/2] == 1) {
				sprintf(buff, "<span bgcolor='#FF3030'>%c%c</span>", mem -> mem[i], mem -> mem[i+1]);
				mem -> modified[i/2] = 0;	
			}
			else	
				sprintf(buff, "%c%c", mem -> mem[i], mem -> mem[i+1]);
			bu = gtk_grid_get_child_at(GTK_GRID(mem_box), j+1, i / 2 / BYTES_PER_MEM_ROW + 1);	
			gtk_label_set_markup(GTK_LABEL(bu), buff);
			i = i + 2;
		}
	}
}

void on_pause_play_item_clicked(GtkWidget* item, struct Compiler* comp) {
	// Callback funtion called when pause/play is clicked
	// Changes speed/display of the button
	GtkWidget* image;
	if (comp -> execution_speed == 1) {
		comp -> execution_speed = 0;	
		image = gtk_image_new_from_file("src/play.png");
		gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(item), image);
		gtk_widget_set_tooltip_text(GTK_WIDGET(item), "Play");
	}
	else {	
		comp -> execution_speed = 1;	
		image = gtk_image_new_from_file("src/pause.png");
		gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(item), image);
		gtk_widget_set_tooltip_text(GTK_WIDGET(item), "Pause");
	}
}

void on_new_file_item_clicked(GtkWidget* item) {
	// Callback funtion called when new file is clicked
	// Creates a file chooser windows which will ask for a .s file and load it into memory
	//
	struct Compiler* comp = g_object_get_data(G_OBJECT(item), "comp");
	GtkWidget* vbox = g_object_get_data(G_OBJECT(item), "vbox");
	GtkWidget* parent = g_object_get_data(G_OBJECT(item), "parent");
	GtkWidget* chooser_window;
	GtkFileFilter* filter = gtk_file_filter_new();
	int res, size;
	
	chooser_window = gtk_file_chooser_dialog_new("Title", NULL, 
												GTK_FILE_CHOOSER_ACTION_OPEN, 
												("_Cancel"), 
												GTK_RESPONSE_CANCEL, 
												("_Open"), 
												GTK_RESPONSE_ACCEPT, 
												NULL);
	// Select only assembly file (.s)
	gtk_file_filter_add_pattern(filter, "*.s");
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(chooser_window), filter);
	res = gtk_dialog_run(GTK_DIALOG(chooser_window));
	if (res == GTK_RESPONSE_ACCEPT)
  	{
		// If user decides to persue change, we destroy old command list and build a new one
    	comp -> file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser_window));
		comp -> command_list = parse_file(comp -> file, &size);
		comp -> command_len = size;
		reset_compiler(comp);
		gtk_widget_destroy(vbox);
		vbox = create_instruction_box(comp, parent);
	    gtk_container_add(GTK_CONTAINER(parent), vbox);	
  	}
	gtk_widget_destroy(chooser_window);
}

void on_reload_item_clicked(GtkWidget* item) {
	// Callback function called when reload is clicked
	// Fetches back the data from loaded file, reloading the instruction list
	struct Compiler* comp = g_object_get_data(G_OBJECT(item), "comp");
	GtkWidget* vbox = g_object_get_data(G_OBJECT(item), "vbox");
	GtkWidget* parent = g_object_get_data(G_OBJECT(item), "parent");
	int size;
    
	comp -> command_list = parse_file(comp -> file, &size);
	comp -> command_len = size;
	reset_compiler(comp);
	gtk_widget_destroy(vbox);
	vbox = create_instruction_box(comp, parent);
	gtk_container_add(GTK_CONTAINER(parent), vbox);	
}

GtkWidget* create_instruction_box(struct Compiler* comp, GtkWidget* parent) {
	// Initialises the box displaying the instructions / the widget to load reload and play
	GtkWidget* ibox;
	GtkWidget* label;
	GtkWidget* s_window;
	GtkWidget* vbox;
	GtkWidget* toolbar; 
	GtkWidget* image;
	GtkToolItem* reload_item;
	GtkToolItem* new_file_item;
	GtkToolItem* pause_play_item;

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	s_window = gtk_scrolled_window_new(NULL, NULL);	
	
	toolbar = gtk_toolbar_new();	
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

	image = gtk_image_new_from_file("src/reload.png");
	reload_item = gtk_tool_button_new(image,"image");
	gtk_widget_set_tooltip_text(GTK_WIDGET(reload_item), "Reload file");
	g_object_set_data(G_OBJECT(reload_item), "vbox", vbox);
	g_object_set_data(G_OBJECT(reload_item), "comp", comp);
	g_object_set_data(G_OBJECT(reload_item), "parent", parent);
	g_signal_connect(G_OBJECT(reload_item), "clicked", G_CALLBACK(on_reload_item_clicked), NULL);	

	image = gtk_image_new_from_file("src/new-page.png");
	new_file_item = gtk_tool_button_new(image,"image");
	gtk_widget_set_tooltip_text(GTK_WIDGET(new_file_item), "Load new file");
	g_object_set_data(G_OBJECT(new_file_item), "vbox", vbox);
	g_object_set_data(G_OBJECT(new_file_item), "comp", comp);
	g_object_set_data(G_OBJECT(new_file_item), "parent", parent);
	g_signal_connect(G_OBJECT(new_file_item), "clicked", G_CALLBACK(on_new_file_item_clicked), NULL);	
	
	image = gtk_image_new_from_file("src/play.png");
	pause_play_item = gtk_tool_button_new(image,"image");
	gtk_widget_set_tooltip_text(GTK_WIDGET(pause_play_item), "Play");
	g_signal_connect(G_OBJECT(pause_play_item), "clicked", G_CALLBACK(on_pause_play_item_clicked), comp);	

	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), reload_item, 1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new_file_item, 2);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), pause_play_item, 3);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(s_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
		
	ibox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	
	for (int i = 0; i < comp -> command_len; i++) {
		label = gtk_label_new("");
		gtk_label_set_xalign(GTK_LABEL(label), 0);
		gtk_box_pack_start(GTK_BOX(ibox), label, FALSE, TRUE, 0);
	}
	
	gtk_container_add(GTK_CONTAINER(s_window), ibox);

	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), s_window, TRUE, TRUE, 0);

	return vbox;
}


void update_instruction_box(GtkWidget* vbox, struct Compiler* comp) {
	// Update the instruction box, highlighting where the current instruction pointer is
	GList* list = gtk_container_get_children(GTK_CONTAINER(vbox));

   	list = list -> next;	
	GtkWidget* s_window = list->data;
	
	GtkWidget* ibox = gtk_bin_get_child(GTK_BIN(gtk_bin_get_child(GTK_BIN(s_window))));
	list = gtk_container_get_children(GTK_CONTAINER(ibox));
	GtkWidget* label;
	char buff[500];
	char instruction[100], format[100], op1[100], op2[100];
    
    	
	for (int i = 0; i < comp -> command_len; i++) {
		label = list->data;
				
		// Markup is used for basic syntax highlighting
		sprintf(instruction, "<span color='#7000FF'>%s</span>", comp -> command_list[i].instruction);

		switch (comp -> command_list[i].format) {
			case 1:
				sprintf(format, "<span color='#C0C000'>.b</span>");
				break;
			case 2:
				sprintf(format, "<span color='#C0C000'>.w</span>");
				break;
			case 4:
				sprintf(format, "<span color='#C0C000'>.l</span>");
				break;
			default:
				sprintf(format, " ");
				break;
		}
		if (comp -> command_list[i].source)
			sprintf(op1, "<span color='#B00000'>%s</span>", comp -> command_list[i].source);
		else
			sprintf(op1, " ");
		
		if (comp -> command_list[i].destination)
			sprintf(op2, "<span color='#B00000'>, %s</span>", comp -> command_list[i].destination);
		else
			sprintf(op2, " ");

		if (i == comp -> command_pointer - 1)
			sprintf(buff, "<span bgcolor='#00FF004F'><b>%d. %s%s %s %s</b></span>", i+1, instruction, format, op1, op2);
		else
			sprintf(buff, "<b>%d. %s%s %s %s</b>", i+1, instruction, format, op1, op2);
		
		gtk_label_set_markup(GTK_LABEL(label), buff);
		list = list->next;
	}

	while (list != NULL) {
		// Clean the memory by destroying 'unreachable' blocks of data (from file reloading)
		gtk_widget_destroy(list->data);
		list = list->next;
	}
}


void update_all(struct Compiler* comp, GtkWidget* window) {
	// Redirects all the updates to the correct components of the gui
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
	
	
	update_dr_box(dr_box, &(comp -> data_register));
	update_ar_box(ar_box, comp);
	update_mem_box(mem_s_window, &(comp -> memory));
	update_instruction_box(i_s_window, comp);
}


GtkWidget* init_window(struct Compiler* comp) {
	// Initialises the whole gui, creating the window and object ordering
	GtkWidget* window;
	GtkWidget* hbox; 
	GtkWidget* dr_box;
	GtkWidget* ar_box;
	GtkWidget* mem_s_window;
	GtkWidget* i_s_window;

	gtk_init(0, NULL);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, HORIZONTAL_PADDING);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	dr_box = create_dr_box();
	ar_box = create_ar_box();
	mem_s_window = create_mem_box();
	i_s_window = create_instruction_box(comp, hbox);
	
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
