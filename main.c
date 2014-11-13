/*
 * main.c
 * (temporariamente usada para testes)
 */

#include <stdio.h>
#include "stack_double.h"

int main()
{
    StackDouble* stack = STACKDOUBLE_create();

    STACKDOUBLE_push(&stack, 4.5);
    STACKDOUBLE_push(&stack, 3);
    STACKDOUBLE_push(&stack, 9);
    STACKDOUBLE_push(&stack, 1);

    printf("peek: %.2f\n", STACKDOUBLE_peek(&stack));
    printf("pop: %.2f\n", STACKDOUBLE_pop(&stack));
    printf("pop: %.2f\n", STACKDOUBLE_pop(&stack));

    stack = STACKDOUBLE_free(stack);

    return 0;
}
