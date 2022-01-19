#ifndef COMMANDS_h
#define COMMANDS_h

#include "register.h"
#include "utils.h"

void move(struct Compiler* comp, int size, struct Operand source, struct Operand dest);
void add(struct Compiler* comp, int size, struct Operand source, struct Operand dest);
void sub(struct Compiler* comp, int size, struct Operand source, struct Operand dest);
void swap(struct Compiler* comp, struct Operand source);
void exchange(struct Compiler* comp, int size, struct Operand source, struct Operand dest);
void clear(struct Compiler* comp, int size, struct Operand source);
void not(struct Compiler* comp, int size, struct Operand source);

#endif

