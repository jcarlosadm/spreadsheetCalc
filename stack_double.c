/*
 * \file stack_double.c
 * Implementação do arquivo stack_double.h
 */

#include "stack_double.h"

/************************************************************
 * Estruturas
 ************************************************************/

/**
 * Estrutura de um elemento da pilha
 */
typedef struct element Element;
struct element{
    double item;
    Element* next;
};

/**
 * Estrutura da pilha de doubles
 */
struct stackDouble{
    Element* top;
};

/************************************************************
 * Funções privadas
 ************************************************************/

Element* freeElements(Element* element){
    if(!element) return NULL;

    Element* prev = element;
    element = element->next;
    free(prev);
    return freeElements(element);
}

/************************************************************
 * Funções públicas
 ************************************************************/

/**
 * Aloca pilha
 * \return Retorna ponteiro para a memória alocada, ou NULL em caso de falha
 */
StackDouble* StackDouble_create(){
    StackDouble* stackDouble = malloc(sizeof(StackDouble));
    if(!stackDouble) return NULL;

    stackDouble->top = NULL;

    return stackDouble;
}

/**
 * Desaloca pilha
 * \return Retorna NULL
 * \param stackDouble Ponteiro para StackDouble
 */
StackDouble* StackDouble_free(StackDouble* stackDouble){
    if(!stackDouble) return stackDouble;

    stackDouble->top = freeElements(stackDouble->top);
    free(stackDouble);
    return NULL;
}

/**
 * Adiciona valor double na pilha
 * \return Retorna 1 em caso de sucesso ou 0 em caso de falha (em
 * geral, problemas de alocação)
 * \param stackDouble Ponteiro para StackDouble
 * \param value Valor a ser adicionado na pilha
 */
int StackDouble_push(StackDouble** stackDouble, double value){
    if(!(*stackDouble)){
        printf("stack is NULL\n");
        return 0;
    }

    Element* element = malloc(sizeof(Element));
    if(!element) return 0;

    element->item = value;
    element->next = (*stackDouble)->top;
    (*stackDouble)->top = element;
    return 1;
}

/**
 * Retira elemento do topo e retorna valor desse elemento
 * \return Valor do elemento removido do topo da pilha
 * \param stackDouble Ponteiro duplo para StackDouble
 */
double StackDouble_pop(StackDouble** stackDouble){
    if(!(*stackDouble)){
        printf("stack is NULL\n");
        return 0;
    }

    double value = (*stackDouble)->top->item;
    Element* element = (*stackDouble)->top;
    (*stackDouble)->top = (*stackDouble)->top->next;
    free(element);
    return value;
}

/**
 * Pega valor do elemento do topo da pilha
 * \return Retorna valor do elemento do topo
 * \param stackDouble Ponteiro duplo para StackDouble
 */
double StackDouble_peek(StackDouble** stackDouble){
    if(!(*stackDouble)){
        printf("stack is NULL\n");
        return 0;
    }

    return (*stackDouble)->top->item;
}

/**
 * Verifica se a pilha se encontra vazia
 * \return 1 se estiver vazia, ou 0 caso não esteja vazia
 * \param stackDouble Ponteiro duplo para StackDouble
 */
int StackDouble_isEmpty(StackDouble** stackDouble){
    return ((*stackDouble)->top == NULL);
}
