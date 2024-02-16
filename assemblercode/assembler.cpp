#include "assembler.h"

#define COPY_TO_BINBUF(A)\
        resize(assembler);\
        memcpy(assembler->bin_buf.translated_code\
               + assembler->bin_buf.size, &A, sizeof(int));\
        assembler->bin_buf.size += sizeof(chunk_t);\

#define MAKE_MASK(A, MOVE, MASK)\
        *((byte_t*)&A + MOVE) = MASK;\

asm_errors resize(Assembler* assembler)
{
    ASSERT(assembler != nullptr);

    if (assembler->bin_buf.size < assembler->bin_buf.capacity - sizeof(chunk_t))
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
    char arg_str[10] = "";
    chunk_t arg_num = 0;

    if (sscanf(line_of_code, "%s %d", command_name, &arg_num) == 2)
    {
        bool got_valid_cmd = false;
        for (size_t pass = 0; pass < NUMBER_OF_COMMANDS; pass++) 
        {
            if (strcmp(cmd_array[pass].name, command_name) == 0)
            {
                chunk_t cmd_form = cmd_array[pass].id; 
                MAKE_MASK(cmd_form, 1, CONST_MASK);
                COPY_TO_BINBUF(cmd_form);

                chunk_t arg_form = arg_num;
                COPY_TO_BINBUF(arg_form);

                got_valid_cmd = true;

                break;
            }

        }
        if (!got_valid_cmd) 
        { 
            return asm_syntax; 
        }
    }
    else
    if (sscanf(line_of_code, "%s %s", command_name, arg_str) == 2)
    {
        bool got_valid_cmd = false;

        for (size_t pass = 0; pass < NUMBER_OF_COMMANDS; pass++) 
        {
            if (strcmp(cmd_array[pass].name, command_name) == 0)
            {
                chunk_t cmd_form = cmd_array[pass].id;
                MAKE_MASK(cmd_form, 1, REG_MASK)
                COPY_TO_BINBUF(cmd_form);

                got_valid_cmd = true;
            
                break;
            }
        }
        for (size_t pass = 0; pass < NUMBER_OF_REGISTRS; pass++)
        {
            if (strcmp(reg_array[pass].name, arg_str) == 0)
            {
                chunk_t reg_form = reg_array[pass].id;
                
                COPY_TO_BINBUF(reg_form);

                got_valid_cmd = true;

                break;
            }
        }
        if (!got_valid_cmd) 
        { 
            return asm_syntax; 
        }
    } 
    else 
    if (sscanf(line_of_code, "%s", command_name) == 1)
    {
        bool got_valid_cmd = false;

        for (size_t pass = 0; pass < NUMBER_OF_COMMANDS; pass++)
        {
            if (strcmp(cmd_array[pass].name, command_name) == 0)
            {
                chunk_t cmd_form = cmd_array[pass].id;

                COPY_TO_BINBUF(cmd_form);

                got_valid_cmd = true;

                break;
            }
        }

        if (!got_valid_cmd) 
        { 
            return asm_syntax; 
        }
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