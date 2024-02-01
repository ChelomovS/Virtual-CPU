#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack.h"
#include "fileLib.h"
#include "commands.h"

struct Processor
{
    Stack stack;
    FileData filedata;
    double reg[4];
};

enum proc_errors
{
    proc_ok                        = 0,
    proc_error                     = 1,
};

proc_errors proc_constructor(Processor* proc);
proc_errors execute(Processor* proc, FILE* translated_file);
proc_errors do_commands(Processor* proc, const char* text);
void proc_dump(Processor* proc);
void proc_destructor(Processor* proc);



#endif // PROCESSOR_H