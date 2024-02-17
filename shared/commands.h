#ifndef COMMANDS_H
#define COMMANDS_H

#include "stack.h"
#include "file_lib_bin.h"

const int NUMBER_OF_COMMANDS = 9;
const int NUMBER_OF_REGISTRS = 4;

const byte_t REG_MASK   = 0b010'00000;
const byte_t CONST_MASK = 0b100'00000;
const byte_t CMD_MASK   = 0b000'11111;
const byte_t NO_MASK    = 0b000;

typedef uint8_t reg_t;
typedef uint8_t cmd_t;
typedef uint64_t imm_t;

struct BinBuf 
{
    size_t capacity;
    size_t size;
    byte_t* translated_code;
};

struct Processor
{
    Stack stack;
    FileDataBin filedata;
    long reg[4];
    size_t ip;
};

struct command 
{
    const char* name;
    const byte_t id;
    const bool have_args;
};

struct reg
{
    const char* name;
    const byte_t id; 
};

constexpr static command cmd_array[] = {
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

constexpr static reg reg_array[] = {
    {
        .name = "rax",
        .id = 1
    },

    {
        .name = "rbx",
        .id = 2
    },

    {
        .name = "rcx",
        .id = 3
    },

    {
        .name = "rdx",
        .id = 4
    }
};

int halt();
int add(Stack* stack);
int sub(Stack* stack);
int mul(Stack* stack);
int div(Stack* stack);
int push(Processor* proc, int primary_arg, long optional_arg);
int pop(Processor* proc, int primary_arg, int optional_arg);
int in(Stack* stack);
int out(Stack* stack);



#endif // COMMANDS_H