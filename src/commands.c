#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "register.h"
#include "utils.h"


void move(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value;
	
	if (size == -1) {
		printf("Move missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			value = source.value;
			break;
		case ADDR_REGISTER:
			value = read_ar(comp, source.value, size);
			break;
		case DATA_REGISTER:
			value = read_dr(comp, source.value, size);
			break;
		case MEMORY_ADDR:	
			value = read_mem(comp, source.value, size);
			break;
	}
	
	switch (dest.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			printf("Can't move to a raw number\n");
			break;

		case ADDR_REGISTER:
			write_ar(comp, dest.value, value, size); 
			break;

		case DATA_REGISTER:
			write_dr(comp, dest.value, value, size); 
			break;

		case MEMORY_ADDR:
			write_mem(comp, dest.value, value, size);		
			break;
	}
	if (value < 0)
		comp -> ccr_n = 1;
	else
		comp -> ccr_n = 0;

	if (value == 0)
		comp -> ccr_z = 1;
	else
		comp -> ccr_z = 0;

	comp -> ccr_v = 0;
	comp -> ccr_c = 0;
}


void add(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value;
	
	if (size == -1) {
		printf("Add missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			value = source.value;
			break;
		case ADDR_REGISTER:
			value = read_ar(comp, source.value, size);
			break;
		case DATA_REGISTER:
			value = read_ar(comp, source.value, size);
			break;
		case MEMORY_ADDR:	
			value = read_mem(comp, source.value, size);
			break;
	}
	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't perfrom an addition to a raw number\n");	
			return;

		case ADDR_REGISTER:
			value = value + read_ar(comp, dest.value, size);
			write_ar(comp, dest.value, value, size); 
			break;

		case DATA_REGISTER:
			value = value + read_dr(comp, dest.value, size);
			write_dr(comp, dest.value, value, size); 
			break;

		case MEMORY_ADDR:
			if (source.type == MEMORY_ADDR) {
				printf("Can't add two memory addresses directly\n");
				return;
			}
			value = value + read_mem(comp, dest.value, size);
			write_mem(comp, dest.value, value, size);
			break;
	}
	if (value < 0) 
		comp -> ccr_n = 1;
	else
		comp -> ccr_n = 0;

	if (value == 0)
		comp -> ccr_z = 1;
	else
		comp -> ccr_z = 0;	
}


void sub(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value;
	
	if (size == -1) {
		printf("Sub missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			value = source.value;
			break;
		case ADDR_REGISTER:
			value = read_ar(comp, source.value, size);
			break;
		case DATA_REGISTER:
			value = read_dr(comp, source.value, size);
			break;
		case MEMORY_ADDR:	
			value = read_mem(comp, source.value, size);
			break;
	}
	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't perfrom a substraction to a raw number\n");	
			return;

		case ADDR_REGISTER:
			value =  - value + read_ar(comp, dest.value, size);
			write_ar(comp, dest.value, value, size); 
			break;

		case DATA_REGISTER:
			value =  - value + read_dr(comp, dest.value, size);
			write_dr(comp, dest.value, value, size); 
			break;

		case MEMORY_ADDR:
			if (source.type == MEMORY_ADDR) {
				printf("Can't add two memory addresses directly\n");
				return;
			}
			value = - value + read_mem(comp, dest.value, size);
			write_mem(comp, dest.value, value, size);
			break;
	}
	if (value < 0) 
		comp -> ccr_n = 1;
	else
		comp -> ccr_n = 0;

	if (value == 0)
		comp -> ccr_z = 1;
	else
		comp -> ccr_z = 0;
}


void swap(struct Compiler* comp, struct Operand source) {
	int l_16, u_16;
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			printf("Can't swap a raw number\n"); 
			return;
		case ADDR_REGISTER:
			printf("Can't swap an address register\n"); 
			return;
		case DATA_REGISTER:
			l_16 = read_hex(&(comp->data_register.DR[source.value][4]), 2);
			u_16 = read_hex(&(comp->data_register.DR[source.value][0]), 2);
			
			write_hex(&comp->data_register.DR[source.value][0], l_16, 2);		
			write_hex(&comp->data_register.DR[source.value][4], u_16, 2);		
			break;
		case MEMORY_ADDR:	
			printf("Can't swap a memory address\n"); 
			return;
	}
	if ((l_16 & (1<<15)) == (1<<15))
		comp -> ccr_n = 1;
	else
		comp -> ccr_n = 0;
	
	if (l_16 == 0 && u_16 == 0)
		comp -> ccr_z = 1;
	else
		comp -> ccr_z = 0;

	comp -> ccr_c = 0;
	comp -> ccr_v = 0;
}

