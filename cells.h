/**
 * \file cells.h
 * Arquivo que descreve a manipulação de matriz de células da planilha
 */

#ifndef CELLS_H_
#define CELLS_H_

#include <stdio.h>
#include <stdlib.h>

#include "binary_expression_tree.h"
#include "stack_binExpTree.h"
#include "functions.h"
#include "undo_redo_cells.h"

/**
 * Estrutura da matriz de células da planilha
 */
typedef struct cells Cells;

/**
 * Cria uma matriz com a quantidade de linhas e colunas especificadas
 * \return Ponteiro para a matriz criada
 * \param rows Quantidade de linhas da matriz
 * \param columns Quantidade de colunas da matriz
 */
Cells* CELLS_create(int rows, int columns);

/**
 * Libera memória alocada na matriz
 * \return NULL
 * \param cells Ponteiro para matriz Cells
 */
Cells* CELLS_free(Cells* cells);

/**
 * Obtém expressão de uma célula específica da matriz
 * \param cells Ponteiro duplo para matriz Cells
 * \param row Linha da célula
 * \param column  Coluna da célula
 * \param expression String a ser preenchida com a expressão da célula
 */
void CELLS_getExpression(Cells** cells, int row, int column, char *expression);

/**
 * Obtém valor da célula
 * \return Valor da célula
 * \param cells Ponteiro duplo para matriz Cells
 * \param row Linha da célula
 * \param column Coluna da célula
 */
double CELLS_getValue(Cells** cells, int row, int column);

/**
 * Define uma expressão para uma célula específica, calculando o seu valor no processo
 * \return 1 se obtiver sucesso, e 0 caso contrário
 * \param cells Ponteiro duplo para matriz Cells
 * \param row Linha da célula
 * \param column Coluna da célula
 * \param expression expressão a ser colocada na célula
 * \param undoRedo Ponteiro duplo para fila de desfazer/refazer. Informe NULL caso
 * não deseje guarda a informação na fila de desfazer/refazer
 */
int CELLS_setExpression(Cells** cells, int row, int column, const char* expression,
        UndoRedoCells** undoRedo);

/**
 * Tenta realizar operação de desfazer na matriz de células
 * \return 1 se obtiver sucesso e 0 em caso contrário
 * \param cells Ponteiro duplo para matriz Cells
 * \param undoRedo Ponteiro duplo para fila de desfazer/refazer
 */
int CELLS_undo(Cells** cells, UndoRedoCells** undoRedo);

/**
 * Tenta realizar operação de refazer na matriz de células
 * \return 1 se obtiver sucesso e 0 em caso contrário
 * \param cells Ponteiro duplo para matriz Cells
 * \param undoRedo Ponteiro duplo para fila de desfazer/refazer
 */
int CELLS_redo(Cells** cells, UndoRedoCells** undoRedo);

#endif /* CELLS_H_ */
