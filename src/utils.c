#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "register.h"
#include "utils.h"

int hex_to_int(char c) {
	// Converts an hex char to it's decimal value
	if (c <= '9' && c >= '0')
		return c-'0';
	switch (c) {
		case 'A': case 'a':
			return 10;
		case 'B': case 'b':
			return 11;
		case 'C': case 'c':
			return 12;
		case 'D': case 'd':
			return 13;
		case 'E': case 'e':
			return 14;
		case 'F': case 'f':
			return 15;
	}
	printf("Error converting : %c isn't a valid hexadecimal character\n", c);
	return -1;
}

int dec_to_int(char c) {
	// Converts a decimal char to it's value 
	if (c <= '9' && c >= '0')
		return c-'0';
	printf("Error converting : %c isn't a valid decimal character\n", c);
	return -1;
}

int bin_to_int(char c) {
	// Converts a binary char to it's value
	if (c <= '1' && c >= '0')
		return c-'0';
	printf("Error reading from memory : %c isn't a valid binary character\n", c);
	return -1;
}

struct Operand val_of_op(struct Compiler* comp, char* op, int size) {
	//  Parses an operand and returns the corresponding Operand object. Can return INVALID operand in case of failure
	
	struct Operand operand;
	operand.value = 0;
	operand.type = MEMORY_ADDR;
	
	if (!op) {
		goto EXIT_ERROR;
	}

	int pos=0, value;
	bool saw_type=false, saw_format=false, saw_number=false;
	int increase = 0;
	char format = 'd';

	// Iter through the string, keeping track of what type the operand can be
	while (op[pos] != '\0') {
		switch (op[pos]) {
			case '#' :
				// Specific for a raw number
				if (saw_number || saw_format || saw_type) {
					printf("Error in op format : didn't expect # here\n");
					goto EXIT_ERROR;
				}
				saw_type = true;
				operand.type = RAW_NUMBER;
				break;

			case '$':
				// Hexadecimal token
				if (saw_format || saw_number) {
					printf("Error in op format : didn't expect $ here\n");
					goto EXIT_ERROR;
				}
				saw_format = true;
				format = 'x';
				break;

			case '%':
				// Binary token
				if (saw_format || saw_number) {
					printf("Error in op format : didn't expect %% here\n");
					goto EXIT_ERROR;
				}
				saw_format = true;
				format = 'b';
				break;
			
			case 'd':
				// Means it's a data register
				if (format == 'x') {
					value = hex_to_int(op[pos]);
					if (value == -1)
						goto EXIT_ERROR;
					operand.value = operand.value * 16 + value;
					break;
				}
				if (saw_format || saw_number || saw_type) {
					printf("Error in op format : didn't expect d here\n");
					goto EXIT_ERROR;
				}
				operand.type = DATA_REGISTER;
				saw_format = true;
				saw_type = true;	
				break;

			case 'a':
				// Means it's an address register
				if (format == 'x') {
					value = hex_to_int(op[pos]);
					if (value == -1)
						goto EXIT_ERROR;
					operand.value = operand.value * 16 + value;
					break;
				}
				if (saw_format || saw_number || saw_type) {
					printf("Error in op format : didn't expect a here\n");
					goto EXIT_ERROR;
				}
				operand.type = ADDR_REGISTER;
				saw_format = true;
				saw_type = true;	
				break;

			case '-':
				// Case of -(ai) where we decrease the address register BEFORE
				if (operand.type != MEMORY_ADDR) {
					printf("Error in op format, didn't expect %c here\n", op[pos+1]);
					goto EXIT_ERROR;
				}
				increase = -1;
				pos++;
				
			case '(':
				// Pointer to memory from address register
				// Here we do an ugly case by case to fistinguish (ai), (ai)+ and -(ai)
				if (operand.type != MEMORY_ADDR) {
					printf("Error in op format, didn't expect %c here\n", op[pos+1]);
					goto EXIT_ERROR;
				}
				if (strlen(op) - pos == 4) {
					if (op[pos+1] != 'a') {
						printf("Error in op format, didn't expect %c here\n", op[pos+1]);
						goto EXIT_ERROR;
					}
					if (op[pos+2] < '0' || op[pos+2] > '7') {
						printf("%c is not a valid data register nam\n", op[pos+2]);
						goto EXIT_ERROR;
					}
					if (op[pos+3] != ')') {
						printf("Error in op format, didn't expect %c here\n", op[pos+3]);
						goto EXIT_ERROR;
					}
					if (increase == -1)
						write_hex(comp->address_register.AR[op[pos+2] - '0'], read_hex(comp->address_register.AR[op[pos+2] - '0'], 4) - size, 4); // sub one to the ar
					operand.value = operand.value + read_hex(comp->address_register.AR[op[pos+2] - '0'], 4);
					pos = pos + 3;
				}
				else if(strlen(op) - pos == 5) {
					if (op[pos+1] != 'a') {
						printf("Error in op format, didn't expect %c here\n", op[pos+1]);
						goto EXIT_ERROR;
					}
					if (op[pos+2] < '0' || op[pos+2] > '7') {
						printf("%c is not a valid data register nam\n", op[pos+2]);
						goto EXIT_ERROR;
					}
					if (op[pos+3] != ')') {
						printf("Error in op format, didn't expect %c here\n", op[pos+3]);
						goto EXIT_ERROR;
					}
					if (op[pos+4] != '+') {
						printf("Error in op format, didn't expect %c here\n", op[pos+4]);
						goto EXIT_ERROR;
					}
					if (increase != 0) {
						printf("Can't increase and decrease an address register at the same time\n");
					}
					operand.value = operand.value + read_hex(comp->address_register.AR[op[pos+2] - '0'], 4);
					write_hex(comp->address_register.AR[op[pos+2] - '0'], read_hex(comp->address_register.AR[op[pos+2] - '0'], 4) + size, 4); // add one to the ar
					pos = pos + 4;
				}
				else {
					printf("Error in op format\n");
					goto EXIT_ERROR;
				}
				break;

			default:
				// It can only be a number now
				saw_number = true;
				switch (format) {
					case 'x':
						value = hex_to_int(op[pos]);
						if (value == -1) 
							goto EXIT_ERROR;
						operand.value = operand.value * 16 + value;
					    break;
					case 'd':	   
						value = dec_to_int(op[pos]);
						if (value == -1) 
							goto EXIT_ERROR;
						operand.value = operand.value * 10 + value;
					    break;
					case 'b':
						value = bin_to_int(op[pos]);
						if (value == -1) 
							goto EXIT_ERROR;
						operand.value = operand.value * 2 + value;
						break;
				}
		}	
		pos++;
	}
	
