#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdlib.h>
#include <string.h>

#include "fileLib.h"
#include "commands.h"

struct Assembler
{
    FileData filedata;
    FILE* trans_file;
};

enum asm_errors
{
    asm_ok                         = 0,
    asm_syntax                     = 1,
    asm_bad_alloc                  = 2,
    asm_not_enought_files          = 3,
    asm_bad_open_file_for_reading  = 4,
    asm_bad_open_file_for_writting = 5,
};


asm_errors asm_ctor(Assembler* assembler, const int argc, const char** argv);
asm_errors asm_dtor(Assembler* assembler, FILE* trans_file);
asm_errors assemble(Assembler* assembler);
asm_errors translate_to_code(Assembler* assembler,char* line_of_text);


#endif // ASSEMBLER_H