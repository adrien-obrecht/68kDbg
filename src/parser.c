#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

void parse_line(char* line,char** op1, char** op2, char** format, char** instruction) {
	char* command;
    char* operands;
	char* cpy = malloc(sizeof(char) * (strlen(line) + 1));

	strcpy(cpy, line);
	command = strtok(cpy, " ");
	if (!command) {
		printf("No command was provided in %s\n", line);
		free(cpy);
	    return;	
	}
	
	operands = strtok(NULL, "\n");
	if (!operands) {
		free(cpy);
		return;
	}
	
	*instruction = strtok(command, ".");
	if (!instruction) {
		printf("No instruction was provided in %s\n", line);
		free(cpy);
	    return;	
	}

	*format = strtok(NULL, " ");

	*op1 = strtok(operands, ",");
	if (!op1) {
		printf("No operand was provided in %s\n", line);
		free(cpy);
	    return;	
	}

	*op2 = strtok(NULL, ",");
	
	if (*format && strlen(*format) != 1) {
		printf("Format %s is unknown \n", *format);
		free(cpy);
		return;
	}	

	
}

int line_number(char* path) {
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
	FILE* fp;
	char* line = NULL;
	char *op1, *op2, *format, *instruction;
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

