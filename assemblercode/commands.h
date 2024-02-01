#ifndef COMMANDS_H
#define COMMANDS_H

#include "stack.h"
#include "fileLib.h"

const int NUMBER_OF_COMMANDS = 9;
const int NUMBER_OF_REGISTRS = 4;

struct Processor
{
    Stack stack;
    FileData filedata;
    int reg[4];
};

struct command 
{
    const char* name;
    const int id;
    const bool have_args;
};

struct reg
{
    const char* name;
    const int id; 
};

command cmd_array[] = {
    {
        .name = "halt",
        .id = 0,
        .have_args = false
    },

    {
        .name = "add",
        .id = 1,
        .have_args = false
    },

    {
        .name = "sub",
        .id = 2,
        .have_args = true
    },

    {
        .name = "mul",
        .id = 3,
        .have_args = false
    },

    {
        .name = "div",
        .id = 4,
        .have_args = false
    },

    {
        .name = "push",
        .id = 5,
        .have_args = true
    },

    {
        .name = "pop",
        .id = 6,
        .have_args = true
    },

    {
        .name = "in",
        .id = 7,
        .have_args = false
    },

    {
        .name = "out",
        .id = 8,
        .have_args = false
    }
};

reg reg_array[] = {
    {
        .name = "rax",
        .id = 0
    },

    {
        .name = "rbx",
        .id = 1
    },

    {
        .name = "rcx",
        .id = 2
    },

    {
        .name = "rdx",
        .id = 3
    }
};

int halt();
int add(Stack* stack);
int sub(Stack* stack);
int mul(Stack* stack);
int div(Stack* stack);
int push(Processor* proc, int primary_arg, int optional_arg);
int pop(Processor* proc, int primary_arg, int optional_arg);
int in(Stack* stack);
int out(Stack* stack);



#endif // COMMANDS_H