#include "commands.h"

int halt()
{
    return 0;
}

int add(Stack* stack)
{
    ASSERT(stack != nullptr);

    elem_t first_tmp = 0;
    elem_t second_tmp = 0;

    stack_pop(stack, &first_tmp); //FIXME - POP(&first_tmp)
    stack_pop(stack, &second_tmp);
    stack_push(stack, first_tmp + second_tmp);

    return 0;
}

int sub(Stack* stack)
{
    ASSERT(stack != nullptr);

    elem_t first_tmp = 0;
    elem_t second_tmp = 0;

    stack_pop(stack, &first_tmp);
    stack_pop(stack, &second_tmp);
    stack_push(stack, first_tmp - second_tmp);
    
    return 0;
}

int mul(Stack* stack)
{
    ASSERT(stack != nullptr);

    elem_t first_tmp = 0;
    elem_t second_tmp = 0;

    stack_pop(stack, &first_tmp);
    stack_pop(stack, &second_tmp);
    stack_push(stack, first_tmp * second_tmp);
    
    return 0;
}

int div(Stack* stack)
{
    ASSERT(stack != nullptr);

    elem_t first_tmp = 0;
    elem_t second_tmp = 0;

    stack_pop(stack, &first_tmp);
    stack_pop(stack, &second_tmp);
    stack_push(stack, first_tmp / second_tmp);
    
    return 0;
}

int push(Processor* proc, int primary_arg, long optional_arg)
{
    if (primary_arg == 1)             
    {
        stack_push(&proc->stack, optional_arg);
    }

    if (primary_arg == 2)
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
    if (primary_arg == 1)
    {
        stack_pop(&proc->stack, &proc->reg[optional_arg - 1]); //FIXME - 

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
    long tmp = 0;

    scanf("%ld", &tmp);
    stack_push(stack, tmp);

    return 0;
}

int out(Stack* stack)
{
    long pop_element = 0;

    stack_pop(stack, &pop_element);

    printf("%ld \n", pop_element);

    return 0;
}

int jmp(Processor* proc, long optional_arg)
{
    proc->ip = (size_t)optional_arg;

    return 0;
}

int je(Processor* proc, long optional_arg)
{
    elem_t first_tmp = 0;
    elem_t second_tmp = 0;

    stack_pop(&proc->stack, &first_tmp);
    stack_pop(&proc->stack, &second_tmp);

    if (first_tmp == second_tmp)
        proc->ip = (size_t)optional_arg;
    else
        proc->ip += sizeof(imm_t);         

    stack_push(&proc->stack, second_tmp);
    stack_push(&proc->stack, first_tmp);

    return 0;
}

int jne(Processor* proc, long optional_arg)
{
    elem_t first_tmp = 0;
    elem_t second_tmp = 0;

    stack_pop(&proc->stack, &first_tmp);
    stack_pop(&proc->stack, &second_tmp);

    if (first_tmp != second_tmp)
        proc->ip = (size_t)optional_arg;
    else
        proc->ip += sizeof(imm_t);

    stack_push(&proc->stack, second_tmp);
    stack_push(&proc->stack, first_tmp);

    return 0;
}

int ja(Processor* proc, long optional_arg)
{
    elem_t first_tmp = 0;
    elem_t second_tmp = 0;

    stack_pop(&proc->stack, &first_tmp);
    stack_pop(&proc->stack, &second_tmp);

    if (first_tmp < second_tmp)
        proc->ip = (size_t)optional_arg;
    else
        proc->ip += sizeof(imm_t);

    stack_push(&proc->stack, second_tmp);
    stack_push(&proc->stack, first_tmp);

    return 0;
}

int jea(Processor* proc, long optional_arg)
{
    elem_t first_tmp = 0;
    elem_t second_tmp = 0;

    stack_pop(&proc->stack, &first_tmp);
    stack_pop(&proc->stack, &second_tmp);

    if (first_tmp <= second_tmp)
        proc->ip = (size_t)optional_arg;
    else
        proc->ip += sizeof(imm_t);

    stack_push(&proc->stack, second_tmp);
    stack_push(&proc->stack, first_tmp);

    return 0;
}

int jb(Processor* proc, long optional_arg)
{
    elem_t first_tmp = 0;
    elem_t second_tmp = 0;

    stack_pop(&proc->stack, &first_tmp);
    stack_pop(&proc->stack, &second_tmp);

    if (first_tmp > second_tmp)
        proc->ip = (size_t)optional_arg;
    else
        proc->ip += sizeof(imm_t);

    stack_push(&proc->stack, second_tmp);
    stack_push(&proc->stack, first_tmp);

    return 0;
}

int jeb(Processor* proc, long optional_arg)
{
    elem_t first_tmp = 0;
    elem_t second_tmp = 0;

    stack_pop(&proc->stack, &first_tmp);
    stack_pop(&proc->stack, &second_tmp);

    if (first_tmp >= second_tmp)
        proc->ip = (size_t)optional_arg;
    else
        proc->ip += sizeof(imm_t);

    stack_push(&proc->stack, second_tmp);
    stack_push(&proc->stack, first_tmp);

    return 0;
}
