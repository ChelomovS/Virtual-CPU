#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../shared/stack.h"
#include "../shared/fileLib.h"
#include "../shared/commands.h"

typedef uint32_t chunk_t;
typedef uint8_t byte_t;

enum proc_errors
{
    proc_ok                        = 0,
    proc_error                     = 1,
};

proc_errors proc_constructor(Processor* proc);
proc_errors execute(Processor* proc, FILE* translated_file);
proc_errors do_commands(Processor* proc, byte_t* text);
void check_arg(Processor* proc, byte_t* text, int command_id);
void proc_dump(Processor* proc);
void proc_destructor(Processor* proc);

#endif // PROCESSOR_H