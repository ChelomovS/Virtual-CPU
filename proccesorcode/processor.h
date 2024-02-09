#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../shared/stack.h"
#include "../shared/fileLib.h"
#include "../shared/commands.h"

enum proc_errors
{
    proc_ok                        = 0,
    proc_error                     = 1,
};

proc_errors proc_constructor(Processor* proc, FILE* translated_file);
proc_errors execute(Processor* proc);
proc_errors do_commands(Processor* proc, chunk_t* text, size_t* pass);
void check_arg(Processor* proc, chunk_t* text, byte_t command_id, size_t* pass);
void proc_dump(Processor* proc);
void proc_destructor(Processor* proc);

#endif // PROCESSOR_H