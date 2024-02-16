#include <stdio.h>

#include "assembler.h"
#include "../shared/fileLib.h"

int main(const int argc, const char** argv)
{
    Assembler assembler = {};

    asm_errors error = asm_ok;

    error = asm_ctor(&assembler, argc, argv);
    if(error == asm_ok)
    {
        error = assemble(&assembler, argc, argv);
    }

    check_errors(error);

    asm_dtor(&assembler);

    return 0;
}