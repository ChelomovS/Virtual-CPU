#include "processor.h"
#include "../shared/commands.h"

#define ADRESS_BYTE(TEXT, PASS, BYTE_I)\
    ((byte_t*)(TEXT) + *(PASS) * sizeof(chunk_t) + BYTE_I)

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

    if (proc->filedata.bufSize % sizeof(chunk_t) != 0)
    {
        fclose(translated_file);

        return proc_invalid_file;
    }

    proc->filedata.buf = (chunk_t*)calloc(proc->filedata.bufSize, sizeof(byte_t));
    if (proc->filedata.buf == nullptr)
        return proc_bad_alloc;

    fread(proc->filedata.buf, sizeof(byte_t), proc->filedata.bufSize, translated_file);

    fclose(translated_file);
    
    stack_constuctor(&proc->stack, 100);
    proc->reg[0] = 0;
    proc->reg[1] = 0;
    proc->reg[2] = 0;
    proc->reg[3] = 0;

    return proc_ok;
}

proc_errors execute(Processor* proc)
{
    ASSERT(proc != nullptr);

    size_t pass = 0;

    for (pass = 0; pass < proc->filedata.bufSize / sizeof(chunk_t);)
    {   
        do_commands(proc, proc->filedata.buf, &pass);
    }

    return proc_ok;
}

proc_errors do_commands(Processor* proc, chunk_t* text, size_t* pass)
{
    ASSERT(proc != nullptr);
    
    byte_t cmd_id = *ADRESS_BYTE(text, pass, 0);

    if (*ADRESS_BYTE(text, pass, 1) == CONST_MASK) 
    {
        check_arg(proc, proc->filedata.buf, cmd_id, pass);

        return proc_ok;
    }

    if (*ADRESS_BYTE(text, pass, 1) == REG_MASK)
    {
        check_arg(proc, proc->filedata.buf, cmd_id, pass);

        return proc_ok;
    }

    if (*ADRESS_BYTE(text, pass, 1) == 0b0) // def no_mask 
    {
        (*pass)++;

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

void check_arg(Processor* proc, chunk_t* text, byte_t command_id, size_t* pass)
{
    if (command_id == 5)
    {
        if (*ADRESS_BYTE(text, pass, 1) == CONST_MASK)
        {
            (*pass)++;

            int const_tmp = (byte_t)*(ADRESS_BYTE(text, pass, 0));

            push(proc, 1, const_tmp);
            (*pass)++;
        }

        if (*ADRESS_BYTE(text, pass, 1) == REG_MASK)
        {
            (*pass)++;

            int reg_tmp = (byte_t)*(ADRESS_BYTE(text, pass, 0));

            push(proc, 0, reg_tmp);
            (*pass)++;
        }
    }
    if (command_id == 6)
    {
        if (*ADRESS_BYTE(text, pass, 1) == CONST_MASK)
        {
            (*pass)++;

            int const_tmp = (byte_t)*(ADRESS_BYTE(text, pass, 0));

            pop(proc, 0, const_tmp);
            (*pass)++;
        }

        if (*ADRESS_BYTE(text, pass, 1) == REG_MASK)
        {
            (*pass)++;

            int reg_tmp = (byte_t)*(ADRESS_BYTE(text, pass, 0));
            pop(proc, 1, reg_tmp);

            (*pass)++;
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
        fprintf(stderr, "%d \n", proc->reg[reg_number]);
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


        default:
            ASSERT(0 && "UNKNOWN ERROR");
            break;

    }
}