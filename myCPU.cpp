/*
 * code_t = int
 * Load_code uses scanf("%d")
 * execute() uses printf("%d)
 *
 * without pop
 * without type in push
 * */

#include "myCPU.h"
#include "assert.h"
#include "math.h"


int CPU(FILE *exe)
{

    assert(exe!=NULL);

    code_t* code;
    size_t size=0;

    code = Load_code(exe,size);

    assert(code!=NULL);

//    for(int i=0;i<size;i++)
//        printf("%d ",code[i]);
//    printf("\n");

    Execute(code,size);

    delete[]code;
    return 0;
}

code_t* Load_code(FILE *exe, size_t &size)
{
    fseek(exe, 0, SEEK_END);
    size_t exesize = ftell(exe);
    rewind(exe);

    code_t* code = new code_t[exesize];
    for(int i = 0; i < exesize;i++)
        code[i] = -1;

    size = fread(code,sizeof(code[0]),exesize,exe);

    return code;
}

int Execute(code_t* code, size_t size)
{
    std:: stack <code_t> st;

#define DEF_CMD(name, num ,code_exp) \
        case num: code_exp break;

    bool isend = false;

    for(size_t ip = 0; ip < size && !isend; ){
        //printf("swip %d\n",code[ip]);
        switch (code[ip++]) {
#include "Commands.h"
            default:
                printf("default!");
                assert(0);
        }
    }

#undef DEF_CMD

    return 0;
}
