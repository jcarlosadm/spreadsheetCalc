/*
 * \file undo_redo_cells.c
 * Implementação do arquivo undo_redo_cells.h
 */

#include "undo_redo_cells.h"

/*********************************************************************
 * Estruturas
 *********************************************************************/

/**
 * Estrutura de cada elemento da fila
 */
typedef struct element Element;
struct element{
    int cellValue;
    char oldExpression[60];
    char newExpression[60];
    Element* next;
    Element* previous;
};

/**
 * Estrutura da fila de desfazer/refazer das células
 */
struct undoRedoCells{
    Element* first;
    Element* last;
    Element* sentinel;
};

/****************************************************************************
 * Funções privadas
 ****************************************************************************/

/**
 * Libera memória de todos os elementos da fila à direita da sentinela
 * \param element Primeiro elemento da fila
 */
Element* UNDOREDOCELLS_freeRightSentinel(Element* element){
    if(!element) return NULL;

    if(element->next){
        element = element->next;
        free(element->previous);
        element->previous = NULL;
        return UNDOREDOCELLS_freeRightSentinel(element);
    }
    else{
        free(element);
        return NULL;
    }
}

/**
 * Libera memória de todos os elementos da fila à esquerda da sentinela
 * \param element Primeiro elemento da fila
 */
Element* UNDOREDOCELLS_freeLeftSentinel(Element* element){
    if(!element) return NULL;

    if(element->previous){
        element = element->previous;
        free(element->next);
        element->next = NULL;
        return UNDOREDOCELLS_freeLeftSentinel(element);
    }
    else{
        free(element);
        return NULL;
    }
}

/****************************************************************************
 * Funções públicas
 ****************************************************************************/

/**
 * Aloca memória para a fila de desfazer/refazer
 * \return Retorna ponteiro para a memória alocada ou NULL em caso de falha de alocação
 */
UndoRedoCells* UNDOREDOCELLS_create(){
    UndoRedoCells* undoRedoCells = malloc(sizeof(UndoRedoCells));
    if(!undoRedoCells) return NULL;

    undoRedoCells->sentinel = malloc(sizeof(Element));
    if(!undoRedoCells->sentinel){
        free(undoRedoCells);
        return NULL;
    }

    undoRedoCells->sentinel->next = NULL;
    undoRedoCells->sentinel->previous = NULL;
    undoRedoCells->first = NULL;
    undoRedoCells->last = NULL;

    return undoRedoCells;
}

/**
 * Libera memória da fila
 * \return Retorna NULL
 * \param undoRedoCells Ponteiro para UndoRedoCells
 */
UndoRedoCells* UNDOREDOCELLS_free(UndoRedoCells* undoRedoCells){
    if(!undoRedoCells) return NULL;

    undoRedoCells->sentinel->previous =
            UNDOREDOCELLS_freeLeftSentinel(undoRedoCells->sentinel->previous);
    undoRedoCells->sentinel->next =
            UNDOREDOCELLS_freeRightSentinel(undoRedoCells->sentinel->next);

    free(undoRedoCells->sentinel);

    undoRedoCells->first = NULL;
    undoRedoCells->last = NULL;
    undoRedoCells->sentinel = NULL;

    free(undoRedoCells);

    return NULL;
}

/**
 * Adiciona um novo item na lista undo (lista redo é apagada)
 * \return Retorna 1 em caso de sucesso, ou 0 em caso de falha de alocação
 * \param undoRedoCells Ponteiro duplo para UndoRedoCells
 * \param oldExpression Expression anterior da célula
 * \param newExpression Nova expressão da célula
 * \param cellValue Valor (posição) da célula
 */
int UNDOREDOCELLS_newItem(UndoRedoCells** undoRedoCells, const char* oldExpression,
        const char* newExpression, int cellValue){
    if(!(*undoRedoCells)) return 0;

    Element* newElement = malloc(sizeof(Element));
    if(!newElement) return 0;

    strcpy(newElement->oldExpression, oldExpression);
    strcpy(newElement->newExpression, newExpression);
    newElement->cellValue = cellValue;
    newElement->next = NULL;

    // coloca novo elemento na posição correta
    if(!(*undoRedoCells)->last){
        (*undoRedoCells)->last = newElement;
        (*undoRedoCells)->sentinel->next = newElement;
        newElement->previous = (*undoRedoCells)->sentinel;
    }
    else{
        (*undoRedoCells)->last->next = newElement;
        newElement->previous = (*undoRedoCells)->last;
        (*undoRedoCells)->last = (*undoRedoCells)->last->next;
    }

    // apaga elementos do começo da fila até a sentinela
    while((*undoRedoCells)->first){
        (*undoRedoCells)->first = (*undoRedoCells)->first->next;
        free((*undoRedoCells)->first->previous);
        (*undoRedoCells)->first->previous = NULL;

        if((*undoRedoCells)->first == (*undoRedoCells)->sentinel)
            (*undoRedoCells)->first = NULL;
    }

    return 1;
}

