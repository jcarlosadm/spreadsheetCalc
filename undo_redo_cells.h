/**
 * \file undo_redo_cells.h
 * Arquivo que trata de uma fila de desfazer/refazer dados de uma célula
 */

#ifndef UNDO_REDO_CELLS_H_
#define UNDO_REDO_CELLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Estrutura da fila de desfazer/refazer das células
 */
typedef struct undoRedoCells UndoRedoCells;

/**
 * Aloca memória para a fila de desfazer/refazer
 * \return Retorna ponteiro para a memória alocada, ou NULL em caso de falha de alocação
 */
UndoRedoCells* UNDOREDOCELLS_create();

/**
 * Libera memória da fila
 * \return Retorna NULL
 * \param undoRedoCells Ponteiro para UndoRedoCells
 */
UndoRedoCells* UNDOREDOCELLS_free(UndoRedoCells* undoRedoCells);

/**
 * Adiciona um novo item na lista undo (lista redo é apagada)
 * \return Retorna 1 em caso de sucesso, ou 0 em caso de falha de alocação
 * \param undoRedoCells Ponteiro duplo para UndoRedoCells
 * \param oldExpression Expression anterior da célula
 * \param newExpression Nova expressão da célula
 * \param cellValue Valor (posição) da célula
 */
int UNDOREDOCELLS_newItem(UndoRedoCells** undoRedoCells, const char* oldExpression,
        const char* newExpression, int cellValue);

/**
 * Verifica se é possível executar operação undo
 * \return Um valor diferente de 0 se puder executar undo, e 0 em caso contrário
 * \param undoRedoCells Ponteiro duplo para UndoRedoCells
 */
int UNDOREDOCELLS_canUndo(UndoRedoCells** undoRedoCells);

/**
 * Verifica se é possível executar operação redo
 * \return Um valor diferente de 0 se puder executar redo, e 0 em caso contrário
 * \param undoRedoCells Ponteiro duplo para UndoRedoCells
 */
int UNDOREDOCELLS_canRedo(UndoRedoCells** undoRedoCells);

/**
 * Desfaz última operação feita pelo usuário, retornando dados necessários
 * \return Retorna 1 em caso de sucesso, e 0 em caso de falha
 * \param undoRedoCells Ponteiro duplo para UndoRedoCells
 * \param expression Ponteiro para variável string a ser preenchida
 * \param cellValue Variável a ser preenchida com a localização da célula
 */
int UNDOREDOCELLS_undo(UndoRedoCells** undoRedoCells, char *expression, int *cellValue);

/**
 * Refaz última operação desfeita, retornando dados necessários
 * \return Retorna 1 em caso de sucesso, e 0 em caso de falha
 * \param undoRedoCells Ponteiro duplo para UndoRedoCells
 * \param expression Ponteiro para variável string a ser preenchida
 * \param cellValue Variável a ser preenchida com a localização da célula
 */
int UNDOREDOCELLS_redo(UndoRedoCells** undoRedoCells, char *expression, int *cellValue);

#endif /* UNDO_REDO_CELLS_H_ */