void exchange(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value1, value2;
	
	if (size == -1) {
		printf("Exchange missing a format specifier\n");
		return;
	}

	if (size != 4) {
		printf("Exchange only supports long word format\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			printf("Can't exchange raw numbers\n");
			return;
		case ADDR_REGISTER:
			value1 = read_ar(comp, source.value, size);
			break;
		case DATA_REGISTER:
			value1 = read_dr(comp, source.value, size);
			break;
		case MEMORY_ADDR:	
			printf("Can't exchange memory adresses\n");
			return;
	}
	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't exchange raw numbers\n");
			return;

		case ADDR_REGISTER:
			value2 = read_ar(comp, dest.value, size);
			write_ar(comp, dest.value, value1, size);
			break;

		case DATA_REGISTER:
			value2 = read_dr(comp, dest.value, size);
			write_dr(comp, dest.value, value1, size);
			break;

		case MEMORY_ADDR:
			printf("Can't exchange memory adresses\n");
			return;
	}

	if (source.type == ADDR_REGISTER)	
		write_ar(comp, source.value, value2, size);
	else	
		write_dr(comp, source.value, value2, size);
}

void clear(struct Compiler* comp, int size, struct Operand source) {
	if (size == -1) {
		printf("Clear missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			printf("Can't clear a raw number\n"); 
			return;
		case ADDR_REGISTER:
			write_ar(comp, source.value, 0, size);
			break;
		case DATA_REGISTER:
			write_dr(comp, source.value, 0, size);
			break;
		case MEMORY_ADDR:	
			write_mem(comp, source.value, 0, size);
			break;
	}
	comp -> ccr_n = 0;
	comp -> ccr_z = 1;
	comp -> ccr_v = 0;
	comp -> ccr_c = 0;
}

void not(struct Compiler* comp, int size, struct Operand source) {
	int value;
	int xor_value;

	if (size == -1) {
		printf("Not missing a format specifier\n");
		return;
	}

	switch (size) {
		case 1:
			xor_value = 0xFF;
			break;
		case 2:
			xor_value = 0xFFFF;
			break;
		default:
			xor_value = 0xFFFFFFFF;
	};
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			printf("Can't not a raw number\n"); 
			return;
		case ADDR_REGISTER:
			printf("Can't not an address register\n"); 
			return;
		case DATA_REGISTER:
			value = read_dr(comp, source.value, size);
			value = value ^ xor_value;
			write_dr(comp, source.value, value, size);
			break;
		case MEMORY_ADDR:	
			value = read_mem(comp, source.value, size);
			value = value ^ xor_value;
			write_mem(comp, source.value, value, size);
			break;
	}
	
	if (value < 0)
		comp -> ccr_n = 1;
	else
		comp -> ccr_n = 0;

	if (value == 0) 
		comp -> ccr_z = 1;
	else
		comp -> ccr_z = 0;
	
	comp -> ccr_v = 0;
	comp -> ccr_c = 0;
}

