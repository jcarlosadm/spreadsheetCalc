/*
 * main.c
 * (temporariamente usada para testes)
 */

#include <stdio.h>
#include "stack_double.h"

int main()
{
    StackDouble* stack = StackDouble_create();

    StackDouble_push(&stack, 4.5);
    StackDouble_push(&stack, 3);
    StackDouble_push(&stack, 9);
    StackDouble_push(&stack, 1);

    printf("peek: %.2f\n", StackDouble_peek(&stack));
    printf("pop: %.2f\n", StackDouble_pop(&stack));
    printf("pop: %.2f\n", StackDouble_pop(&stack));

    stack = StackDouble_free(stack);

    return 0;
}
