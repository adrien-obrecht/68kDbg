#ifndef REGISTER_h
#define REGISTER_h 0 

#include "constants.h"

struct DataRegister {
	char DR[8][9];
};

struct Command {
	char* line;
	char* instruction;
	int format;
    char* source;
	char* destination;	
};	

struct AddressRegister {
	char AR[7][9];
	char stack_pointer[9];
};

struct Memory {
	char mem[2 * MEM_SIZE];
};


struct Compiler {
	struct AddressRegister address_register;
	struct DataRegister data_register;
	struct Memory memory;
	struct Command* command_list;
	int command_pointer;
	int command_len;
	int execution_speed;
};

void reset_ar(struct AddressRegister*);
void reset_dr(struct DataRegister*);
void reset_mem(struct Memory*);
void reset_compiler(struct Compiler*);

#endif
