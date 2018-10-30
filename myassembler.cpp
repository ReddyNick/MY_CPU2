/*
 * code_t = int
 * Write() uses printf("%d")
 * Generate_code uses scanf("%d")
 * assmeble after end
 * without conditions
 * without functions
 * */

#include "myassembler.h"

constexpr int BUF_SIZE=50;

FILE* listing = fopen("listing.txt","w");
int marks[10];

int Assembler(FILE *prog, FILE *exe)
{
    assert(prog != NULL);
    assert(exe != NULL);
    assert(listing != NULL);

    CODE code;
    for(int i=0; i<10; i++) {marks[i]= -1;
        // printf("%d",marks[i]);
    }

    code = Read_and_asm(prog);

    fclose(listing);
    listing = fopen("listing.txt","w");

    code = Read_and_asm(prog);

    if(code.c==NULL) return 1;
    Write(exe,code);


    //repeat//delete code

    fclose(listing);
    delete[] code.c;
    return 0;
}
int Initialization(FILE*prog,CODE& code,char* buf){
    // size of prog
    fseek(prog, 0, SEEK_END);
    size_t capacity = ftell(prog);
    fseek(prog, 0, SEEK_SET);

    if (capacity == 0) return 1;

    code.c = new code_t[capacity];

    for(int i = 0; i < capacity; i++)
        code.c[i] = -1;

    memset(buf, '\0', BUF_SIZE);

    return 0;
}
CODE Read_and_asm(FILE* prog)
{
    CODE code;
    char buf[BUF_SIZE];

    Initialization(prog,code,buf);


    while(fgets(buf, BUF_SIZE, prog) != NULL){
        if (buf[0] == '\n') continue;
        Generate_code(buf, code);
    }

    return code;
}

int Generate_code(char* buf, CODE& code)
{

    char command[BUF_SIZE]= "";

    if(sscanf(buf," %s ",command) ==0) {
        printf("ERROR2 OF ASSEMBLING!");
        abort();
    }

    //if mark
    if(buf[1] ==':'){
        marks[buf[0]-'0']= code.size;
        return 0;
    }

    size_t size = code.size;
    int commandnum=-1;

#define DEF_CMD(name,num,code_exp) \
    if(strcmp(command, #name) == 0){ \
        code.c[size++] = commandnum = CMD_##name; \
        }else

#include "Commands.h"
    if(0);
#undef DEF_CMD

    int arg=-1;
    char reg[5]="";

    bool regRAM = false;
    int type = Parse(buf,arg,reg,regRAM);


#define register(x,num)  \
    if(strcmp(reg,x) == 0) { \
    code.c[size++] = (num);  \
    arg = (num); }

    if(commandnum >= CMD_jmp && commandnum <= CMD_call ){
        if(sscanf(buf," %*s %d:",&arg) == -1) {
            printf("ERROR jump without destination");
            abort();
        }
        assert(arg<10);
        type = 5;
        // printf("arg %d\n",arg);
        code.c[size++] = arg = marks[arg];
    }
    else{

        switch(type)
        {
            case -1:
                if(strcmp(command,"pop") == 0)
                    code.c[size++] = 0;
                break;

            case 0:
                code.c[size++] = 0;
                code.c[size++] = arg;
                break;

            case 1:
                code.c[size++] = 1;
                code.c[size++] = arg;
                break;

            case 2:
                if(regRAM){
                    code.c[size++] = 3;
                    type = 3;
                }
                else{
                    code.c[size++] = 2;
                    type = 2;
                }
                register("ax",0)
                else
                register("bx",1)
                else
                register("cx",2)
                else
                register("dx",3)

                else if(reg[0] == 'R') {
                    arg = reg[1] - '0';

                    if(reg[2] != '\0')
                        arg = reg[2]-'0' + 10*arg;

                    code.c[size++] = arg;
                }
                break;

            default:
                printf("Ooops! default");
                abort();
        }
    }

    code.size = size;
    Listing(buf,type,arg,size,commandnum);

    return 0;
}

int Parse(char* buf, int &arg, char* reg, bool& regRAM)
{
    int max=0, cur = 0, type = -1;

    cur = sscanf(buf,"%*s %d ", &arg);
    if (cur > max) { max = cur; type = 0;}

    cur = sscanf(buf,"%*s [ %d ] ", &arg);
    if (cur > max) { max = cur; type = 1;}

    cur = sscanf(buf,"%*s [ %[^] ] ", reg);

    if (cur > max) { max = cur; type = 2; regRAM=true;}

    if(!regRAM){
        cur = sscanf(buf,"%*s %s ", reg);
        if (cur > max) { max = cur; type = 2;}
    }

    return type;
}

int Listing(char* buf,int type,int arg,int size,int commandnum)
{
    if(listing == NULL) return 1;
    if(type == -1) {
        //address
        fprintf(listing, "%04d    ", size - 1);
        //command
        Write_HEX(commandnum);

        fprintf(listing,"%31c %s", '|', buf);
    }
    else
    if(type == 5){
        fprintf(listing, "%04d    ", size - 2);
        Write_HEX(commandnum);
        Write_HEX(arg);
        fprintf(listing,"%16c %s",'|',buf);
    }
    else{
        //address
        fprintf(listing, "%04d    ", size - 3);
        //command
        Write_HEX(commandnum);
        // type
        Write_HEX(type);
        //argument
        Write_HEX(arg);

        fprintf(listing,"| %s",buf);
    }

    return 0;
}

int Write_HEX(code_t code)
{
    char* ptr = (char*)&code;

    for(int j = 0; j < 4; j++)
        fprintf(listing, "%02X ", (unsigned char)*(ptr++));
    fprintf(listing, "   ");

    return 0;
}

int Write(FILE* exe,CODE code)
{
    fwrite(code.c, sizeof(code.c[0]),code.size,exe);
    return 0;
}
