// WOOF - WOOF!

#include "myCPU.h"
#include "assert.h"
#include "math.h"

//my header
// my cpp

int CPU (FILE *exe)
{
    assert (exe!=NULL);

    size_t size = 0;

    code_t* code = Load_code (exe, &size);

    assert (code!=NULL);

    Execute (code, size);

    delete[]code;
    return 0;
}

code_t* Load_code (FILE *exe, size_t *size)
{
    //size of exe file
    fseek (exe, 0, SEEK_END);
    size_t exesize = ftell (exe);
    rewind (exe);

    code_t* code = new code_t[exesize] {-1};

    *size = fread (code, sizeof (code[0]), exesize, exe);

    return code;
}

int Execute (code_t* code, size_t size)
{
    // for functions
    std::stack <int> functions;
    // for calculations
    std::stack <code_t> st;

    code_t Registers[16] = {};
    code_t RAM[100] = {};

    #define DEF_CMD(name, num , code_exp) \
        case num: code_exp break;

    // true if end
    bool isEnd = false;

    for (size_t ip = 0; ip < size && !isEnd; )
        switch (code[ip++])
        {

        #include "Commands.h"

        default:
            printf ("default! %d ip = %d", code[ip-1], ip - 1);
            assert (0);
        }

        #undef DEF_CMD
    return 0;
}
