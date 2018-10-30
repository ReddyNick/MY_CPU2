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
    size_t size=0;
};


int Assembler(FILE* prog, FILE* exe);

CODE Read_and_asm(FILE * prog);

int Write(FILE* exe, CODE code);



int Generate_code(char* buf, CODE& code);

int Write_HEX(code_t code);

int Listing(char* buf,int type,int arg,int size,int commandnum);

int Parse(char* buf, int &arg,char*reg,bool& regRAM);

#endif //CPU1_MYASSEMBLER_H