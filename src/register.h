#include "constants.h"

#ifndef REGISTER_h
#define REGISTER_h 0 

struct DataRegister {
	char DR[8][9];
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
};

void reset_ar(struct AddressRegister*);
void reset_dr(struct DataRegister*);
void reset_mem(struct Memory*);
void reset_compiler(struct Compiler*);

#endif
