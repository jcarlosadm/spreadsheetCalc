/**
 * \file matrix.h
 * Arquivo que descreve a manipulação de matriz de células da planilha
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "binary_expression_tree.h"
#include "stack_binExpTree.h"
#include "functions.h"
#include "undo_redo_cells.h"
#include "graphics_cells.h"
#include "graphics_instructions.h"

/**
 * Define uma quantidade padrão de linhas para a matriz
 */
#define ROWS 6

/**
 * Define uma quantidade padrão de colunas para a matriz
 */
#define COLUMNS 13

/**
 * Estrutura da matriz de células da planilha
 */
typedef struct matrix Matrix;

/**
 * Cria uma matriz com a quantidade de linhas e colunas especificadas
 * \return Ponteiro para a matriz criada
 * \param rows Quantidade de linhas da matriz
 * \param columns Quantidade de colunas da matriz
 */
Matrix* MATRIX_create(int rows, int columns);

/**
 * Libera memória alocada na matriz
 * \return NULL
 * \param matrix Ponteiro para matriz Matrix
 */
Matrix* MATRIX_free(Matrix* matrix);

/**
 * Pega número de linhas da matriz
 * \return Linhas da matriz. -1 em caso de erro
 * \param matrix Ponteiro duplo para matriz Matrix
 */
int MATRIX_getRows(Matrix** matrix);

/**
 * Pega número de colunas da matriz
 * \return Colunas da matriz. -1 em caso de erro
 * \param matrix Ponteiro duplo para matriz Matrix
 */
int MATRIX_getColumns(Matrix** matrix);

/**
 * Obtém expressão de uma célula específica da matriz
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param row Linha da célula
 * \param column  Coluna da célula
 * \param expression String a ser preenchida com a expressão da célula.
 * Se a célula não estiver alocada ou não houver expressão, essa variável
 * é preenchida com uma string vazia ("")
 */
void MATRIX_getExpression(Matrix** matrix, int row, int column, char *expression);

/**
 * Obtém valor da célula
 * \return Valor da célula
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param row Linha da célula
 * \param column Coluna da célula
 */
double MATRIX_getValue(Matrix** matrix, int row, int column);

/**
 * Define uma expressão para uma célula específica, calculando o seu valor no processo
 * \return 1 se obtiver sucesso, e 0 caso contrário
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param row Linha da célula
 * \param column Coluna da célula
 * \param expression expressão a ser colocada na célula
 * \param undoRedo Ponteiro duplo para fila de desfazer/refazer. Informe NULL caso
 * não queira guarda a informação na fila de desfazer/refazer
 * \param graphic Ponteiro duplo para GraphicCells
 */
int MATRIX_setExpression(Matrix** matrix, int row, int column, const char* expression,
        UndoRedoCells** undoRedo, GraphicCells** graphic);

/**
 * Tenta realizar operação de desfazer na matriz de células
 * \return 1 se obtiver sucesso e 0 em caso contrário
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param undoRedo Ponteiro duplo para fila de desfazer/refazer
 * \param graphic Ponteiro duplo para GraphicCells
 */
int MATRIX_undo(Matrix** matrix, UndoRedoCells** undoRedo, GraphicCells** graphic);

/**
 * Tenta realizar operação de refazer na matriz de células
 * \return 1 se obtiver sucesso e 0 em caso contrário
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param undoRedo Ponteiro duplo para fila de desfazer/refazer
 * \param graphic Ponteiro duplo para GraphicCells
 */
int MATRIX_redo(Matrix** matrix, UndoRedoCells** undoRedo, GraphicCells** graphic);

/**
 * Valida expressão
 * \return 1 se a expressão for válida, 0 em caso contrário
 * \param graphic Ponteiro duplo para GraphicInstructions (para informar o usuário
 * de possíveis erros). Informe NULL se não desejar imprimir essas informações
 * \param rows Quantidade de linhas da matriz de células
 * \param columns Quantidade de colunas da matriz de células
 * \param expression Expressão a ser validada
 */
int MATRIX_validateExpression(GraphicInstructions** graphic, int rows, int columns,
        const char *expression);

/**
 * Checa se haverá dependência cíclica se uma expressão for configurada em uma célula
 * \return 1 se houver dependência na expressão, 0 em caso contrário
 * \param rows Quantidade de linhas da matriz de células
 * \param columns Quantidade de colunas da matriz de células
 * \param expression Expressão a ser validada
 * \param matrix Ponteiro duplo para matriz Matrix
 */
int MATRIX_checkCyclicDependency(int row, int column, const char* expression, Matrix** matrix);

#endif /* MATRIX_H_ */
