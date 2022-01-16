#ifndef PARSER_h
#define PARSER_h

#include "utils.h"

void parse_line(char* line, char** op1, char** op2, char** format, char** instruction);
struct CommandList* parse_file(char* file);

struct CommandList {
	struct CommandList* next;
	char* instruction;
	int format;
    char* source;
	char* destination;	
};	

#endif
