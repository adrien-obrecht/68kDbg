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
		source = val_of_op(comp, cmd.source, cmd.format);
		dest = val_of_op(comp, cmd.destination, cmd.format);
			
		if (strcmp("move", cmd.instruction) == 0) {
			printf("move.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			move(comp, cmd.format, source, dest);
		}
		else if (strcmp("add", cmd.instruction) == 0) {
			printf("add.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			add(comp, cmd.format, source, dest);
		}
			
		else if (strcmp("sub", cmd.instruction) == 0) {
			printf("sub.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			sub(comp, cmd.format, source, dest);
		}
			
		else if (strcmp("swap", cmd.instruction) == 0) {
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
		
		else if (strcmp("exg", cmd.instruction) == 0) {
			printf("exg.%d %s %s \n", cmd.format, cmd.source, cmd.destination);
			exchange(comp, cmd.format, source, dest);
		}
		
		else if (strcmp("clr", cmd.instruction) == 0) {
			if (cmd.destination) {
				printf("Clear only has one operand\n");
			}
			else {
				printf("clr.%d %s\n", cmd.format, cmd.source);
				clear(comp, cmd.format, source);
			}
		}
			
		else {
			printf("Unknown instruction %s\n", cmd.instruction);
		}
		comp -> command_pointer++;
	}
}
