#include <string.h>
#include <stdio.h>

#include "register.h"
#include "utils.h"
#include "commands.h"

void do_one_iteration(struct Compiler* comp) {
	
	struct Command cmd;
	struct Operand source, dest;
	
	if (comp -> execution_speed != 0 && comp -> command_pointer < comp -> command_len) {
		
		cmd = comp -> command_list[comp -> command_pointer];
		
		if (!cmd.source) {
			comp -> command_pointer++;
			return;
		}

		source = val_of_op(comp, cmd.source, cmd.format);
		dest = val_of_op(comp, cmd.destination, cmd.format);
			
		if (strcmp("\tmove", cmd.instruction) == 0) {
			printf("move.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			move(comp, cmd.format, source, dest);
		}
		else if (strcmp("\tadd", cmd.instruction) == 0) {
			printf("add.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			add(comp, cmd.format, source, dest);
		}
			
		else if (strcmp("\tsub", cmd.instruction) == 0) {
			printf("sub.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			sub(comp, cmd.format, source, dest);
		}
			
		else if (strcmp("\tswap", cmd.instruction) == 0) {
			if (cmd.format != -1) {
				printf("Swap doesn't support custom format\n");
			}
			else if (cmd.destination) {
				printf("Swap only has one operand\n");
			}
			else {
				printf("swap %s \n", cmd.source);
				swap(comp, source);
			}
		}
		
		else if (strcmp("\texg", cmd.instruction) == 0) {
			printf("exg.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			exchange(comp, cmd.format, source, dest);
		}
		
		else if (strcmp("\tclr", cmd.instruction) == 0) {
			if (cmd.destination) {
				printf("Clear only has one operand\n");
			}
			else {
				printf("clr.%d %s\n", cmd.format, cmd.source);
				clear(comp, cmd.format, source);
			}
		}
		
		else if (strcmp("\tnot", cmd.instruction) == 0) {
			if (cmd.destination) {
				printf("Not only has one operand\n");
			}
			else {
				printf("not.%d %s\n", cmd.format, cmd.source);
				not(comp, cmd.format, source);
			}
		}

		else if (strcmp("\tand", cmd.instruction) == 0) {
			printf("and.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			and(comp, cmd.format, source, dest);
		}
		
		else if (strcmp("\tor", cmd.instruction) == 0) {
			printf("or.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			or(comp, cmd.format, source, dest);
		}
		
		else if (strcmp("\txor", cmd.instruction) == 0) {
			printf("xor.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			xor(comp, cmd.format, source, dest);
		}

		else if (strcmp("\tbset", cmd.instruction) == 0) {
			printf("bset.%d %s %s\n", cmd.format, cmd.source, cmd.destination);
			bset(comp, cmd.format, source, dest);
		}
		
		else if (strcmp("\tbclr", cmd.instruction) == 0) {
			printf("bclr.%d %s %s\n", cmd.format, cmd.source, cmd.destination);
			bclr(comp, cmd.format, source, dest);
		}
		
		else if (strcmp("\tbchg", cmd.instruction) == 0) {
			printf("bchg.%d %s %s\n", cmd.format, cmd.source, cmd.destination);
			bchg(comp, cmd.format, source, dest);
		}

		else {
			printf("Unknown instruction %s\n", cmd.instruction);
		}
		comp -> command_pointer++;
	}
}
