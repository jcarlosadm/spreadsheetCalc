/**
 * \file functions.c
 * Implementação do arquivo functions.h
 */

#include "functions.h"

/**
 * Lista funções disponíveis para o usuário
 */
void listFunctions(){
    printf("sum(value1, value2, ..., valueN) - soma uma lista de valores\n");
    printf("mean(value1, value2, ..., valueN) - calcula a media de uma lista de valores\n");
    printf("max(value1, value2, ..., valueN) - retorna o maior valor de uma lista de valores\n");
    printf("min(value1, value2, ..., valueN) - retorna o menor valor de uma lista de valores\n");
}

/**
 * Calcula a soma de uma lista de valores
 * \return Retorna a soma
 * \param values Ponteiro para o começo de um array de doubles
 * \param size Tamanho do array de doubles
 */
double sum(double *values, int size){
    if(size<=0) return 0;

    double total=0;
    int count;
    for(count=0; count<size; count++)
        total+=values[count];
    return total;
}

/**
 * Calcula a média de uma lista de valores
 * \return Retorna a média
 * \param values Ponteiro para o começo de um array de doubles
 * \param size Tamanho do array de doubles
 */
double mean(double *values, int size){
    if(size<=0) return 0;

    double total = sum(values, size);
    return (total/size);
}

/**
 * Procura pelo maior valor de uma lista de valores
 * \return Retorna o maior valor
 * \param values Ponteiro para o começo de um array de doubles
 * \param size Tamanho do array de doubles
 */
double max(double *values, int size){
    if(size<=0) return 0;

    double value = values[0];
    int count;
    for(count=1; count<size; count++){
        if(values[count] > value)
            value = values[count];
    }

    return value;
}

/**
 * Procura pelo menor valor de uma lista de valores
 * \return Retorna o menor valor
 * \param values Ponteiro para o começo de um array de doubles
 * \param size Tamanho do array de doubles
 */
double min(double *values, int size){
    if(size <= 0) return 0;

    double value = values[0];
    int count;
    for(count=0; count<size; count++){
        if(values[count] < value)
            value = values[count];
    }

    return value;
}
