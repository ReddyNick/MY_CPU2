// WOOF - WOOF!

#include "myassembler.h"

constexpr int BUF_SIZE = 50;
const int MARK_SIZE = 100;


int Assembler (FILE *prog, FILE *exe, FILE *listing)
{
    assert (prog != NULL);
    assert (exe != NULL);
    assert (listing != NULL);

    //int marks[10][10] ;//= {'\0'};

    char** marks = new char* [MARK_SIZE];
    for (int i = 0; i < MARK_SIZE; i++)
        marks[i] = NULL;

    int marks_sz = 0;
    int marks_add[MARK_SIZE] = {-1};

    //array for the code
    CODE code = Read_and_asm (prog, marks, listing, &marks_sz, marks_add);

//    for (int i = 0; i < marks_sz; i++)
//    {
//        printf ("%s %d\n", marks[i], marks_add[i]);
//    }


    // repeat for marks
    marks_sz = 0;
    fclose (listing);
    listing = fopen ("listing.txt", "w");

    code = Read_and_asm (prog, marks, listing, &marks_sz, marks_add);
    Write (exe, code);

   // delete[]
    delete[] code.c;

    return 0;
}

int Initialization (FILE* prog, CODE* code, char* buf)
{
    // size of program
    fseek (prog, 0, SEEK_END);
    size_t capacity = ftell (prog);
    fseek (prog, 0, SEEK_SET);

    if (capacity == 0) return 1;

    code->c = new code_t[capacity];

    for (int i = 0; i < capacity; i++)
        code->c[i] = -1;

    memset (buf, '\0', BUF_SIZE);

    return 0;
}

CODE Read_and_asm (FILE* prog, char** marks, FILE* listing, int* marks_sz, int* marks_add)
{
    CODE code = {};
    char buf[BUF_SIZE] = "";

    Initialization (prog, &code, buf);

    while (fgets (buf, BUF_SIZE, prog) != NULL)
    {
        if (buf[0] == '\n' || buf[0] == '/') continue;
        int code_of_operation = 0;
        code_of_operation = Generate_code (buf    , &code   , marks,
                                           listing, marks_sz, marks_add);
        if (code_of_operation)
        {
            printf("Smth went wrong. Code_of_operation = %d", code_of_operation);
            abort();
        }

    }

    return code;
}

