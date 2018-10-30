#include "myassembler.h"
#include "myCPU.h"
#include "stdio.h"
#include "string.h"


int main() {

    FILE* prog = fopen("prog.txt","r");
    FILE* exe=fopen("progexe.exe","w");

    assert(prog!=NULL);

    Assembler(prog,exe);

    fclose(exe);
    exe=fopen("progexe.exe","r");

    CPU(exe);


    fclose(prog);
    fclose(exe);

    return 0;
}