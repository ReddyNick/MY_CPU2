// WOOF - WOOF!

#ifndef CPU2_MYCPU_H
#define CPU2_MYCPU_H

#include <stack>
#include "stdio.h"

typedef int code_t;

int CPU (FILE* exe);

code_t* Load_code (FILE* exe, size_t* size);

int Execute (code_t* code, size_t size);

#endif //CPU2_MYCPU_H
