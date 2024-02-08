#include "processor.h"


int main()
{   
    Processor proc = {};

    proc_constructor(&proc);

    FILE* translated_file = fopen("translator_file.txt", "r");

    execute(&proc, translated_file);

    proc_destructor(&proc);

    return 0;
}