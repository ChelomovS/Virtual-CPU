#include "processor.h"
#include "commands.h"

proc_errors proc_constructor(Processor* proc)
{
    ASSERT(proc != nullptr)

    stack_constuctor(&proc->stack, 100);
    proc->reg[0] = 0;
    proc->reg[1] = 0;
    proc->reg[2] = 0;
    proc->reg[3] = 0;
    proc->reg[4] = 0;

    return proc_ok;
}
proc_errors execute(Processor* proc, FILE* translated_file)
{
    ASSERT(proc != nullptr);
    ASSERT(translated_file != nullptr);

    ParseFile(translated_file, &proc->filedata);

    for (size_t pass = 0; pass < proc->filedata.textSize; pass++)
    {
        do_commands(proc->filedata.text[pass]);
    }

    return proc_ok;
}

proc_errors do_commands(Processor* proc, const char* text)
{
    int cmd_id = 0;
    double primary_arg = 0;
    double optional_arg = 0;

    if (sscanf(text, "%d%lf%lf", &cmd_id, &primary_arg, &optional_arg) == 1)
        switch (cmd_id)
        {
            case cmd_array[0].id:
            halt();

            case cmd_array[1].id:
            add(&proc->stack);

            case cmd_array[2].id:
            sub(&proc->stack);

            case cmd_array[3].id:
            mul(&proc->stack);

            case cmd_array[4].id:
            div(&proc->stack);

            case cmd_array[5].id:
            push(&proc->stack, primary_arg, optional_arg);

            case cmd_array[6].id:
            pop(&proc->stack, primary_arg, optional_arg);

            case cmd_array[7].id:
            in(&proc->stack);

            case cmd_array[8].id:
            out(&proc->stack);
        }

    return proc_ok;
}

void proc_destructor(Processor* proc)
{
    ASSERT(proc != nullptr);

    proc->filedata.textSize = 0;
    proc->filedata.bufSize = 0;
    free(&proc->filedata);

    proc->stack.capacity = 0;
    proc->stack.size = 0;
    free(&proc->stack);

    
    proc->reg[1];
    proc->reg[2];
    proc->reg[3];
    proc->reg[4];
}

void proc_dump(Processor* proc)
{
    ASSERT(proc != nullptr);

    stack_dump(&proc->stack);

    for (size_t reg_number = 0; reg_number < NUMBER_OF_REGISTRS; reg_number++)
    {
        fprintf(stderr, "%lf", proc->reg[reg_number]);
    }

    fprintf(stderr, "%d", &proc->filedata.textSize);
    fprintf(stderr, "%s", &proc->filedata.text);
}