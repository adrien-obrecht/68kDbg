#include <string.h>

#include "register.h"

void reset_ar(struct AddressRegister* ar) {
	for (int i = 0; i < 7; i++) {
		strcpy(ar->AR[i], "00000000");
	}
	strcpy(ar->stack_pointer, "00000000");
}

void reset_dr(struct DataRegister* dr) {
	for (int i = 0; i < 8; i++) {
		strcpy(dr->DR[i], "00000000");
	}
}

void reset_mem(struct Memory* mem) {
	for (int i = 0; i < sizeof(mem -> mem); i++) {
		mem->mem[i] = '0';
	}
}

void reset_compiler(struct Compiler* comp) {
	reset_dr(&comp -> data_register);
	reset_ar(&comp -> address_register);
	reset_mem(&comp -> memory);
}
