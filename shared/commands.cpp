#include "commands.h"

int halt()
{
    return 0;
}

int add(Stack* stack)
{
    ASSERT(stack != nullptr);

    int first_tmp = 0;
    int second_tmp = 0;

    stack_pop(stack, &first_tmp);
    stack_pop(stack, &second_tmp);
    stack_push(stack, first_tmp + second_tmp);

    return 0;
}

int sub(Stack* stack)
{
    ASSERT(stack != nullptr);

    int first_tmp = 0;
    int second_tmp = 0;

    stack_pop(stack, &first_tmp);
    stack_pop(stack, &second_tmp);
    stack_push(stack, first_tmp - second_tmp);
    
    return 0;
}

int mul(Stack* stack)
{
    ASSERT(stack != nullptr);

    int first_tmp = 0;
    int second_tmp = 0;

    stack_pop(stack, &first_tmp);
    stack_pop(stack, &second_tmp);
    stack_push(stack, first_tmp * second_tmp);
    
    return 0;
}

int div(Stack* stack)
{
    ASSERT(stack != nullptr);

    int first_tmp = 0;
    int second_tmp = 0;

    stack_pop(stack, &first_tmp);
    stack_pop(stack, &second_tmp);
    stack_push(stack, first_tmp / second_tmp);
    
    return 0;
}

int push(Processor* proc, int primary_arg, int optional_arg)
{
    if (primary_arg == 01)             
    {
        stack_push(&proc->stack, optional_arg);
    }

    if (primary_arg == 02)
    {
        switch (optional_arg)
        {
            case reg_array[0].id:
            {
                stack_push(&proc->stack, proc->reg[0]);
                return 0;
            }

            case reg_array[1].id:
            {
                stack_push(&proc->stack, proc->reg[1]);
                return 0;
            }

            case reg_array[2].id:
            {
                stack_push(&proc->stack, proc->reg[2]);
                return 0;
            }

            case reg_array[3].id:
            {
                stack_push(&proc->stack, proc->reg[3]);
                return 0;
            }
        }
    }

    return 0;
}

int pop(Processor* proc, int primary_arg, int optional_arg)
{
    if (primary_arg == 0)                // 0 соответствует const
    {
        stack_pop(&proc->stack, &optional_arg);
    }

    if (primary_arg == 1)
    {
        switch (optional_arg)
        {
            case reg_array[0].id:
            {
                stack_pop(&proc->stack, &proc->reg[0]);
                return 0;
            }

            case reg_array[1].id:
            {
                stack_pop(&proc->stack, &proc->reg[1]);
                return 0;
            }

            case reg_array[2].id:
            {
                stack_pop(&proc->stack, &proc->reg[2]);
                return 0;
            }

            case reg_array[3].id:
            {
                stack_pop(&proc->stack, &proc->reg[3]);
                return 0;
            }
        }
    }

    return 0;
}

int in(Stack* stack)
{
    int tmp = 0;

    scanf("%d", &tmp);
    stack_push(stack, tmp);

    return 1;
}

int out(Stack* stack)
{
    int pop_element = 0;

    stack_pop(stack, &pop_element);

    printf("%d \n", pop_element);

    return 1;
}