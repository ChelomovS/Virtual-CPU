#include "processor.h"
#include "../shared/commands.h"

proc_errors proc_constructor(Processor* proc, FILE* translated_file)
{
    ASSERT(proc != nullptr);
    ASSERT(translated_file != nullptr);

    proc->filedata.bufSize = GetFileSize(translated_file);

    if (proc->filedata.bufSize % sizeof(chunk_t) != 0)
    {
        fclose(translated_file);
        fprintf(stderr, ">>>BAD SIZE BUF \n");
        return proc_error;
    }

    proc->filedata.buf = (chunk_t*)calloc(proc->filedata.bufSize, sizeof(byte_t));
    
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
    printf (">>>размер буфера %d, команд должно быть: %d \n", proc->filedata.bufSize, proc->filedata.bufSize/4);

    size_t pass = 0;

    for (pass = 0; pass < proc->filedata.bufSize; pass += sizeof(chunk_t))
    {   
        printf(">>>Зашел в цикл\n");
        printf(">>>Команда номер: %d \n", pass);
        do_commands(proc, proc->filedata.buf, &pass);
    }

    return proc_ok;
}

proc_errors do_commands(Processor* proc, chunk_t* text, size_t* pass)
{
    ASSERT(proc != nullptr);

    byte_t cmd_id = (byte_t)*text;


    if (*(((byte_t*)&text) + 1) == CONST_MASK)
    {
        printf(">>>Нашел команду c аргументом!!!!!!!!\n");

        *pass += sizeof(chunk_t);

        check_arg(proc, proc->filedata.buf + *pass, cmd_id, pass);

        return proc_ok;
    }

    if (*(((byte_t*)&text) + 1) == REG_MASK)
    {
        printf(">>>Нашел команду c аргументом!!!!!!!!\n");

        *pass += sizeof(chunk_t);

        check_arg(proc, proc->filedata.buf + *pass, cmd_id, pass);

        return proc_ok;
    }

    if (*((byte_t*)&text + 1) == 0b0)
    {
        *pass += sizeof(chunk_t);
        printf(">>>Нашел команду без аргументов\n");

        switch (cmd_id)
        {
            case cmd_array[0].id:
                printf(">>>Нашел halt\n");
                halt();
                break;

            case cmd_array[1].id:
                printf(">>>Нашел add\n");
                add(&proc->stack);
                break;

            case cmd_array[2].id:
                printf(">>>Нашел sub\n");
                sub(&proc->stack);
                break;

            case cmd_array[3].id:
                printf(">>>Нашел mul\n");
                mul(&proc->stack);
                break;

            case cmd_array[4].id:
                printf(">>>Нашел div\n");   
                div(&proc->stack);
                break;

            case cmd_array[8].id:
                printf(">>>Нашел ou\n");
                out(&proc->stack);
                break;

            case cmd_array[7].id:
                printf(">>>Нашел in\n");
                in(&proc->stack);
                break;
            
            default:
                printf("Команда без аргумента не найдена\n");
        }
    }
        return proc_ok;
}

void check_arg(Processor* proc, chunk_t* text, byte_t command_id, size_t* pass)
{
    printf("Найдена функция с аргументом \n");
    *pass += sizeof(chunk_t);
    if (command_id == 5)
    {
        if (*((byte_t*)text + 1) == CONST_MASK)
        {
            int const_tmp = (byte_t)*text;
            push(proc, 0, const_tmp);
        }
        if (*((byte_t*)text + 1) == REG_MASK)
        {
            int reg_tmp = (byte_t)*text;
            push(proc, 1, reg_tmp);
        }
    }
    if (command_id == 6)
    {
        if (*((byte_t*)text + 1) == CONST_MASK)
        {
            int const_tmp = (byte_t)*text;
            pop(proc, 0, const_tmp);
        }
        if (*((byte_t*)text + 1) == REG_MASK)
        {
            int reg_tmp = (byte_t)*text;
            pop(proc, 1, reg_tmp);
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