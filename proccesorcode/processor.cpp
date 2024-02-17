#include "processor.h"
#include "../shared/commands.h"

proc_errors proc_constructor(Processor* proc, const int argc, const char** argv)
{
    ASSERT(proc != nullptr);
    ASSERT(argv != nullptr);

    if (argc != 2)
        return proc_too_few_files;

    FILE* translated_file = fopen(argv[1], "r");

    if (translated_file == nullptr)
        return proc_bad_open_file_for_reading;

    proc->filedata.bufSize = GetFileSize(translated_file);

    proc->filedata.buf = (byte_t*)calloc(proc->filedata.bufSize, sizeof(byte_t));
    if (proc->filedata.buf == nullptr)
        return proc_bad_alloc;

    fread(proc->filedata.buf, sizeof(byte_t), proc->filedata.bufSize, translated_file);

    fclose(translated_file);
    
    stack_constuctor(&proc->stack, 100);
    proc->reg[0] = 0;
    proc->reg[1] = 0;
    proc->reg[2] = 0;
    proc->reg[3] = 0;
    proc->ip = 0;

    return proc_ok;
}

proc_errors execute(Processor* proc)
{
    ASSERT(proc != nullptr);

    while (proc->ip < proc->filedata.bufSize)
    {   
        fprintf(stderr, "вошел в цикл \n");
        do_commands(proc);
    }

    return proc_ok;
}

proc_errors do_commands(Processor* proc)
{
    ASSERT(proc != nullptr);

    PRINT_INTX(*(proc->filedata.buf + proc->ip));
    PRINT_NUM(proc->ip);

    byte_t cmd_raw = *(proc->filedata.buf + proc->ip); 
    byte_t cmd_id = cmd_raw & CMD_MASK;

    if ((*(proc->filedata.buf + proc->ip) & CONST_MASK) == CONST_MASK) 
    {
        proc->ip += sizeof(cmd_t);
        fprintf(stderr, "нашел команду с константой \n");
        check_arg(proc, proc->filedata.buf, cmd_raw);
        return proc_ok;
    }

    if ((*(proc->filedata.buf + proc->ip) & REG_MASK) == REG_MASK)
    {
        proc->ip += sizeof(cmd_t);
        fprintf(stderr, "нашел команду с reg \n");
        check_arg(proc, proc->filedata.buf, cmd_raw);
        return proc_ok;
    }

    if ((*(proc->filedata.buf + proc->ip) & NO_MASK) == NO_MASK)
    {
        fprintf(stderr, "нашел команду без аргумента \n");
        proc->ip += sizeof(cmd_t);
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
        
            default:
                break;
        }
    }
        return proc_ok;
}

void check_arg(Processor* proc, byte_t* text, byte_t cmd_raw)
{
    byte_t command_id = cmd_raw & CMD_MASK;
    
    if (command_id == 5)
    {
        
        fprintf(stderr, "Чекаю аргумент\n");

        if ((cmd_raw & CONST_MASK) == CONST_MASK)
        {
            fprintf(stderr, "о конста\n");
            long const_tmp = *(long*)(text + proc->ip);

            push(proc, 1, const_tmp);

            proc->ip += sizeof(imm_t);
        }

        if ((cmd_raw & REG_MASK) == REG_MASK)
        {
            fprintf(stderr, "о рег\n");
            reg_t reg_tmp = *(reg_t*)(text + proc->ip);

            push(proc, 2, reg_tmp);

            proc->ip += sizeof(reg_t);
        }
    }
    if (command_id == 6)
    {
        if ((cmd_raw & REG_MASK) == REG_MASK)
        {
            fprintf(stderr, "о рег\n");
            reg_t reg_tmp = *(reg_t*)(text + proc->ip);

            pop(proc, 1, reg_tmp);

            proc->ip += sizeof(reg_t);
        }
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

void proc_dump(Processor* proc)
{
    ASSERT(proc != nullptr);

    stack_dump(&proc->stack);

    for (size_t reg_number = 0; reg_number < NUMBER_OF_REGISTRS; reg_number++)
    {
        fprintf(stderr, "%ld \n", proc->reg[reg_number]);
    }   
}

void check_errors(proc_errors error)
{
    switch(error)
    {
        case proc_ok:
            break;

        case proc_bad_alloc:
            fprintf(stderr, RED "Realloc error\n" BLACK);
            break;

        case proc_bad_open_file_for_reading:
            fprintf(stderr, RED "Bad open file for reading\n" BLACK);
            break;

        case proc_too_few_files:
            fprintf(stderr, RED "Too few files\n" BLACK);
            break;

        case proc_invalid_file:
            fprintf(stderr, RED "Invalid file");
            break;

        default:
            ASSERT(0 && "UNKNOWN ERROR");
            break;

    }
}