void and(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value;
	
	if (size == -1) {
		printf("And missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			value = source.value;
			break;
		case ADDR_REGISTER:
			value = read_ar(comp, source.value, size);
			break;
		case DATA_REGISTER:
			value = read_dr(comp, source.value, size);
			break;
		case MEMORY_ADDR:	
			value = read_mem(comp, source.value, size);
			break;
	}
	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't perform an and to a raw number\n");	
			return;

		case ADDR_REGISTER:
			value = value & read_ar(comp, dest.value, size);
			write_ar(comp, dest.value, value, size); 
			break;

		case DATA_REGISTER:
			value = value & read_dr(comp, dest.value, size);
			write_dr(comp, dest.value, value, size); 
			break;

		case MEMORY_ADDR:
			if (source.type == MEMORY_ADDR) {
				printf("Can't and two memory addresses directly\n");
				return;
			}
			value = value & read_mem(comp, dest.value, size);
			write_mem(comp, dest.value, value, size);
			break;
	}
	if (value < 0) 
		comp -> ccr_n = 1;
	else
		comp -> ccr_n = 0;

	if (value == 0)
		comp -> ccr_z = 1;
	else
		comp -> ccr_z = 0;	
	comp -> ccr_c = 0;
	comp -> ccr_v = 0;
}


void or(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value;
	
	if (size == -1) {
		printf("Or missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			value = source.value;
			break;
		case ADDR_REGISTER:
			value = read_ar(comp, source.value, size);
			break;
		case DATA_REGISTER:
			value = read_dr(comp, source.value, size);
			break;
		case MEMORY_ADDR:	
			value = read_mem(comp, source.value, size);
			break;
	}
	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't perform an or to a raw number\n");	
			return;

		case ADDR_REGISTER:
			value = value | read_ar(comp, dest.value, size);
			write_ar(comp, dest.value, value, size); 
			break;

		case DATA_REGISTER:
			value = value | read_dr(comp, dest.value, size);
			write_dr(comp, dest.value, value, size); 
			break;

		case MEMORY_ADDR:
			if (source.type == MEMORY_ADDR) {
				printf("Can't or two memory addresses directly\n");
				return;
			}
			value = value | read_mem(comp, dest.value, size);
			write_mem(comp, dest.value, value, size);
			break;
	}
	if (value < 0) 
		comp -> ccr_n = 1;
	else
		comp -> ccr_n = 0;

	if (value == 0)
		comp -> ccr_z = 1;
	else
		comp -> ccr_z = 0;	
	comp -> ccr_c = 0;
	comp -> ccr_v = 0;
}


void xor(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value;
	
	if (size == -1) {
		printf("Xor missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			value = source.value;
			break;
		case ADDR_REGISTER:
			value = read_ar(comp, source.value, size);
			break;
		case DATA_REGISTER:
			value = read_dr(comp, source.value, size);
			break;
		case MEMORY_ADDR:	
			value = read_mem(comp, source.value, size);
			break;
	}
	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't perform a xor to a raw number\n");	
			return;

		case ADDR_REGISTER:
			value = value ^ read_ar(comp, dest.value, size);
			write_ar(comp, dest.value, value, size); 
			break;

		case DATA_REGISTER:
			value = value ^ read_dr(comp, dest.value, size);
			write_dr(comp, dest.value, value, size); 
			break;

		case MEMORY_ADDR:
			if (source.type == MEMORY_ADDR) {
				printf("Can't xor two memory addresses directly\n");
				return;
			}
			value = value ^ read_mem(comp, dest.value, size);
			write_mem(comp, dest.value, value, size);
			break;
	}
	if (value < 0) 
		comp -> ccr_n = 1;
	else
		comp -> ccr_n = 0;

	if (value == 0)
		comp -> ccr_z = 1;
	else
		comp -> ccr_z = 0;	
	comp -> ccr_c = 0;
	comp -> ccr_v = 0;
}


