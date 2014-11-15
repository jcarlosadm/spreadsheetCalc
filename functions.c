/**
 * \file functions.c
 * Implementação do arquivo functions.h
 */

#include "functions.h"

/********************************************************************************
 * Estruturas
 ********************************************************************************/

/**
 * Estrutura de uma lista de valores
 */
struct listDouble{
    double value;
    ListDouble* next;
};

/********************************************************************************
 * Funções privadas
 ********************************************************************************/

/**
 * Calcula a soma de uma lista de valores
 * \return Retorna a soma
 * \param list Lista de doubles
 */
double FUNCTIONS_sum(ListDouble** list){
    if(!list || !(*list)) return 0;

    double total=0;

    ListDouble* current = (*list);

    while(current){
        total += current->value;
        current = current->next;
    }

    return total;
}

/**
 * Calcula a média de uma lista de valores
 * \return Retorna a média
 * \param list Lista de doubles
 */
double FUNCTIONS_mean(ListDouble** list){
    if(!list || !(*list)) return 0;

    double total = FUNCTIONS_sum(&(*list));

    int amount = 0;
    ListDouble* current = (*list);
    while(current){
        amount++;
        current = current->next;
    }

    return (total/amount);
}

/**
 * Procura pelo maior valor de uma lista de valores
 * \return Retorna o maior valor
 * \param list Lista de doubles
 */
double FUNCTIONS_max(ListDouble** list){
    if(!list || !(*list)) return 0;

    double value = (*list)->value;
    ListDouble* current = (*list)->next;

    while(current){
        if(current->value > value)
            value = current->value;
        current = current->next;
    }

    return value;
}

/**
 * Procura pelo menor valor de uma lista de valores
 * \return Retorna o menor valor
 * \param list Lista de doubles
 */
double FUNCTIONS_min(ListDouble** list){
    if(!list || !(*list)) return 0;

    double value = (*list)->value;
    ListDouble* current = (*list)->next;

    while(current){
        if(current->value < value)
            value = current->value;
        current = current->next;
    }

    return value;
}

/********************************************************************************
 * Funções públicas
 ********************************************************************************/

/**
 * Lista funções disponíveis para o usuário
 */
void FUNCTIONS_listFunctions(){
    printf("sum(value1, value2, ..., valueN) - soma uma lista de valores\n");
    printf("mean(value1, value2, ..., valueN) - calcula a media de uma lista de valores\n");
    printf("max(value1, value2, ..., valueN) - retorna o maior valor de uma lista de valores\n");
    printf("min(value1, value2, ..., valueN) - retorna o menor valor de uma lista de valores\n");
}

/**
 * Cria a lista de doubles
 * \return Ponteiro para ListDouble, ou NULL em caso de falha de alocação
 */
ListDouble* FUNCTIONS_createList(){
    return NULL;
}

/**
 * Desaloca memória da lista de doubles
 * \return NULL
 * \param list Ponteiro para a lista de doubles
 */
ListDouble* FUNCTIONS_free(ListDouble* list){

    ListDouble* current = NULL;
    while(list){
        current = list;
        list = list->next;
        free(current);
    }

    return NULL;
}

/**
 * Adiciona valor na lista de doubles
 * \return Ponteiro para a lista de double
 * \param list Ponteiro para a lista de doubles
 * \param value Valor a ser adicionado na lista
 */
ListDouble* FUNCTIONS_addValue(ListDouble* list, double value){

    if(!list){
        list = malloc(sizeof(ListDouble));
        if(!list) return list;

        list->next = NULL;
        list->value = value;
        return list;
    }

    ListDouble* current = list;

    while(current->next) current = current->next;

    current->next = malloc(sizeof(ListDouble));
    if(!current->next) return list;

    current = current->next;

    current->next = NULL;
    current->value = value;

    return list;
}

/**
 * Executa uma função do módulo
 * \return O resultado da execução da função na lista fornecida
 * \param function string literal do nome da função
 * \param list Ponteiro duplo para lista de doubles
 */
double FUNCTIONS_evalFunction(const char* function, ListDouble** list){
    if(strcmp(function, "sum")==0)
        return FUNCTIONS_sum(&(*list));
    else if(strcmp(function, "mean")==0)
        return FUNCTIONS_mean(&(*list));
    else if(strcmp(function, "max")==0)
        return FUNCTIONS_max(&(*list));
    else if(strcmp(function, "min")==0)
        return FUNCTIONS_min(&(*list));

    return 0;
}
