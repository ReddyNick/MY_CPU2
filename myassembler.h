// WOOF - WOOF!
#ifndef CPU1_MYASSEMBLER_H
#define CPU1_MYASSEMBLER_H

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"

#include "Commands_enum.h"

typedef int code_t;

struct CODE {
    code_t* c = NULL;
    size_t size = 0;
};


int Assembler (FILE* prog, FILE* exe, FILE* listing);

CODE Read_and_asm (FILE * prog, char** marks, FILE* listing, int* marks_sz, int *marks_add);

int Write (FILE* exe, CODE code);

int Generate_code (char* buf, CODE* code, char** marks, FILE* listing, int* marks_sz, int* marks_add);

int Write_HEX (code_t code, FILE* listing);

int Listing (char* buf, int type, int arg, int size, int commandnum, FILE* listin);

int Parse (char* buf, int &arg, char*reg, bool* regRAM);

int Write_arguments (char* buf, char* command, CODE* code, size_t* size,
                    int commandnum, char** marks, FILE* listing, int* marks_sz, int* marks_add);

int Command_reg(CODE* code, bool regRAM, size_t* size, int *arg,
                char* reg, int* type);

#endif //CPU1_MYASSEMBLER_H