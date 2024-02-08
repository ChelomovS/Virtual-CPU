#include <stdio.h>

#include "assembler.h"
#include "../shared/fileLib.h"

int main(const int argc, const char** argv)
{
    Assembler assembler = {};
    
    asm_ctor(&assembler, argc, argv);

    assemble(&assembler);

    translate_code_to_file(&assembler);

    asm_dtor(&assembler);

    return 0;

}