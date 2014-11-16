/**
 * \file graphics_cells.c
 * Implementação do arquivo graphics_cells.h
 */

#include "graphics_cells.h"

// quantidade máxima de células
#define MAX_CELLS 150

// estados possíveis
#define ON 1
#define OFF 0

// dimensões da célula
#define CELLWIDTH 10
#define CELLHEIGHT 3

/***************************************************************************
 * Estruturas
 ***************************************************************************/

/**
 * Estrutura de uma célula
 */
typedef struct windowCell WindowCell;
struct windowCell{
    WINDOW* cell;
    int status;
};

/**
 * Estrutura do objeto que guarda dados relativos ao gráfico das células
 */
struct graphicCells{
    int rows;
    int columns;
    WindowCell *windowCell[MAX_CELLS];
};

/***************************************************************************
 * Funções privadas
 ***************************************************************************/

void GRAPHICSCELLS_freeCells(WindowCell** cells, int maxCells, int count){
    printf("%d\n", maxCells);
    if(count >= maxCells) return;

    if(cells[count]){
        if(cells[count]->cell)
            delwin(cells[count]->cell);
        free(cells[count]);
        cells[count] = NULL;
    }
    GRAPHICSCELLS_freeCells(cells, maxCells, count+1);
}

/***************************************************************************
 * Funções públicas
 ***************************************************************************/

/**
 * Aloca memória para objeto GraphicCells, adiciona células de acordo com
 * a quantidade de linhas e colunas informadas, e desenha na tela
 * \return Ponteiro para GraphicCells, ou NULL em caso de falha de alocação
 * \param rows Quantidade de linhas
 * \param columns Quantidade de colunas
 */
GraphicCells* GRAPHICSCELLS_create(int rows, int columns){
    GraphicCells* graphic = malloc(sizeof(GraphicCells));
    if(!graphic) return NULL;

    int index;
    int countRow, countColumn;

    graphic->columns = columns;
    graphic->rows = rows;

    for(countRow=0; countRow < rows+1; countRow++){
        for(countColumn=0; countColumn<columns+1; countColumn++){
            index = countColumn + countRow*columns;

            graphic->windowCell[index] = malloc(sizeof(WindowCell));
            if(!graphic->windowCell[index]){
                graphic = GRAPHICSCELLS_free(graphic);
                return NULL;
            }

            if(countRow==1 && countColumn==1)
                graphic->windowCell[index]->status = ON;
            else
                graphic->windowCell[index]->status = OFF;

            graphic->windowCell[index]->cell = newwin(CELLHEIGHT, CELLWIDTH,
                    countRow*CELLHEIGHT, countColumn*CELLWIDTH);
            if(!graphic->windowCell[index]->cell){
                graphic = GRAPHICSCELLS_free(graphic);
                return NULL;
            }
            if(countRow != 0 && countColumn != 0){
                if(countRow==1 && countColumn==1)
                    box(graphic->windowCell[index]->cell, '*', '*');
                else
                    box(graphic->windowCell[index]->cell, '|', '-');
            }
            else{
                if(countRow==0 && countColumn!=0)
                    mvwprintw(graphic->windowCell[index]->cell, 1, 4, "%c", 64+countColumn);
                else if(countRow!=0)
                    mvwprintw(graphic->windowCell[index]->cell, 1, 4, "%d", countRow);
            }
            wrefresh(graphic->windowCell[index]->cell);
        }
    }

    return graphic;

}

/**
 * Desaloca memória de objeto graphicCells
 * \return NULL
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
GraphicCells* GRAPHICSCELLS_free(GraphicCells* graphicCells){
    if(!graphicCells) return NULL;

    int total = (graphicCells->columns+1) * (graphicCells->rows+1);

    GRAPHICSCELLS_freeCells(graphicCells->windowCell, total, 0);
    free(graphicCells);
    return NULL;
}

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
        int mark, int disable){
    if(!graphicCells || !(*graphicCells)) return 0;

    int index = column + row*((*graphicCells)->columns);

    wclear((*graphicCells)->windowCell[index]->cell);


    if(((*graphicCells)->windowCell[index]->status==ON && mark==0)
            || (mark==1)
            || (mark==2 && (*graphicCells)->windowCell[index]->status==OFF))
        box((*graphicCells)->windowCell[index]->cell, '*', '*');
    else
        box((*graphicCells)->windowCell[index]->cell, '|', '-');

    if(!disable)
        mvwprintw((*graphicCells)->windowCell[index]->cell, 1, 2, "%.2f", value);
    else
        mvwprintw((*graphicCells)->windowCell[index]->cell, 1, 2, "");
    wrefresh((*graphicCells)->windowCell[index]->cell);

    return 1;
}

/**
 * Pega posição da borda inferior do conjunto inteiro de células
 * \return Posição da borda inferior do conjunto de células
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
int GRAPHICSCELLS_getBottomPosition(GraphicCells** graphicCells){
    if(!graphicCells || !(*graphicCells)) return 0;

    return (((*graphicCells)->rows+1)*CELLHEIGHT);
}
