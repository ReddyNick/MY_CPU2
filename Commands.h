#define EMPTY   st.empty()

#define POP     st.top(); st.pop()

#define PUSH(a) st.push(a)

#define TOP     st.top()

#define isOK assert(!EMPTY);\
             int a = POP;   \
             assert(!EMPTY);\
             int b = POP;

#define canjmp assert(!EMPTY);             \
               int a = POP;                \
               assert(!EMPTY); int b = TOP;\
               PUSH(a);
//___END___//
DEF_CMD(end, 65, { isend = true; })

//___OUT___//
DEF_CMD(out, 66,
{
    assert(!EMPTY);
    int out = POP;
    printf("%d\n", out);
})

//___PUSH___//
DEF_CMD(push,67,
{
    switch(code[ip++]){
        case 0:
            PUSH(code[ip++]);
            break;
        case 1:
            PUSH(RAM[code[ip++]]);
            break;
        case 2:
            PUSH(Registers[code[ip++]]);
            break;
        case 3:
            int address = Registers[code[ip++]];
            PUSH(RAM[address]);
    }
})

//___POP___//
DEF_CMD(pop, 68,
{
    assert(!EMPTY);
    switch(code[ip++]){
        case 0:
            POP; break;
            case 1:
            RAM[code[ip++]] = POP; break;
        case 2:
            Registers[code[ip++]] = POP;
            break;
        case 3:
            int address = Registers[code[ip++]];
            RAM[address] = POP;
    }
})

//___ADD___//
DEF_CMD(add, 69,
{
    isOK;
    PUSH(a+b);
})

//___SUB___//
DEF_CMD(sub, 70,
{
    isOK;
    PUSH(a-b);
})

//___MUL___//
DEF_CMD(mul, 71,
{
    isOK;
    PUSH(a*b);
})

//___DIV___//
DEF_CMD(div, 72,
{
    isOK;
    PUSH(a/b);
})

//___SQRT___//
DEF_CMD(sqrt,73,
{
    assert(!EMPTY);
    int a = POP;
    PUSH(sqrt(a));
})

//___JMP___//
DEF_CMD(jmp, 74,
{
    if(code[ip]==-1)
    ip++;
    else
    ip = code[ip];
})

//___JA___//
DEF_CMD(ja,75,
{
    canjmp;
    if( a > b && code[ip]!= -1)
        ip = code[ip];
    else
        ip++;
})

//___JB___//
DEF_CMD(jb,76,
{
    canjmp;
    if( a < b && code[ip]!= -1)
        ip = code[ip];
    else
        ip++;
})

//___JE___//
DEF_CMD(je,77,
{
    canjmp;
    if( a == b && code[ip]!= -1)
        ip = code[ip];
    else
        ip++;
})

//___CALL___//
DEF_CMD(call, 78,
{
    if(code[ip]==-1)
        ip++;
    else{
        functions.push(ip+1);
        ip = code[ip];
    }
})

//___RET___//
DEF_CMD(ret,90,
{
    assert(!functions.empty());
    ip = functions.top();
    functions.pop();
})

//___IN___//
DEF_CMD(in, 91,
{
    int num;
    scanf("%d",&num);
    PUSH(num);
})

