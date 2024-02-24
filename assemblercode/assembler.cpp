#include <string.h>
#include "assembler.h"

#define COPY_TO_BINBUF_CONST(A)\
        resize(assembler);\
        memcpy(assembler->bin_buf.translated_code\
               + assembler->bin_buf.size, &A, sizeof(imm_t));\
        assembler->bin_buf.size += sizeof(imm_t);\

#define COPY_TO_BINBUF_COMMAND(A)\
        resize(assembler);\
        memcpy(assembler->bin_buf.translated_code\
               + assembler->bin_buf.size, &A, sizeof(cmd_t));\
        assembler->bin_buf.size += sizeof(cmd_t);\

#define COPY_TO_BINBUF_REG(A)\
        resize(assembler);\
        memcpy(assembler->bin_buf.translated_code\
               + assembler->bin_buf.size, &A, sizeof(reg_t));\
        assembler->bin_buf.size += sizeof(reg_t);\

#define MAKE_MASK(A, MASK)\
        A |= MASK;\

asm_errors asm_ctor(Assembler* assembler, const int argc, const char** argv)
{
    ASSERT(assembler != nullptr);
    ASSERT(argv      != nullptr);

    if (argc != 3)
        return asm_too_few_files;

    FILE* source_file = fopen(argv[1], "r");

    if(source_file == nullptr)
        return asm_bad_open_file_for_reading;

    ParseFile(source_file, &assembler->filedata);

    fclose(source_file);

    assembler->bin_buf.translated_code = (byte_t*)calloc(STANDART_ALLOC_SIZE,
                                                         sizeof(byte_t));
    if (assembler->bin_buf.translated_code == nullptr)
        return asm_bad_alloc;

    assembler->name_table = (label*)(calloc(10, sizeof(label)));
    
    assembler->pass = 0;

    assembler->number_labels = 0;



    return asm_ok;
}

void asm_dtor(Assembler* assembler)
{
    ASSERT(assembler != nullptr);

    free(assembler->bin_buf.translated_code);
    free(assembler->name_table); //FIXME - работа с выделение памяти обработка ошибок т д

    FileDataDtor(&assembler->filedata);
}

asm_errors assemble(Assembler* assembler, const char** argv)
{
    ASSERT(assembler != nullptr);
    ASSERT(argv      != nullptr);

    char** pass_text = assembler->filedata.text;

    asm_errors error = asm_ok;

    while (pass_text < assembler->filedata.text + assembler->filedata.textSize)
    {
        error = translate_to_code(assembler, *pass_text);
        if (error != asm_ok)
            return error;

        pass_text++;
    }

    assembler->pass++;
    pass_text = assembler->filedata.text;
    assembler->bin_buf.size = 0;

    while (pass_text < assembler->filedata.text + assembler->filedata.textSize)
    {
        error = translate_to_code(assembler, *pass_text);
        if (error != asm_ok)
            return error;

        pass_text++;
    }

    return (copy_code_to_file(assembler, argv));
}

