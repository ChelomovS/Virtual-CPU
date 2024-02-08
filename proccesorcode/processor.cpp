#include "processor.h"
#include "../shared/commands.h"

proc_errors proc_constructor(Processor* proc)
{
    ASSERT(proc != nullptr)

    stack_constuctor(&proc->stack, 100);
    proc->reg[0] = 0;
    proc->reg[1] = 0;
    proc->reg[2] = 0;
    proc->reg[3] = 0;

    return proc_ok;
}

proc_errors execute(Processor* proc, FILE* translated_file)
{
    ASSERT(proc != nullptr);
    ASSERT(translated_file != nullptr);

    proc->filedata.bufSize = GetFileSize(translated_file);
    proc->filedata.buf = (byte_t*)calloc(proc->filedata.bufSize, sizeof(byte_t));
    fread(proc->filedata.buf, sizeof(byte_t), proc->filedata.bufSize, translated_file);

    fclose(translated_file);

    for (size_t pass = 0; pass < proc->filedata.bufSize; pass += sizeof(byte_t))
    {
        do_commands(proc, &proc->filedata.buf[pass]);
    }

    free(proc->filedata.buf);
    return proc_ok;
}

proc_errors do_commands(Processor* proc, byte_t* text)
{
    ASSERT(proc != nullptr);
    ASSERT(text != nullptr); 
    int cmd_id = *text; 

    if ((*(byte_t*)text + 1) == 0b0)
    {
        switch (cmd_id)
        {
            case cmd_array[0].id:
                halt();
                break;


            case cmd_array[1].id:
                add(&proc->stack);
                break;

            case cmd_array[2].id:
                sub(&proc->stack);
                break;

            case cmd_array[3].id:
                mul(&proc->stack);
                break;

            case cmd_array[4].id:
                div(&proc->stack);
                break;

            case cmd_array[8].id:
                out(&proc->stack);
                break;

            case cmd_array[7].id:
                in(&proc->stack);
                break;

        }
    }
    else 
        if (((*(byte_t*)text + 1) == CONST_MASK) || ((*(byte_t*)text + 1) == REG_MASK))
        {
            text += sizeof(byte_t);
            check_arg(proc, text, cmd_id);
        }

}
void check_arg(Processor* proc, byte_t* text, int command_id)
{
    if (command_id == 5)
        if ((*(byte_t*)text + 1) == CONST_MASK)
        {
            int const_tmp = *text;
            push(proc, 0, const_tmp);
        }
        if ((*(byte_t*)text + 1) == REG_MASK)
        {
            int reg_tmp = *text;
            push(proc, 1, reg_tmp);
        }

    if (command_id == 6)
        if ((*(byte_t*)text + 1) == CONST_MASK)
        {
            int const_tmp = *text;
            pop(proc, 0, const_tmp);
        }
        if ((*(byte_t*)text + 1) == REG_MASK)
        {
            int reg_tmp = *text;
            pop(proc, 1, reg_tmp);
        }

}

void proc_destructor(Processor* proc)
{
    ASSERT(proc != nullptr);

    FileDataBinDtor(&proc->filedata);
    stack_destructor(&proc->stack);

    
    proc->reg[0] = 0;
    proc->reg[1] = 0;
    proc->reg[2] = 0;
    proc->reg[3] = 0;
}

void proc_dump(Processor* const proc)
{
    ASSERT(proc != nullptr);

    stack_dump(&proc->stack);

    for (size_t reg_number = 0; reg_number < NUMBER_OF_REGISTRS; reg_number++)
    {
        fprintf(stderr, "%d \n", proc->reg[reg_number]);
    }
}