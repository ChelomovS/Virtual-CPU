#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../shared/file_lib.h"
#include "../shared/file_lib_bin.h"
#include "../shared/commands.h"

static const size_t STANDART_ALLOC_SIZE = 256;
static const size_t STANDART_REALLOC_COEF = 2;

typedef size_t jump_adress;

struct label 
{
    char* lbl_start;
    size_t lbl_size;
    size_t lbl_pozition;
};

struct Assembler
{
    FileData filedata;
    FILE* trans_file;
    BinBuf bin_buf;
    size_t pass;  
    label* name_table;
    size_t number_labels;
};

enum asm_errors
{
    asm_ok                         = 0,
    asm_syntax                     = 1,
    asm_bad_alloc                  = 2,
    asm_bad_open_file_for_reading  = 3,
    asm_bad_open_file_for_writing  = 4,
    asm_too_few_files              = 5,
};

asm_errors asm_ctor(Assembler* assembler, const int argc, const char** argv);
void asm_dtor(Assembler* assembler);
asm_errors assemble(Assembler* assembler, const char** argv);
asm_errors translate_to_code(Assembler* assembler,char* line_of_text);
asm_errors resize(Assembler* assembler);
asm_errors copy_code_to_file(Assembler* assembler, const char** argv);
void check_errors(asm_errors error);

#endif // ASSEMBLER_H