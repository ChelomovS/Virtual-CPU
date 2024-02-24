#ifndef COMMANDS_H
#define COMMANDS_H

#include "stack.h"
#include "file_lib_bin.h"

const int NUMBER_OF_COMMANDS = 9;
const int NUMBER_OF_REGISTRS = 4;

const byte_t REG_MASK   = 0b010'00000;
const byte_t CONST_MASK = 0b100'00000;
const byte_t MEM_MASK   = 0b001'00000;
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
int jmp(Processor* proc, long optional_arg);
int je(Processor* proc, long optional_arg);
int jne(Processor* proc, long optional_arg);
int ja(Processor* proc, long optional_arg);
int jea(Processor* proc, long optional_arg);
int jb(Processor* proc, long optional_arg);
int jeb(Processor* proc, long optional_arg);

#endif // COMMANDS_H