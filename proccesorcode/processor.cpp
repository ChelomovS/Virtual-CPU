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

    if ((cmd_id >= 16) && (cmd_id <= 22))
    {
        proc->ip += sizeof(cmd_t);
        check_arg(proc, proc->filedata.buf, cmd_raw);
        return proc_ok;
    }

    if ((*(proc->filedata.buf + proc->ip) & CONST_MASK) == CONST_MASK) 
    {
        proc->ip += sizeof(cmd_t);
        check_arg(proc, proc->filedata.buf, cmd_raw);
        return proc_ok;
    }

    if ((*(proc->filedata.buf + proc->ip) & REG_MASK) == REG_MASK) // NOTE macro
    {
        proc->ip += sizeof(cmd_t);
        check_arg(proc, proc->filedata.buf, cmd_raw);
        return proc_ok;
    }

    if ((*(proc->filedata.buf + proc->ip) & NO_MASK) == NO_MASK)
    {
        proc->ip += sizeof(cmd_t);
        #define DEF_CMD(cmd_name, id, is_jump, ...) \
            case id: \
                __VA_ARGS__ \
                break;
        //end of def
        
        switch (cmd_id) 
        {
            #include "../shared/cmd_def.h"

            default:
                ASSERT(0 && ":(");
                break;
        } 
    }
}

void check_arg(Processor* proc, byte_t* text, byte_t cmd_raw)
{
    byte_t command_id = cmd_raw & CMD_MASK;
    
    if (command_id == 5)
    {
        if ((cmd_raw & CONST_MASK) == CONST_MASK)
        {
            long const_tmp = *(long*)(text + proc->ip);

            push(proc, 1, const_tmp);

            proc->ip += sizeof(imm_t);
        }

        if ((cmd_raw & REG_MASK) == REG_MASK)
        {
            reg_t reg_tmp = *(reg_t*)(text + proc->ip);

            push(proc, 2, reg_tmp);

            proc->ip += sizeof(reg_t);
        }
    }
    else
    if (command_id == 6)
    {
        if ((cmd_raw & REG_MASK) == REG_MASK)
        {
            reg_t reg_tmp = *(reg_t*)(text + proc->ip);

            pop(proc, 1, reg_tmp);

            proc->ip += sizeof(reg_t);
        }
    }
    else
    if ((command_id >= 16) && (command_id <= 22))
    {
        long const_tmp = *(long*)(text + proc->ip);

        if (command_id == 16)
        {
            jmp(proc, const_tmp);
        }

        if (command_id == 17)
        {
            je(proc, const_tmp);
        }

        if (command_id == 18)
        {
            jne(proc, const_tmp);
        }

        if (command_id == 19)
        {
            ja(proc, const_tmp);
        }

        if (command_id == 20)
        {
            jea(proc, const_tmp);
        }

        if (command_id == 21)
        {
            jb(proc, const_tmp);
        }

        if (command_id == 22)
        {
            jeb(proc, const_tmp);
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