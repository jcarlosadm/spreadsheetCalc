/*
 * main.c
 *
 */

#include <stdio.h>
#include "functions.h"

int main()
{
    //printf("hello world!\n");
    listFunctions();

    double myArray[] = {2.3, 5.3, 3, 7, 1, 9.4};

    printf("min: %.2f\n", min(myArray, sizeof(myArray)/sizeof(double)));

    return 0;
}