int Generate_code (char* buf, CODE* code, char** marks, FILE* listing, int* marks_sz, int* marks_add)
{
    char command[BUF_SIZE]= "";

    sscanf (buf, " %s ", command);

    //if mark
    char* endmark = NULL;
    if ((endmark = strchr (command, ':')) != NULL)
    {
        if (*marks_sz >= MARK_SIZE) return 1;

        if (marks[*marks_sz] == NULL)
        {
            *endmark = '\0';

            marks[*marks_sz] = new char[BUF_SIZE];
            strcpy (marks[*marks_sz], command);

            marks_add[*marks_sz] = code->size;
            (*marks_sz)++;
        }

        return 0;
    }

    // current size of the code
    size_t *size = &code->size;
    // number of the command
    int commandnum = -1;

    #define DEF_CMD(name,num,code_exp)                    \
        if(strcmp(command, #name) == 0){                  \
            code->c[(*size)++] = commandnum = CMD_##name; \
        }else
    #include "Commands.h"
        if (0);          // to compensate else
    #undef DEF_CMD

    Write_arguments (buf, command, code, size,
                     commandnum, marks, listing, marks_sz, marks_add);
    return 0;
}

int Command_reg(CODE* code, bool regRAM, size_t* size, int *arg,
                char* reg, int* type)
{
    enum Type
    {
        Just_cmd = -1, Cmd_const = 0, Cmd_square = 1,
        Cmd_reg  =  2, Jmp_cmd = 5, Reg_ramcm = 3,
    };

#define register(x,num)           \
     if (strcmp(reg, x) == 0) {   \
     code->c[(*size)++] = (num);  \
     *arg = (num); }

    if (regRAM)
    {
        code->c[(*size)++] = 3;
        *type = Reg_ramcm;
    }
    else
    {
        code->c[(*size)++] = 2;
        *type = Cmd_reg;
    }
    register ("ax", 0)
    else
    register ("bx", 1)
    else
    register ("cx", 2)
    else
    register ("dx", 3)

    else if (reg[0] == 'R')
    {
        *arg = reg[1] - '0';

        if (reg[2] != '\0')
            *arg = reg[2]-'0' + 10 * (*arg);

        code->c[(*size)++] = *arg;
    }

    return 0;
}

int Write_arguments (char* buf, char* command, CODE* code, size_t* size,
                     int commandnum, char** marks, FILE*listing, int* marks_sz, int* marks_add)
{
    // argument and register if needed
    int arg = -1;
    char reg[BUF_SIZE] = "";

    // true if pop[ax] / push[R13] etc.
    bool regRAM = false;

    // type of the command
    int type = Parse (buf, arg, reg, &regRAM);

    enum Type
    {
        Just_cmd = -1, Cmd_const = 0, Cmd_square = 1,
        Cmd_reg  =  2, Jmp_cmd   = 5, Reg_ramcm  = 3,
    };

    // if JUMP-commands
    if (commandnum >= CMD_jmp && commandnum <= CMD_call )
    {
        char label[BUF_SIZE] = "";

        sscanf (buf, " %*s %[^:]", &label);
        type = Jmp_cmd;

        arg = -1;
        for (int i = 0; marks[i] != NULL; i++)
        {
            if (strcmp (marks[i], label) == 0)
            {
                arg = marks_add[i];
                break;
            }
        }
        code->c[(*size)++] = arg;
    }
    else{
        switch (type)
        {
        // just command
        case Just_cmd:
            if (strcmp (command, "pop") == 0){
                code->c[(*size)++] = arg = 0;
                type = Jmp_cmd;
            }
            break;
        // command + const
        case Cmd_const:
            code->c[(*size)++] = Cmd_const;
            code->c[(*size)++] = arg;
            break;
        // command [const]
        case Cmd_square:
            code->c[(*size)++] = Cmd_square;
            code->c[(*size)++] = arg;
            break;
        // command ax / R6 / [cx]
        case Cmd_reg:
            Command_reg(code, regRAM, size, &arg, reg, &type);
            break;

        default:
            printf ("Ooops! default %d", type);
            abort ();
        }
    }

   Listing (buf, type, arg, *size, commandnum, listing);
   return 0;
}

int Parse (char* buf, int &arg, char* reg, bool* regRAM)
{
    //??
    int max=0, cur = 0, type = -1;

    cur = sscanf (buf, " %*s %d " , &arg);
    if (cur > max) { max = cur; type = 0;}

    cur = sscanf (buf, " %*s [ %d ] ", &arg);
    if (cur > max) { max = cur; type = 1;}

    cur = sscanf (buf, " %*s [ %[^] ] ", reg);

    if (cur > max) { max = cur; type = 2; *regRAM = true;}

    if (!*regRAM)
    {
        cur = sscanf (buf, " %*s %[^ /\n\r] ", reg);
        if (cur > max) { max = cur; type = 2;}
    }

    return type;
}

int Listing (char* buf, int type, int arg, int size, int commandnum, FILE* listing)
{

    if (listing == NULL) return 1;
    if (type == -1)
    {
        //address
        fprintf (listing, "%04d    ", size - 1);
        //command
        Write_HEX (commandnum, listing);

        fprintf (listing, "%31c %s", '|', buf);
    }
    else
    if (type == 5)
    {
        fprintf (listing, "%04d    ", size - 2);
        Write_HEX (commandnum, listing);
        Write_HEX (arg, listing);

        fprintf (listing, "%16c %s", '|', buf);
    }
    else{
        //address
        fprintf (listing, "%04d    ", size - 3);
        //command
        Write_HEX (commandnum, listing);
        // type
        Write_HEX (type, listing);
        //argument
        Write_HEX (arg, listing);

        fprintf (listing, "| %s", buf);
    }

    return 0;
}

int Write_HEX (code_t code, FILE* listing)
{
    char* ptr = (char*)&code;

    for (int j = 0; j < sizeof (code_t); j++)
        fprintf (listing, "%02X ", (unsigned char)*(ptr++));
    fprintf (listing, "   ");

    return 0;
}

int Write (FILE* exe, CODE code)
{
    fwrite (code.c, sizeof (code.c[0]), code.size, exe);
    return 0;
}