void bset(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value1, value2;
	int former_bit;
	
	if (size == -1) {
		printf("Bset missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			value1 = source.value;
			break;
		case ADDR_REGISTER:
			value1 = read_ar(comp, source.value, size);
			break;
		case DATA_REGISTER:
			printf("Cannot bset with data register as source\n");
			return;
		case MEMORY_ADDR:	
			printf("Cannot bset with memory as source\n");
			return;
	}
	

	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't perform a bset to a raw number\n");	
			return;

		case ADDR_REGISTER:
			printf("Can't perform a bset to an address register\n");	
			return;

		case DATA_REGISTER:
			value2 = read_dr(comp, dest.value, size);
			break;

		case MEMORY_ADDR:
			if (source.type == MEMORY_ADDR) {
				printf("Can't bset two memory addresses directly\n");
				return;
			}
			value2 = read_mem(comp, dest.value, size);
			break;
	}
	
	// We keep the former value of the set bit
	if (value2 != (value2 | (1<<(value1 % (8 * size)))))
		former_bit = 0;
	else
		former_bit = 1;
	
	value2 = value2 | (1<<(value1 % (8 * size)));

	if (dest.type == DATA_REGISTER)
		write_dr(comp, dest.value, value2, size); 
	else
		write_mem(comp, dest.value, value2, size);

	comp -> ccr_z = 1 - former_bit;
}


void bclr(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value1, value2;
	int former_bit;
	
	if (size == -1) {
		printf("Bclr missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			value1 = source.value;
			break;
		case ADDR_REGISTER:
			value1 = read_ar(comp, source.value, size);
			break;
		case DATA_REGISTER:
			printf("Cannot bclr with data register as source\n");
			return;
		case MEMORY_ADDR:	
			printf("Cannot bclr with memory as source\n");
			return;
	}
	

	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't perform a bclr to a raw number\n");	
			return;

		case ADDR_REGISTER:
			printf("Can't perform a bclr to an address register\n");	
			return;

		case DATA_REGISTER:
			value2 = read_dr(comp, dest.value, size);
			break;

		case MEMORY_ADDR:
			if (source.type == MEMORY_ADDR) {
				printf("Can't bclr two memory addresses directly\n");
				return;
			}
			value2 = read_mem(comp, dest.value, size);
			break;
	}
	
	// We keep the former value of the cleared bit
	if (value2 != (value2 | (1<<(value1 % (8 * size)))))
		former_bit = 0;
	else {
		former_bit = 1;
		value2 = value2 - (1<<(value1 % (8 * size)));
	}

	if (dest.type == DATA_REGISTER)
		write_dr(comp, dest.value, value2, size); 
	else
		write_mem(comp, dest.value, value2, size);

	comp -> ccr_z = 1 - former_bit;
}

void bchg(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value1, value2;
	int former_bit;
	
	if (size == -1) {
		printf("Bchg missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			value1 = source.value;
			break;
		case ADDR_REGISTER:
			value1 = read_ar(comp, source.value, size);
			break;
		case DATA_REGISTER:
			printf("Cannot bchg with data register as source\n");
			return;
		case MEMORY_ADDR:	
			printf("Cannot bchg with memory as source\n");
			return;
	}
	

	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't perform a bchg to a raw number\n");	
			return;

		case ADDR_REGISTER:
			printf("Can't perform a bchg to an address register\n");	
			return;

		case DATA_REGISTER:
			value2 = read_dr(comp, dest.value, size);
			break;

		case MEMORY_ADDR:
			if (source.type == MEMORY_ADDR) {
				printf("Can't bchg two memory addresses directly\n");
				return;
			}
			value2 = read_mem(comp, dest.value, size);
			break;
	}
	
	// We keep the former value of the cleared bit
	if (value2 != (value2 | (1<<(value1 % (8 * size)))))
		former_bit = 0;
	else 
		former_bit = 1;

	value2 = value2 ^ (1<<(value1 % (8 * size)));

	if (dest.type == DATA_REGISTER)
		write_dr(comp, dest.value, value2, size); 
	else
		write_mem(comp, dest.value, value2, size);

	comp -> ccr_z = 1 - former_bit;
}

