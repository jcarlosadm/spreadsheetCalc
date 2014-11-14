/*
 * \file stack_binExpTree.c
 * Implementação de stack_binExpTree.h
 */

#include "stack_binExpTree.h"

/***************************************************************************
 * Estruturas
 ***************************************************************************/

/**
 * Estrutura de cada elemento da pilha
 */
typedef struct element Element;
struct element{
    BinaryExpTree* tree;
    Element* next;
};

/**
 * Estrutura da pilha de árvores de expressão binária
 */
struct stackBinExpTree{
    Element* top;
};

/***************************************************************************
 * Funções privadas
 ***************************************************************************/

/**
 * Libera elementos da pilha
 */
Element* STACKBINEXPTREE_freeElements(Element* element){
    if(!element) return NULL;

    Element* previous = element;
    element = element->next;
    previous->tree = BINARYEXPTREE_free(previous->tree);
    free(previous);

    return STACKBINEXPTREE_freeElements(element);
}

/***************************************************************************
 * Funções públicas
 ***************************************************************************/

/**
 * Cria uma pilha de árvores de expressão binária
 * \return Ponteiro para StackBinTree, ou NULL em caso de falha de alocação
 */
StackBinExpTree* STACKBINEXPTREE_create(){
    StackBinExpTree* stack = malloc(sizeof(StackBinExpTree));
    if(!stack) return NULL;

    stack->top = NULL;
    return stack;
}

/**
 * Libera a memória alocada na pilha
 * \return NULL
 * \param stack Ponteiro para StackBinExpTree
 */
StackBinExpTree* STACKBINEXPTREE_free(StackBinExpTree* stack){
    if(!stack) return NULL;

    if(stack->top) stack->top = STACKBINEXPTREE_freeElements(stack->top);

    free(stack);
    return NULL;
}

/**
 * Adiciona uma árvore de expressão binária (que conterá o valor fornecido) na pilha
 * \return 1 em caso de sucesso, ou 0 em caso de falha
 * \param stack Ponteiro duplo para StackBinExpTree
 * \param value Valor a ser adicionado na árvore
 */
int STACKBINEXPTREE_pushValue(StackBinExpTree** stack, double value){
    BinaryExpTree* newTree = BINARYEXPTREE_create(value);
    if(!newTree) return 0;

    Element* newElement = malloc(sizeof(Element));
    if(!newElement){
        newTree = BINARYEXPTREE_free(newTree);
        return 0;
    }

    newElement->next = (*stack)->top;
    newElement->tree = newTree;

    (*stack)->top = newElement;

    return 1;
}

