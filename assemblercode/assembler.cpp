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
    
    return asm_ok;
}

void asm_dtor(Assembler* assembler)
{
    ASSERT(assembler != nullptr);

    free(assembler->bin_buf.translated_code);

    FileDataDtor(&assembler->filedata);
}

asm_errors translate_to_code(Assembler* assembler, char* line_of_code)
{
    ASSERT(line_of_code != nullptr);
    ASSERT(assembler    != nullptr);
    
    char command_name[10] = "";
    char arg_str[10]      = "";
    long arg_num = 0;
    imm_t const_num = 0;
    reg_t reg_num   = 0;
    cmd_t cmd_form  = 0;

    if (sscanf(line_of_code, "%s", command_name) == 1)
    {
        #define DEF_CMD(cmd_name, id, is_jump, ...)     \
            if (strcmp(#cmd_name, command_name) == 0)   \
            {                                           \
                cmd_form = id;                          \
                printf(">> id: %d \n",id);                        \
            }                                           \
            else                                       
        // end def
            
        #include "../shared/cmd_def.h"
        /* else */
            return asm_syntax; 

        #undef DEF_CMD
    }

    if (sscanf(line_of_code, "%s %ld", command_name, &arg_num) == 2)
    {
        MAKE_MASK(cmd_form, CONST_MASK);
        COPY_TO_BINBUF_COMMAND(cmd_form);

        const_num = (imm_t)arg_num; 
        COPY_TO_BINBUF_CONST(const_num); 
    } 
    
    else if (sscanf(line_of_code, "%s %s", command_name, arg_str) == 2)
    {
        MAKE_MASK(cmd_form, REG_MASK);
        COPY_TO_BINBUF_COMMAND(cmd_form);

        for (size_t pass = 0; pass < NUMBER_OF_REGISTRS; pass++)
        {
            if (strcmp(reg_array[pass].name, arg_str) == 0)
            {
                reg_num = (reg_t)reg_array[pass].id;
                COPY_TO_BINBUF_REG(reg_num);

                break;
            }
        }
    } 

    else if (sscanf(line_of_code, "%s", command_name) == 1)
    {
        COPY_TO_BINBUF_COMMAND(cmd_form);
    }

    return asm_ok;
}

asm_errors assemble(Assembler* assembler, const int argc, const char** argv)
{
    ASSERT(assembler != nullptr);
    if(argc != 3)
        return asm_too_few_files;

    char** pass_text = assembler->filedata.text;

    asm_errors error = asm_ok;

    while (pass_text < assembler->filedata.text + assembler->filedata.textSize)
    {
        error = translate_to_code(assembler, *pass_text);
        if (error != asm_ok)
            return error;

        pass_text++;
    }

    return (translate_code_to_file(assembler, argv));
}

asm_errors translate_code_to_file(Assembler* assembler, const char** argv)
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