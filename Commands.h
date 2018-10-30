#define EMPTY st.empty()
#define POP st.top(); st.pop()
#define PUSH(a) st.push(a)
#define TOP st.top()
#define isOK assert(!EMPTY);\
             int a = POP;   \
             assert(!EMPTY);\
             int b = POP;

#define canjmp assert(!EMPTY);\
               int a = POP;   \
               assert(!EMPTY); int b = TOP;\
               PUSH(a);

DEF_CMD(end, 65, { isend = true;})

DEF_CMD(out, 66, {

assert(!EMPTY);
int out = POP;
printf("%d\n",out);

})

DEF_CMD(push,67,{
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

DEF_CMD(pop, 68,{

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

DEF_CMD(add, 69,{
isOK;
PUSH(a+b);
})

DEF_CMD(sub, 70, {
isOK;
PUSH(a-b);
})

DEF_CMD(mul, 71,{
isOK;
PUSH(a*b);
})

DEF_CMD(div, 72,{
isOK;
PUSH(a/b);
})

DEF_CMD(sqrt,73,{

assert(!EMPTY);
int a = POP;
PUSH(sqrt(a));
})

DEF_CMD(jmp, 74,{

if(code[ip]==-1)
ip++;
else
ip = code[ip];

})
DEF_CMD(ja,75,{

canjmp;

if( a > b && code[ip]!= -1)
ip = code[ip];
else
ip++;
})

DEF_CMD(jb,76,{

canjmp;
if( a < b && code[ip]!= -1)
ip = code[ip];
else
ip++;
})

DEF_CMD(je,77,{

canjmp;
if( a == b && code[ip]!= -1)
ip = code[ip];
else
ip++;
})
DEF_CMD(call, 78,{

if(code[ip]==-1)
ip++;
else{
//        functions.push(ip+1);
ip = code[ip];
}
})

//DEF_CMD(ret,90,{
//    assert(!std::stack:: functions.empty());
//    ip = std::stack:: functions.top();
//    std::stack:: functions.pop();
//})


