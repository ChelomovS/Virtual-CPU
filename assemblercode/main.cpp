#include <stdio.h>

#include "assembler.h"
#include "fileLib.h"

int main(const int argc, const char** argv)
{
    Assembler assembler = {};
    
    asm_ctor(&assembler, argc, argv);

    assemble(&assembler);

    asm_dtor(&assembler, assembler.trans_file);

    return 0;

}