/**
 * \file graphics_cells.h
 * Trata dos dados das células da planilha
 */

#ifndef GRAPHICS_CELLS_H_
#define GRAPHICS_CELLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

/**
 * Estrutura do objeto que guarda dados relativos ao gráfico das células
 */
typedef struct graphicCells GraphicCells;

/**
 * Aloca memória para objeto GraphicCells, adiciona células de acordo com
 * a quantidade de linhas e colunas informadas, e desenha na tela
 * \return Ponteiro para GraphicCells, ou NULL em caso de falha de alocação
 * \param rows Quantidade de linhas
 * \param columns Quantidade de colunas
 */
GraphicCells* GRAPHICSCELLS_create(int rows, int columns);

/**
 * Desaloca memória de objeto graphicCells
 * \return NULL
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
GraphicCells* GRAPHICSCELLS_free(GraphicCells* graphicCells);

/**
 * Atualiza o gráfico de uma célula
 * \return 1 em caso de sucesso, ou 0 em caso de falha
 * \param row Linha da célula
 * \param column Coluna da célula
 * \param value Valor a ser colocado na célula
 * \param mark Informe 1 se deseja que a célula seja destacada, -1 se quer retirar o destaque,
 * 0 se quiser manter o estado atual, e 2 se deseja mudar o estado atual (de destaque para sem
 * destaque, ou o contrário)
 * \param disable 1 para que apareça uma célula vazia, 0 caso contrário
 */
int GRAPHICSCELLS_updateCell(GraphicCells** graphicCells, int row, int column, double value,
        int mark, int disable);

#endif /* GRAPHICS_H_ */
