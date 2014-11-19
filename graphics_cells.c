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

// borda lateral da célula "ligada"
#define CELLLATERALBORDERON '*'
// borda superior e inferior "desligada"
#define CELLUPDOWNBORDERON '*'
// borda lateral da célula "desligada"
#define CELLLATERALBORDEROFF '|'
// borda superior e inferior "desligada"
#define CELLUPDOWNBORDEROFF '-'

// equivalente a uma linha
#define ROW 1
// equivalente a uma coluna
#define COLUMN 1

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

    if(count >= maxCells) return;

    if(cells[count]){
        if(cells[count]->cell)
            delwin(cells[count]->cell);
        free(cells[count]);
        cells[count] = NULL;
    }
    GRAPHICSCELLS_freeCells(cells, maxCells, count+1);
}

int GRAPHICSCELLS_getIndex(int row, int column, int max_columns){
    return ((column)+(row)*(max_columns+1));
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

    for(countRow=0; countRow <= rows; countRow++){
        for(countColumn=0; countColumn <= columns; countColumn++){
            index = countColumn + countRow*(columns+1);

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
                    box(graphic->windowCell[index]->cell, CELLLATERALBORDERON, CELLUPDOWNBORDERON);
                else
                    box(graphic->windowCell[index]->cell, CELLLATERALBORDEROFF, CELLUPDOWNBORDEROFF);
            }
            else{
                if(countRow==0 && countColumn!=0)
                    mvwprintw(graphic->windowCell[index]->cell, ROW*1, CELLWIDTH/2, "%c",
                            64+countColumn);
                else if(countRow!=0)
                    mvwprintw(graphic->windowCell[index]->cell, ROW*1, CELLWIDTH/2, "%d", countRow);
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
 * \param mark Informe MARK_ON se deseja que a célula seja destacada, MARK_OFF se quer retirar
 * o destaque, KEEP_MARK se quiser manter o estado atual, e CHANGE_MARK se deseja mudar o estado
 * atual (de destaque para sem destaque, ou o contrário)
 * \param disable informe DISABLE para que apareça uma célula vazia, e ENABLE caso contrário
 */
int GRAPHICSCELLS_updateCell(GraphicCells** graphicCells, int row, int column, double value,
        int mark, int disable){
    if(!graphicCells || !(*graphicCells)) return 0;

    int index = GRAPHICSCELLS_getIndex(row,column, (*graphicCells)->columns);

    wclear((*graphicCells)->windowCell[index]->cell);


    if(((*graphicCells)->windowCell[index]->status==ON && mark==KEEP_MARK)
            || (mark==MARK_ON)
            || (mark==CHANGE_MARK && (*graphicCells)->windowCell[index]->status==OFF))
        box((*graphicCells)->windowCell[index]->cell, CELLLATERALBORDERON, CELLUPDOWNBORDERON);
    else
        box((*graphicCells)->windowCell[index]->cell, CELLLATERALBORDEROFF, CELLUPDOWNBORDEROFF);

    if(disable==ENABLE)
        mvwprintw((*graphicCells)->windowCell[index]->cell, ROW*1, COLUMN*2, "%.2f", value);
    else
        mvwprintw((*graphicCells)->windowCell[index]->cell, ROW*1, COLUMN*2, "");
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

/**
 * Permite ao usuário selecionar uma célula
 * \param graphicCells Ponteiro para objeto GraphicCells
 * \param cellRow Linha atual (e será preenchida com a linha da célula selecionada)
 * \param cellColumn Coluna atual (e será preenchida com a coluna da célula selecionada)
 */
void GRAPHICSCELL_selectCell(GraphicCells** graphicCells, int *cellRow, int *cellColumn){
    if(!graphicCells || !(*graphicCells)) return;

    int index = GRAPHICSCELLS_getIndex(*cellRow, *cellColumn, (*graphicCells)->columns);

    keypad(stdscr,TRUE);
    cbreak();

    int c = 0;

    // enquanto não pressionar enter
    while(c!=10){
        int c = getch();

        switch(c){
        case KEY_UP:
            box((*graphicCells)->windowCell[index]->cell, CELLLATERALBORDEROFF, CELLUPDOWNBORDEROFF);
            wrefresh((*graphicCells)->windowCell[index]->cell);

            if((*cellRow)==1) (*cellRow) = (*graphicCells)->rows;
            else (*cellRow)--;

            index = GRAPHICSCELLS_getIndex(*cellRow, *cellColumn, (*graphicCells)->columns);
            box((*graphicCells)->windowCell[index]->cell, CELLLATERALBORDERON, CELLUPDOWNBORDERON);
            wrefresh((*graphicCells)->windowCell[index]->cell);
            break;
        case KEY_DOWN:
            box((*graphicCells)->windowCell[index]->cell, CELLLATERALBORDEROFF, CELLUPDOWNBORDEROFF);
            wrefresh((*graphicCells)->windowCell[index]->cell);

            if((*cellRow)==(*graphicCells)->rows) (*cellRow) = 1;
            else (*cellRow)++;

            index = GRAPHICSCELLS_getIndex(*cellRow, *cellColumn, (*graphicCells)->columns);
            box((*graphicCells)->windowCell[index]->cell, CELLLATERALBORDERON, CELLUPDOWNBORDERON);
            wrefresh((*graphicCells)->windowCell[index]->cell);
            break;
        case KEY_LEFT:
            box((*graphicCells)->windowCell[index]->cell, CELLLATERALBORDEROFF, CELLUPDOWNBORDEROFF);
            wrefresh((*graphicCells)->windowCell[index]->cell);

            if((*cellColumn)==1) (*cellColumn) = (*graphicCells)->columns;
            else (*cellColumn)--;

            index = GRAPHICSCELLS_getIndex(*cellRow, *cellColumn, (*graphicCells)->columns);
            box((*graphicCells)->windowCell[index]->cell, CELLLATERALBORDERON, CELLUPDOWNBORDERON);
            wrefresh((*graphicCells)->windowCell[index]->cell);
            break;
        case KEY_RIGHT:
            box((*graphicCells)->windowCell[index]->cell, CELLLATERALBORDEROFF, CELLUPDOWNBORDEROFF);
            wrefresh((*graphicCells)->windowCell[index]->cell);

            if((*cellColumn)==(*graphicCells)->columns) (*cellColumn) = 1;
            else (*cellColumn)++;

            index = GRAPHICSCELLS_getIndex(*cellRow, *cellColumn, (*graphicCells)->columns);
            box((*graphicCells)->windowCell[index]->cell, CELLLATERALBORDERON, CELLUPDOWNBORDERON);
            wrefresh((*graphicCells)->windowCell[index]->cell);
            break;
        case 10:
            return;
        }
    }

}
