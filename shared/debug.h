#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "colors.h"

#ifndef NDEBUG
#define DEBUG_PRINT(...)\
    fprintf(stderr, __VA_ARGS__);

#define ASSERT(a)                                                              \
    do                                                                         \
    {                                                                          \
        if (!(a))                                                              \
            {                                                                  \
                fprintf(stderr, RED "Ошибка! \n" RED);                         \
                fprintf(stderr, RED "В файле: %s \n" RED , __FILE__);          \
                fprintf(stderr, RED "В функции: %s \n" RED, __func__);         \
                fprintf(stderr, RED "В строке: %d \n" BLACK , __LINE__);       \
                exit(0);                                                       \
            }                                                                  \
    }                                                                          \
    while (0);
#else
#define ASSERT(a) ;
#define DEBUG_PRINT(...) ;
#endif


#define $ fprintf(stderr, RED ">>> %s (%d) %s\n" WHITE,                        \
                  __FILE__, __LINE__, __PRETTY_FUNCTION__);

#define PRINT_NUM(num) fprintf(stderr, "%lld\n", num);
#define PRINT_POINTER(ptr) fprintf(stderr, "%s %p\n", #ptr, ptr);

#endif // DEBUG_H
