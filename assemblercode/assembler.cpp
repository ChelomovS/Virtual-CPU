#include "assembler.h"

#define COPY_TO_BINBUF(A)\
        resize(assembler);\
        memcpy(assembler->bin_buf.translated_code + assembler->bin_buf.size, &A, sizeof(int));\
        assembler->bin_buf.size += sizeof(chunk_t);\

#define MAKE_MASK(A, MOVE, MASK)\
        *((byte_t*)&A + MOVE) = MASK;\


asm_errors resize(Assembler* assembler)
{
    ASSERT(assembler != nullptr);

    if (assembler->bin_buf.size >= assembler->bin_buf.capacity - sizeof(uint8_t))
    {
        byte_t* tmp = assembler->bin_buf.translated_code;

        assembler->bin_buf.translated_code = (byte_t*)
                                    realloc(assembler->bin_buf.translated_code,
                                            STANDART_REALLOC_COEF);

        assembler->bin_buf.capacity *= STANDART_REALLOC_COEF;

        if (assembler->bin_buf.translated_code == nullptr)
        {
            assembler->bin_buf.translated_code = tmp;
            return asm_bad_alloc;
        }
        else 
            return asm_ok;
    }
    else 
        return asm_ok;
}

asm_errors asm_ctor(Assembler* assembler, const int argc, const char** argv)
{
    ASSERT(assembler != nullptr);
    ASSERT(argv      != nullptr);

    FILE* source_file = fopen(argv[1], "r");

    ParseFile(source_file, &assembler->filedata);

    fclose(source_file);

    assembler->bin_buf.translated_code = (byte_t*)calloc(STANDART_ALLOC_SIZE,
                                                          sizeof(byte_t));
    if (assembler->bin_buf.translated_code == nullptr)
        return asm_bad_alloc;
    else
        return asm_ok;
}

asm_errors asm_dtor(Assembler* assembler)
{
    ASSERT(assembler != nullptr);

    free(assembler->bin_buf.translated_code);

    FileDataDtor(&assembler->filedata);

    return asm_ok;
}

asm_errors translate_to_code(Assembler* assembler, char* line_of_code)
{
    ASSERT(line_of_code != nullptr);
    ASSERT(assembler    != nullptr);
    
    char command_name[10] = "";
    char arg_str[10] = "";
    byte_t arg_num = 0;

    if (sscanf(line_of_code, "%s %d", command_name, &arg_num) == 2)
    {
        for (size_t pass = 0; pass < NUMBER_OF_COMMANDS; pass++)
            if (strcmp(cmd_array[pass].name, command_name) == 0)
            {
                chunk_t cmd_form = cmd_array[pass].id; 
                MAKE_MASK(cmd_form, 1, CONST_MASK);
                COPY_TO_BINBUF(cmd_form);

                chunk_t arg_form = arg_num;
                COPY_TO_BINBUF(arg_form);

                break;
            }
    }
    else
    if (sscanf(line_of_code, "%s %s", command_name, arg_str) == 2)
        {
            for (size_t pass = 0; pass < NUMBER_OF_COMMANDS; pass++)
                if (strcmp(cmd_array[pass].name, command_name) == 0)
                {
                    chunk_t cmd_form = cmd_array[pass].id;
                    MAKE_MASK(cmd_form, 1, REG_MASK)
                    COPY_TO_BINBUF(cmd_form);

                    break;
                }
        for (size_t pass = 0; pass < NUMBER_OF_REGISTRS; pass++)
            if (strcmp(reg_array[pass].name, arg_str) == 0)
            {
                chunk_t reg_form = reg_array[pass].id;
                
                COPY_TO_BINBUF(reg_form);

                break;
            }
    
    } 
    else 
    if (sscanf(line_of_code, "%s", command_name) == 1)
    {
        for (size_t pass = 0; pass < NUMBER_OF_COMMANDS; pass++)
        {
            if (strcmp(cmd_array[pass].name, command_name) == 0)
            {
                chunk_t cmd_form = cmd_array[pass].id;

                COPY_TO_BINBUF(cmd_form);

                break;
            }
        }
    }
    else 
        return asm_ok;
}

asm_errors assemble(Assembler* assembler)
{
    ASSERT(assembler != nullptr);

    char** pass_text = assembler->filedata.text;

    while (pass_text < assembler->filedata.text + assembler->filedata.textSize)
    {
        translate_to_code(assembler, *pass_text);
        pass_text++;
    }

    return asm_ok;
}

void translate_code_to_file(Assembler* assembler)
{
    FILE* translated_file = fopen("translator_file.txt", "w");

    fwrite(assembler->bin_buf.translated_code, sizeof(uint8_t), assembler->bin_buf.size, translated_file);

    fclose(translated_file);
}