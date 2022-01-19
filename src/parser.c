#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

void parse_line(char* line,char** op1, char** op2, char** format, char** instruction) {
	// Parses a line of assembly code, using strtok to separate command, operand and format specifier
	// A normal line looks like:
	// instruction.format operand1, operand2
	// But format can be unspecified and both operands can be missing
	char* command;
    char* operands;
	char* cpy = malloc(sizeof(char) * (strlen(line) + 1));

	strcpy(cpy, line);
	cpy[strlen(line) - 1] = '\0';
	command = strtok(cpy, " ");
	if (!command) {
		printf("No command was provided in %s\n", line);
	    return;	
	}

	operands = strtok(NULL, "\n");
	
	*instruction = strtok(command, ".");
	if (!instruction) {
		printf("No instruction was provided in %s\n", line);
	    return;	
	}
	
	*format = strtok(NULL, " ");
	
	if (!operands) {
		return;
	}


	*op1 = strtok(operands, ",");
	if (!op1) {
		printf("No operand was provided in %s\n", line);
	    return;	
	}

	*op2 = strtok(NULL, ",");
	
	if (*format && strlen(*format) != 1) {
		printf("Format %s is unknown \n", *format);
		return;
	}	

	
}

int line_number(char* path) {
	// Counts the number of line of the file, to allocate enough memory for it
	FILE* fp;
	int num = 0;

	fp = fopen(path, "r");
	
	for (char c=getc(fp); c != EOF; c=getc(fp)){
		if (c == '\n') {
			num++;
		}
	}
	fclose(fp);
	return num;
}
	
struct Command* parse_file(char* path, int* size) {
	// Parses the whole file and returns the corresponding allocated command list
	FILE* fp;
	char* line = NULL;
	char *op1 = NULL, *op2 = NULL, *format = NULL, *instruction = NULL;
	struct Command* command_list;
	size_t u;
	
	fp = fopen(path, "r");
    
	if (!fp) {
		printf("Unknown path %s\n", path);
		return NULL;
	}	
	
	*size = line_number(path);
	command_list = malloc(sizeof(struct Command) * *size);

	for (int i = 0; i < *size; i++) { 
		// For each line, we parse it and copy the data to the command list
		getline(&line, &u, fp);
		parse_line(line, &op1, &op2, &format, &instruction);
			
		command_list[i].line = malloc((1 + strlen(line)) * sizeof(char));
		strcpy(command_list[i].line, line);
		
		if (instruction) {
			command_list[i].instruction = malloc((1 + strlen(instruction)) * sizeof(char));
			strcpy(command_list[i].instruction, instruction);
		}
		else 
			command_list[i].instruction = NULL;
		
		if (op1) {
			command_list[i].source = malloc((1 + strlen(op1)) * sizeof(char));
			strcpy(command_list[i].source, op1);
		}
		else
			command_list[i].source = NULL;
		
		if (op2) {
			command_list[i].destination = malloc((1 + strlen(op2)) * sizeof(char));
			strcpy(command_list[i].destination, op2);
		}
		else
			command_list[i].destination = NULL;
	
		if (format)
			command_list[i].format = size_of_type(format[0]);
		else
			command_list[i].format = -1;
	}
	
	fclose(fp);
	
	return command_list;	
}   

