#include "assembler.h"

asm_errors asm_ctor(Assembler* assembler, const int argc, const char** argv)
{
    ASSERT(assembler != nullptr);
    ASSERT(argv != nullptr);

    FILE* source_file = fopen(argv[1], "r");

    ParseFile(source_file, &assembler->filedata);

    fclose(source_file);

    assembler->trans_file = fopen(argv[2], "w");

    return asm_ok;
}

asm_errors asm_dtor(Assembler* assembler, FILE* trans_file)
{
    ASSERT(assembler != nullptr);

    fclose(trans_file);

    Freedom(&assembler->filedata);

    return asm_ok;
}

asm_errors translate_to_code(Assembler* assembler, char* line_of_code)
{
    ASSERT(line_of_code != nullptr);
    ASSERT(assembler != nullptr);
    
    char command_name[100] = "";
    char arg_str[100] = "";
    double arg_num = 0;

    if (sscanf(line_of_code, "%s %lf", command_name, &arg_num) == 2)
    {
        $
        for (size_t pass = 0; pass < NUMBER_OF_COMMANDS; pass++)
            if (strcmp(cmd_array[pass].name, command_name) == 0)
            {
                fprintf(assembler->trans_file, "%d ", cmd_array[pass].id);
                fprintf(assembler->trans_file, "%lf \n", arg_num);
            }
    }
    else
    if (sscanf(line_of_code, "%s %s", command_name, arg_str) == 2) 
    {
        for (size_t pass = 0; pass < NUMBER_OF_COMMANDS; pass++)
            if (strcmp(cmd_array[pass].name, command_name) == 0)
            {
                fprintf(assembler->trans_file, "%d ", cmd_array[pass].id);
            }
        for (size_t pass = 0; pass < NUMBER_OF_REGISTRS; pass++)
            if (strcmp(reg_array[pass].name, arg_str))
            {
                fprintf(assembler->trans_file, "%d \n", reg_array[pass].id);
            }
    } 
    else 
    if (sscanf(line_of_code, "%s", command_name) == 1)
    {
        $
        for (size_t pass = 0; pass < NUMBER_OF_COMMANDS; pass++)
        {
            if (strcmp(cmd_array[pass].name, command_name) == 0)
            {
                fprintf(assembler->trans_file, "%d \n", cmd_array[pass].id);
            }
        }
    }
    else 
    $
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
