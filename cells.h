/**
 * \file cells.h
 * Arquivo que descreve a manipulação de matriz de células da planilha
 */

#ifndef CELLS_H_
#define CELLS_H_

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
 */
void CELLS_getExpression(Cells* cells, int row, int column, char *expression);

#endif /* CELLS_H_ */
