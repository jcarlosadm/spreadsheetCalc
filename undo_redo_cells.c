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
    char expression[60];
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
 * Libera memória de todos os elementos da fila
 * \param element Primeiro elemento da fila
 */
Element* UNDOREDOCELLS_freeElements(Element* element){
    if(!element) return NULL;

    if(element->next){
        element = element->next;
        free(element->previous);
        element->previous = NULL;
        return UNDOREDOCELLS_freeElements(element);
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
    undoRedoCells->first = UNDOREDOCELLS_freeElements(undoRedoCells->first);
    undoRedoCells->last = NULL;
    undoRedoCells->sentinel = NULL;

    free(undoRedoCells);

    return NULL;
}

