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
 * Enumerador de marcação
 */
enum mark{
    MARK_ON, ///< destacar célula
    MARK_OFF, ///< retirar destaque
    KEEP_MARK, ///< manter o destaque atual
    CHANGE_MARK ///< mudar o destaque atual
};

/**
 * Estrutura do objeto que guarda dados relativos ao gráfico das células
 */
typedef struct graphicCells GraphicCells;

/**
 * Aloca memória para objeto GraphicCells, adiciona células de acordo com
 * a quantidade de linhas e colunas informadas, e desenha na tela
 * \return Ponteiro para GraphicCells, ou NULL em caso de falha de alocação
 * \param positionX Posição x da janela
 * \param positionY Posição y da janela
 * \param rows Quantidade de linhas
 * \param columns Quantidade de colunas
 */
GraphicCells* GRAPHICSCELLS_create(int positionX, int positionY, int rows, int columns);

/**
 * Desaloca memória de objeto graphicCells
 * \return NULL
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
GraphicCells* GRAPHICSCELLS_free(GraphicCells* graphicCells);

/**
 * Atualiza o gráfico de uma célula
 * \return 1 em caso de sucesso, ou 0 em caso de falha
 * \param graphicCells Ponteiro para objeto GraphicCells
 * \param row Linha da célula
 * \param column Coluna da célula
 * \param value Valor a ser colocado na célula
 * \param mark Informe MARK_ON se deseja que a célula seja destacada, MARK_OFF se quer retirar
 * o destaque, KEEP_MARK se quiser manter o estado atual, e CHANGE_MARK se deseja mudar o estado
 * atual (de destaque para sem destaque, ou o contrário)
 * \param disable Se desabilita célula. booleano
 */
int GRAPHICSCELLS_updateCell(GraphicCells** graphicCells, int row, int column, double value,
        int mark, int disable);

/**
 * Pega altura da janela
 * \return Altura da janela
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
int GRAPHICSCELLS_getHeight(GraphicCells** graphicCells);

/**
 * Pega largura da janela de células
 * \return Largura da janela
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
int GRAPHICSCELLS_getWidth(GraphicCells** graphicCells);

/**
 * Pega posição x da janela
 * \return Posição x da janela
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
int GRAPHICSCELLS_getPositionX(GraphicCells** graphicCells);

/**
 * Pega posição y da janela
 * \return Posição y da janela
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
int GRAPHICSCELLS_getPositionY(GraphicCells** graphicCells);

/**
 * Permite ao usuário selecionar uma célula
 * \param graphicCells Ponteiro para objeto GraphicCells
 * \param cellRow Linha atual (e será preenchida com a linha da célula selecionada)
 * \param cellColumn Coluna atual (e será preenchida com a coluna da célula selecionada)
 */
void GRAPHICSCELL_selectCell(GraphicCells** graphicCells, int *cellRow, int *cellColumn);

#endif /* GRAPHICS_H_ */
