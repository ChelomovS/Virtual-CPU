#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../shared/stack.h"
#include "../shared/fileLib.h"
#include "../shared/commands.h"

enum proc_errors
{
    proc_ok                        = 0,
    proc_bad_open_file_for_reading = 1,
    proc_invalid_file              = 2,
    proc_bad_alloc                 = 3,
    proc_too_few_files             = 4,
};

proc_errors proc_constructor(Processor* proc, const int argc, const char** argv);
proc_errors execute(Processor* proc);
proc_errors do_commands(Processor* proc, chunk_t* text, size_t* pass);
void check_arg(Processor* proc, chunk_t* text, byte_t command_id, size_t* pass);
void proc_dump(Processor* proc);
void proc_destructor(Processor* proc);
void check_errors(proc_errors error);

#endif // PROCESSOR_H