#ifndef UTILS_h
#define UTILS_h

#include "register.h"

enum OpType{INVALID, DATA_REGISTER, ADDR_REGISTER, RAW_NUMBER, MEMORY_ADDR}; 

struct Operand {
	enum OpType type;
	int value;
};

int size_of_type(char type);
void write_mem(struct Compiler* comp, int position, int data, int size);
void write_hex(char* tab, int data, int size);
void write_dr(struct Compiler* comp, int position, int data, int size); 
void write_ar(struct Compiler* comp, int position, int data, int size); 
int read_mem(struct Compiler* comp, int position, int size); 
int read_dr(struct Compiler* comp, int position, int size);
int read_ar(struct Compiler* comp, int position, int size); 
int read_hex(char* tab, int size);
struct Operand val_of_op(struct Compiler* comp, char* op, int size);

#endif