	// Now we check if what we found is coherent (checks for out of bound)

	switch (operand.type) {
		case MEMORY_ADDR:
			if (operand.value < MEM_OFFSET || operand.value + 4 > MEM_OFFSET + MEM_SIZE) {
				printf("Memory address %d is out of bound\n", operand.value);
				goto EXIT_ERROR;
			}
			break;
		
		case ADDR_REGISTER:
			if (operand.value > 7) {
				printf("Address register %d doesn't exist\n", operand.value);
				goto EXIT_ERROR;
			}
			break;
		
		case DATA_REGISTER:
			if (operand.value > 7) {
				printf("Data register %d doesn't exist\n", operand.value);
				goto EXIT_ERROR;
			}
		
		default:
			break;
	}


	return operand;


	EXIT_ERROR:
		// Whenever we fail parsing, we can fallback to this label and exit safely
		operand.type = INVALID;
		return operand;	
}	


int size_of_type(char type) {
	// Converts the type (byte, word and long) to it's corresponding byte length
	switch (type) {
		case('b'):
			return 1;
		case('w'):
			return 2;
		case('l'):
			return 4;
	}
	printf("ERROR : Unknown type %c\n", type);
	return -1;
}

int read_hex(char* tab, int size) {
	// Reads <size> bytes of data from <tab>, converting it from hex
	int r = 0, num;
	for (int i = 0; i < 2 * size; i++) {
		num = hex_to_int(tab[i]);
		if (num == -1)
			return -1;
		r = 16 * r + num;
	}
	return r;
}

int read_mem(struct Compiler* comp, int position, int size) {
	// Reads <size> bytes at <psoition> of <comp>
	return read_hex(&(comp -> memory.mem[2 * (position - MEM_OFFSET)]), size);
}

int read_dr(struct Compiler* comp, int position, int size) {
	// Reads <size> bytes of <domp> data register number <position>
	return read_hex(&(comp -> data_register.DR[position][8 - 2 * size]), size);	
}

int read_ar(struct Compiler* comp, int position, int size) {
	// Reads <size> bytes of <domp> address register number <position>
	return read_hex(&(comp -> address_register.AR[position][8 - 2 * size]), size);	
}

void write_hex(char* tab, int data, int size) {
	// Writes <size> bytes of <data> to <tab>, in hex format
	char buff[1000]; 
	char bbuff[100];

	sprintf(bbuff, "%%0%dX", 2 * size); 
	sprintf(buff, bbuff, data);
	for (int i = 0; i < 2 * size; i++) {
		tab[i] = buff[strlen(buff) - 2 * size + i];
	}
};

void write_mem(struct Compiler* comp, int position, int data, int size) {
	// Writes <size> bytes of <data> at <position> in the memory
	write_hex(&comp->memory.mem[2 * (position - MEM_OFFSET)], data, size);
	for (int i = 0; i < size; i++)
		comp->memory.modified[position+i] = 1; // We mark the modified regions to highlight them in gui
}

void write_ar(struct Compiler* comp, int position, int data, int size) {
	// Writes <size> bytes of <data> in the address register <position>
	write_hex(&(comp -> address_register.AR[position][8 - 2 * size]), data, size);
}

void write_dr(struct Compiler* comp, int position, int data, int size) {
	// Writes <size> bytes of <data> in the data register <position>
	write_hex(&(comp -> data_register.DR[position][8 - 2 * size]), data, size);
}

int find_label(struct Compiler* comp, char* label) {
	// Finds the position of the label <label> in the instruction list. Returns -1 if it doesn't exist
	char* l;

	l = malloc(sizeof(char) * strlen(label + 2));
	sprintf(l, "%s:", label);
	for (int i = 0; i < comp -> command_len; i++) {
		if (strcmp(comp -> command_list[i].instruction, l) == 0) {
			free(l);
			return i;
		}	
	}	
	free(l);
	return -1;
}
