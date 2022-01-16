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
			value = read_hex(&(comp->address_register.AR[source.value][8 - 2 * size]), size);
			break;
		case DATA_REGISTER:
			value = read_hex(&(comp->data_register.DR[source.value][8 - 2 * size]), size);
			break;
		case MEMORY_ADDR:	
			value = read_hex(&(comp->memory.mem[2 * (source.value - MEM_OFFSET)]), size);
			break;
	}
	
	switch (dest.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			printf("Can't move to a raw number\n");
			break;

		case ADDR_REGISTER:
			write_hex(&comp->address_register.AR[dest.value][8 - 2 * size], value, size);		
			break;

		case DATA_REGISTER:
			write_hex(&comp->data_register.DR[dest.value][8 - 2 * size], value, size);		
			break;

		case MEMORY_ADDR:
			write_hex(&comp->memory.mem[2 * (dest.value - MEM_OFFSET)], value, size);		
			break;
	}
}


void add(struct Compiler* comp, int size, struct Operand source, struct Operand dest) {
	int value;
	
	if (size == -1) {
		printf("Addii missing a format specifier\n");
		return;
	}
	
	switch (source.type) {
		case INVALID:
			return;
		case RAW_NUMBER:
			value = source.value;
			break;
		case ADDR_REGISTER:
			value = read_hex(&(comp->address_register.AR[source.value][8 - 2 * size]), size);
			break;
		case DATA_REGISTER:
			value = read_hex(&(comp->data_register.DR[source.value][8 - 2 * size]), size);
			break;
		case MEMORY_ADDR:	
			value = read_hex(&(comp->memory.mem[2 * (source.value - MEM_OFFSET)]), size);
			break;
	}
	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't perfrom an addition to a raw number\n");	
			return;

		case ADDR_REGISTER:
			value = value + read_hex(&(comp->address_register.AR[dest.value][8 - 2 * size]), size);
			write_hex(&comp->address_register.AR[dest.value][8 - 2 * size], value, size);		
			break;

		case DATA_REGISTER:
			value = value + read_hex(&(comp->data_register.DR[dest.value][8 - 2 * size]), size);
			write_hex(&comp->data_register.DR[dest.value][8 - 2 * size], value, size);		
			break;

		case MEMORY_ADDR:
			if (source.type == MEMORY_ADDR) {
				printf("Can't add two memory addresses directly\n");
				return;
			}
			value = value + read_hex(&(comp->memory.mem[2 * (dest.value - MEM_OFFSET)]), size);
			write_hex(&comp->memory.mem[2 * (dest.value - MEM_OFFSET)], value, size);		
			break;
	}
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
			value = read_hex(&(comp->address_register.AR[source.value][8 - 2 * size]), size);
			break;
		case DATA_REGISTER:
			value = read_hex(&(comp->data_register.DR[source.value][8 - 2 * size]), size);
			break;
		case MEMORY_ADDR:	
			value = read_hex(&(comp->memory.mem[2 * (source.value - MEM_OFFSET)]), size);
			break;
	}
	switch (dest.type) {
		case INVALID:
			return;

		case RAW_NUMBER:
			printf("Can't perfrom a substraction to a raw number\n");	
			return;

		case ADDR_REGISTER:
			value = -value + read_hex(&(comp->address_register.AR[dest.value][8 - 2 * size]), size);
			write_hex(&comp->address_register.AR[dest.value][8 - 2 * size], value, size);		
			break;

		case DATA_REGISTER:
			value = -value + read_hex(&(comp->data_register.DR[dest.value][8 - 2 * size]), size);
			write_hex(&comp->data_register.DR[dest.value][8 - 2 * size], value, size);		
			break;

		case MEMORY_ADDR:
			if (source.type == MEMORY_ADDR) {
				printf("Can't add two memory addresses directly\n");
				return;
			}
			value = -value + read_hex(&(comp->memory.mem[2 * (dest.value - MEM_OFFSET)]), size);
			write_hex(&comp->memory.mem[2 * (dest.value - MEM_OFFSET)], value, size);		
			break;
	}
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
			value1 = read_hex(&(comp->address_register.AR[source.value][8 - 2 * size]), size);
			break;
		case DATA_REGISTER:
			value1 = read_hex(&(comp->data_register.DR[source.value][8 - 2 * size]), size);
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
			value2 = read_hex(&(comp->address_register.AR[dest.value][8 - 2 * size]), size);
			write_hex(&comp->address_register.AR[dest.value][8 - 2 * size], value1, size);		
			break;

		case DATA_REGISTER:
			value2 = read_hex(&(comp->data_register.DR[dest.value][8 - 2 * size]), size);
			write_hex(&comp->data_register.DR[dest.value][8 - 2 * size], value1, size);		
			break;

		case MEMORY_ADDR:
			printf("Can't exchange memory adresses\n");
			return;
	}

	if (source.type == ADDR_REGISTER)	
		write_hex(&comp->address_register.AR[source.value][8 - 2 * size], value2, size);		
	else	
		write_hex(&comp->data_register.DR[source.value][8 - 2 * size], value2, size);		
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
			write_hex(&comp->address_register.AR[source.value][8 - 2 * size], 0, size);		
			break;
		case DATA_REGISTER:
			write_hex(&comp->data_register.DR[source.value][8 - 2 * size], 0, size);		
			break;
		case MEMORY_ADDR:	
			write_hex(&comp->memory.mem[2 * (source.value - MEM_OFFSET)], 0, size);		
			break;
	}
}
