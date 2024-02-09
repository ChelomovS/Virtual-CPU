#include "processor.h"


int main()
{   
    Processor proc = {};

    FILE* translated_file = fopen("translator_file.txt", "r");

    proc_constructor(&proc, translated_file);

    execute(&proc);

    proc_destructor(&proc);

    return 0;
}