asm_errors translate_to_code(Assembler* assembler, char* line_of_code)
{
    ASSERT(line_of_code != nullptr);
    ASSERT(assembler    != nullptr);
    
    char  command_name[10] = "";
    char  arg_str[10]      = "";
    long  arg_num          = 0;
    imm_t const_num        = 0;
    reg_t reg_num          = 0;
    cmd_t cmd_form         = 0;
    bool is_jump_found     = false;

    //DEBUGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
    if (assembler->pass == 0)
    {
        printf ("Первый проход \n");
    }

    if (assembler->pass == 1)
    {
        printf ("Второй проход \n");
    }

    // поиск метки при первом проходе

        char* pos_label = strchr(line_of_code, ':');
        if (pos_label != nullptr)
        {
            if (assembler->pass == 0)
            {
                // assembler->name_table[assembler->number_labels].lbl_size =
                //                              (size_t)(pos_label - line_of_code);
                // assembler->name_table[assembler->number_labels].lbl_start =
                //                              pos_label - assembler->name_table
                //                                     [assembler->number_labels].lbl_size;
                assembler->name_table[assembler->number_labels] = {line_of_code, (size_t)(pos_label - line_of_code), assembler->bin_buf.size};
                printf("%s \n", pos_label);
                printf("%d \n", assembler->name_table[assembler->number_labels].lbl_size);
                printf("%d \n", assembler->name_table[0].lbl_size);
                printf("%s \n", assembler->name_table[assembler->number_labels].lbl_start);
                printf("lbl_start %s \n", assembler->name_table[assembler->number_labels].lbl_start);

                // assembler->name_table[assembler->number_labels].lbl_size = (size_t)(pos_label);

                assembler->number_labels++;
            }
            return asm_ok;
        } 



    // кодогенерация
    if (sscanf(line_of_code, "%s", command_name) == 1)
    {
        #define DEF_CMD(cmd_name, id, is_jump, ...)     \
            if (strcmp(#cmd_name, command_name) == 0)   \
            {                                           \
                cmd_form = id;                          \
                is_jump_found = is_jump;                \
            }                                           \
            else                                       
        // end def
            
        #include "../shared/cmd_def.h"
        /*else*/
            return asm_syntax; 

        #undef DEF_CMD
    }

    // есть числовой аргумент
    if (sscanf(line_of_code, "%s %ld", command_name, &arg_num) == 2)
    {
        printf("Найдена команда с числом %s \n", command_name);
        MAKE_MASK(cmd_form, CONST_MASK);
        COPY_TO_BINBUF_COMMAND(cmd_form);

        const_num = (imm_t)arg_num; 
        COPY_TO_BINBUF_CONST(const_num); 
    } 

    // есть строковый аргумент
    if (sscanf(line_of_code, "%s %s", command_name, arg_str) == 2)
    {
        if (is_jump_found)
        {
            printf("Найден джамп: %s аргумент: %s \n", command_name, arg_str);

            COPY_TO_BINBUF_COMMAND(cmd_form);

            if (assembler->pass == 0) // первый проход
            {
                printf("записываю в джамп 0 \n");
                const_num = (imm_t)0;
            }
            else 
            if (assembler->pass == 1) // второй проход
            {
                for (size_t label_counter = 0; label_counter < assembler->number_labels; label_counter++)
                {
                    printf("ищем метку в таблице \n");
                    printf("метка: %s \n", assembler->name_table[label_counter].lbl_start);
                    printf("аргумент: %s \n", arg_str);
                    printf("размер метки: %d \n", assembler->name_table[0].lbl_size);
                    if (strncmp(assembler->name_table[label_counter].lbl_start, arg_str,
                                         assembler->name_table[label_counter].lbl_size) == 0)
                    {
                        printf("записал метку %s \n", assembler->name_table[label_counter].lbl_start);
                        const_num = (imm_t)(assembler->name_table[label_counter].lbl_pozition);
                        break;
                    }
                }
            }
            COPY_TO_BINBUF_CONST(const_num);
        }

        else // если регистр 
        for (size_t pass = 0; pass < NUMBER_OF_REGISTRS; pass++)
        {
            if (strcmp(reg_array[pass].name, arg_str) == 0)
            {
                printf("записываю регистр %s \n", arg_str);
                MAKE_MASK(cmd_form, REG_MASK);
                COPY_TO_BINBUF_COMMAND(cmd_form);
                reg_num = (reg_t)reg_array[pass].id;
                COPY_TO_BINBUF_REG(reg_num);

                break;
            }
        }
    } 

    // команда без аргумента 
    else
    if (sscanf(line_of_code, "%s", command_name) == 1)
    {
        printf("Найдена команда без аргумента %s\n", command_name);
        COPY_TO_BINBUF_COMMAND(cmd_form);
    }

    return asm_ok;
}

asm_errors copy_code_to_file(Assembler* assembler, const char** argv)
{
    ASSERT(assembler != nullptr);

    if(argv[2] == nullptr)
        return asm_bad_open_file_for_writing;

    FILE* translated_file = fopen(argv[2], "w");

    fwrite(assembler->bin_buf.translated_code, sizeof(uint8_t), 
           assembler->bin_buf.size, translated_file);

    fclose(translated_file);

    return asm_ok;
}

void check_errors(asm_errors error)
{
    switch(error)
    {
        case asm_ok:
            break;

        case asm_bad_alloc:
            fprintf(stderr, RED "Realloc error\n" BLACK);
            break;

        case asm_bad_open_file_for_reading:
            fprintf(stderr, RED "Bad open file for reading\n" BLACK);
            break;

        case asm_bad_open_file_for_writing:
            fprintf(stderr, RED "Bad open file for writing\n" BLACK);
            break;

        case asm_syntax:
            fprintf(stderr, RED "Syntax error\n" BLACK);
            break;

        case asm_too_few_files:
            fprintf(stderr, RED "Too few files\n" BLACK);
            break;

        default:
            ASSERT(0 && "UNKNOWN ERROR");
            break;

    }
}

asm_errors resize(Assembler* assembler)
{
    ASSERT(assembler != nullptr);

    if (assembler->bin_buf.size < assembler->bin_buf.capacity - sizeof(imm_t))
        return asm_ok;

    byte_t* tmp = assembler->bin_buf.translated_code;

    assembler->bin_buf.translated_code = 
        (byte_t*)realloc(assembler->bin_buf.translated_code,
                            STANDART_REALLOC_COEF);

    assembler->bin_buf.capacity *= STANDART_REALLOC_COEF;

    if (assembler->bin_buf.translated_code == nullptr)
    {
        assembler->bin_buf.translated_code = tmp;
        return asm_bad_alloc;
    }

    return asm_ok;
}
