#include "stack.h"
#include "stackConfig.h"

stack_error stack_constuctor(Stack* stack, size_t init_capacity)
{
    ASSERT(stack != nullptr);
    ASSERT(init_capacity > 0);
    
    elem_t* hold_ptr = stack->data;

    stack->data = (elem_t*)calloc(init_capacity, sizeof(elem_t));

    if (stack->data == nullptr)
    {
        stack->data = hold_ptr;

        return stack_bad_alloc;
    }

    stack->size = 0;

    stack->capacity = init_capacity;

    return stack_ok;
}

stack_error stack_push(Stack* stack, elem_t element)
{
    ASSERT(stack != nullptr);

    if (stack->size >= stack->capacity)
    {
        elem_t* hold_ptr = stack->data; 

        stack->data = (elem_t*)realloc(stack->data, 
                                       stack->size * GROW_FACTOR * sizeof(elem_t));   
        if (stack->data == nullptr)
        {
            stack->data = hold_ptr;

            return stack_bad_alloc;
        }

        stack->capacity *= GROW_FACTOR;
    }
    stack->size++;
    stack->data[stack->size] = element;

    return stack_ok;
}

stack_error stack_pop(Stack* stack, elem_t* pop_element)
{
    ASSERT(stack != nullptr);
    ASSERT(pop_element != nullptr);

    if (stack->size <= 0)
        return stack_empty_pop;

    elem_t element = stack->data[stack->size];
    *pop_element = element;

    stack->size--;

    return stack_ok;
}

void stack_destructor(Stack* stack)
{
    ASSERT(stack != nullptr);

    free(stack->data);
}

void stack_dump(Stack* stack)
{
    ASSERT(stack != nullptr);

    printf(YELLOW ">>> Capacity: %lu \n" WHITE, stack->capacity);
    printf(YELLOW ">>> Size: %lu \n" WHITE, stack->size);

    for (size_t i = 0; i < stack->size; i++)
    {
        printf(YELLOW ">>> Element %d \n", i, ELEM_SPEC "\n" BLACK, stack->data[i]); 
    }
}
