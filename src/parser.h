#ifndef PARSER_h
#define PARSER_h

#include "utils.h"

void parse_line(char* line, char** op1, char** op2, char** format, char** instruction);
struct Command* parse_file(char* file, int* size);


#endif
