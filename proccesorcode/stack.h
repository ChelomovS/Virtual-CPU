#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "debug.h"
#include "stackConfig.h"
#include "fileLib.h"

enum stack_error
{
    stack_ok = 0,
    stack_bad_alloc = 1,
    stack_empty_pop = 2,
};

struct Stack
{
    size_t size;
    size_t capacity;
    elem_t* data;
};

stack_error stack_constuctor(Stack* stack, size_t initcapasity);
stack_error stack_push(Stack* stack, elem_t element);
stack_error stack_pop(Stack* stack, elem_t* pop_element);
void stack_destructor(Stack* stack);
void stack_dump(Stack* stack);

#endif // STACK_H
