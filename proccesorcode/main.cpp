#include "processor.h"


int main(const int argc, const char** argv)
{   
    Processor proc = {};
    proc_errors error = proc_ok;

    error = proc_constructor(&proc, argc, argv);
    if (error == proc_ok)
    {
        error = execute(&proc);
    }

    check_errors(error);
    
    proc_destructor(&proc);

    return 0;
}