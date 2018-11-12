#ifndef CPU2_COMMANDS_ENUM_H
#define CPU2_COMMANDS_ENUM_H

#define DEF_CMD(name,num, code_exp) CMD_##name = (num),

enum Commands
{
    #include"Commands.h"
};

#undef DEF_CMD

#endif //CPU2_COMMANDS_ENUM_H