/**
 * Verifica se é possível executar operação undo
 * \return Um valor diferente de 0 se puder executar undo, e 0 em caso contrário
 * \param undoRedoCells Ponteiro duplo para UndoRedoCells
 */
int UNDOREDOCELLS_canUndo(UndoRedoCells** undoRedoCells){
    if(!(*undoRedoCells)) return 0;

    return ((*undoRedoCells)->last != NULL);
}

/**
 * Verifica se é possível executar operação redo
 * \return Um valor diferente de 0 se puder executar redo, e 0 em caso contrário
 * \param undoRedoCells Ponteiro duplo para UndoRedoCells
 */
int UNDOREDOCELLS_canRedo(UndoRedoCells** undoRedoCells){
    if(!(*undoRedoCells)) return 0;

    return ((*undoRedoCells)->first != NULL);
}

/**
 * Desfaz última operação feita pelo usuário, retornando dados necessários
 * \return Retorna 1 em caso de sucesso, e 0 em caso de falha
 * \param undoRedoCells Ponteiro duplo para UndoRedoCells
 * \param expression Ponteiro para variável string a ser preenchida
 * \param cellValue Variável a ser preenchida com a localização da célula
 */
int UNDOREDOCELLS_undo(UndoRedoCells** undoRedoCells, char *expression, int *cellValue){
    if(!(*undoRedoCells) || !(*undoRedoCells)->last) return 0;

    strcpy(expression, (*undoRedoCells)->last->oldExpression);
    *cellValue = (*undoRedoCells)->last->cellValue;

    (*undoRedoCells)->last = (*undoRedoCells)->last->previous;

    if(!(*undoRedoCells)->first){

        (*undoRedoCells)->first = (*undoRedoCells)->last->next;
        (*undoRedoCells)->first->next = (*undoRedoCells)->sentinel;
        (*undoRedoCells)->first->previous = NULL;

        (*undoRedoCells)->sentinel->previous = (*undoRedoCells)->first;

    }
    else{

        (*undoRedoCells)->first->previous = (*undoRedoCells)->last->next;
        (*undoRedoCells)->first->previous->next = (*undoRedoCells)->first;
        (*undoRedoCells)->first = (*undoRedoCells)->first->previous;
        (*undoRedoCells)->first->previous = NULL;
    }

    (*undoRedoCells)->last->next = NULL;

    if((*undoRedoCells)->last == (*undoRedoCells)->sentinel)
        (*undoRedoCells)->last = NULL;

    return 1;
}

/**
 * Refaz última operação desfeita, retornando dados necessários
 * \return Retorna 1 em caso de sucesso, e 0 em caso de falha
 * \param undoRedoCells Ponteiro duplo para UndoRedoCells
 * \param expression Ponteiro para variável string a ser preenchida
 * \param cellValue Variável a ser preenchida com a localização da célula
 */
int UNDOREDOCELLS_redo(UndoRedoCells** undoRedoCells, char *expression, int *cellValue){
    if(!(*undoRedoCells) || !(*undoRedoCells)->first) return 0;

    strcpy(expression, (*undoRedoCells)->first->newExpression);
    *cellValue = (*undoRedoCells)->first->cellValue;

    (*undoRedoCells)->first = (*undoRedoCells)->first->next;

    if(!(*undoRedoCells)->last){

        (*undoRedoCells)->last = (*undoRedoCells)->first->previous;
        (*undoRedoCells)->last->previous = (*undoRedoCells)->sentinel;
        (*undoRedoCells)->last->next = NULL;

        (*undoRedoCells)->sentinel->next = (*undoRedoCells)->last;
    }
    else{
        (*undoRedoCells)->last->next = (*undoRedoCells)->first->previous;
        (*undoRedoCells)->last->next->previous = (*undoRedoCells)->last;
        (*undoRedoCells)->last = (*undoRedoCells)->last->next;
        (*undoRedoCells)->last->next = NULL;
    }

    (*undoRedoCells)->first->previous = NULL;

    if((*undoRedoCells)->first == (*undoRedoCells)->sentinel)
        (*undoRedoCells)->first = NULL;

    return 1;
}
