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
	    return;	
	}

	operands = strtok(NULL, "\n");
	if (!operands) {
		printf("No operands were provided in %s\n", line);
		return;
	}
	
	*instruction = strtok(command, ".");
	if (!instruction) {
		printf("No instruction was provided in %s\n", line);
	    return;	
	}

	*format = strtok(NULL, " ");

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
	
struct CommandList* parse_file(char* path) {
	FILE* fp;
	char* line = NULL;
	int len;
	char *op1, *op2, *format, *instruction;
	struct CommandList *cl, *current, *prev;
	size_t u;
	
	fp = fopen(path, "r");
    
	if (!fp) {
		printf("Unknown path %s\n", path);
		return NULL;
	}	
	
	current = malloc(sizeof(struct CommandList));
	current -> instruction = "start";
	current -> source = NULL;
	current -> destination = NULL;
	current -> format = -1;
	cl = current;

	do { 
		prev = current;	
	    current = malloc(sizeof(struct CommandList));	
		prev -> next = current;
		len = getline(&line, &u, fp);
		parse_line(line, &op1, &op2, &format, &instruction);
		
		if (instruction) {
			current -> instruction = malloc((1 + strlen(instruction)) * sizeof(char));
			strcpy(current->instruction, instruction);
		}
		else 
			current->instruction = NULL;
		
		if (op1) {
			current -> source = malloc((1 + strlen(op1)) * sizeof(char));
			strcpy(current->source, op1);
		}
		else
			current -> source = NULL;
		
		if (op2) {
			current -> destination = malloc((1 + strlen(op2)) * sizeof(char));
			strcpy(current->destination, op2);
		}
		else
			current -> destination = NULL;
	
		if (format)
			current -> format = size_of_type(format[0]);
		else
			current -> format = -1;
		
	} while (len != -1);
	prev -> next = NULL;
	return cl;
}   

