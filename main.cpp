// WOOF - WOOF!
#include "myassembler.h"
#include "myCPU.h"
#include "stdio.h"
#include "string.h"





int main ()
{
    char* factorial = "fact.txt";
    char* fibonacci = "fibo.txt";
    char* square_eq = "square_eq.txt";

    char* programm = fibonacci;

    FILE* prog = fopen (programm, "r");
    FILE* exe = fopen  ("progexe.exe", "wb");
    FILE* listing = fopen ("listing2.txt", "w");

    assert (listing != NULL);
    assert (exe != NULL);
    assert (prog != NULL);

    Assembler (prog, exe, listing);

    fclose (exe);
    exe = fopen ("progexe.exe", "rb");

    CPU (exe);

    fclose (listing);
    fclose (prog);
    fclose (exe);

    return 0;
}