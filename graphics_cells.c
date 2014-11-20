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
    int positionX;
    int positionY;
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

/**
 * Pega índice da célula com base em sua linha e coluna, e o máximo de colunas
 * \return Índice da célula
 * \param row Linha da célula
 * \param column Coluna da célula
 * \param max_columns Número de colunas da matriz
 */
int GRAPHICSCELLS_getIndex(int row, int column, int max_columns){
    return ((column)+(row)*(max_columns+1));
}

/**
 * Desenha caixa, habilitada ou desabilitada
 * \param cell Ponteiro para WindowCell
 * \param status Estado da célula (ON ou OFF)
 */
void GRAPHICSCELLS_drawBox(WindowCell** cell, int status){
    if(status==ON)
        box((*cell)->cell,CELLLATERALBORDERON,CELLUPDOWNBORDERON);
    else
        box((*cell)->cell,CELLLATERALBORDEROFF, CELLUPDOWNBORDEROFF);
    wrefresh((*cell)->cell);
}

/***************************************************************************
 * Funções públicas
 ***************************************************************************/

/**
 * Aloca memória para objeto GraphicCells, adiciona células de acordo com
 * a quantidade de linhas e colunas informadas, e desenha na tela
 * \return Ponteiro para GraphicCells, ou NULL em caso de falha de alocação
 * \param positionX Posição x da janela
 * \param positionY Posição y da janela
 * \param rows Quantidade de linhas
 * \param columns Quantidade de colunas
 */
GraphicCells* GRAPHICSCELLS_create(int positionX, int positionY, int rows, int columns){
    GraphicCells* graphic = malloc(sizeof(GraphicCells));
    if(!graphic) return NULL;

    int index;
    int countRow, countColumn;

    graphic->positionX = positionX;
    graphic->positionY = positionY;
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
                    countRow*CELLHEIGHT+positionY, countColumn*CELLWIDTH+positionX);
            if(!graphic->windowCell[index]->cell){
                graphic = GRAPHICSCELLS_free(graphic);
                return NULL;
            }
            if(countRow != 0 && countColumn != 0){
                if(countRow==1 && countColumn==1)
                    GRAPHICSCELLS_drawBox(&(graphic->windowCell[index]),ON);
                else
                    GRAPHICSCELLS_drawBox(&(graphic->windowCell[index]),OFF);
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
 * \param disable Se desabilita célula. booleano
 */
int GRAPHICSCELLS_updateCell(GraphicCells** graphicCells, int row, int column, double value,
        int mark, int disable){
    if(!graphicCells || !(*graphicCells)) return 0;

    int index = GRAPHICSCELLS_getIndex(row,column, (*graphicCells)->columns);

    wclear((*graphicCells)->windowCell[index]->cell);


    if(((*graphicCells)->windowCell[index]->status==ON && mark==KEEP_MARK)
            || (mark==MARK_ON)
            || (mark==CHANGE_MARK && (*graphicCells)->windowCell[index]->status==OFF))
        GRAPHICSCELLS_drawBox(&((*graphicCells)->windowCell[index]),ON);
    else
        GRAPHICSCELLS_drawBox(&((*graphicCells)->windowCell[index]),OFF);

    if(!disable)
        mvwprintw((*graphicCells)->windowCell[index]->cell, ROW*1, COLUMN*2, "%.2f", value);
    else
        mvwprintw((*graphicCells)->windowCell[index]->cell, ROW*1, COLUMN*2, "");
    wrefresh((*graphicCells)->windowCell[index]->cell);

    return 1;
}

/**
 * Pega altura da janela
 * \return Altura da janela
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
int GRAPHICSCELLS_getHeight(GraphicCells** graphicCells){
    if(!graphicCells || !(*graphicCells)) return 0;

    return (((*graphicCells)->rows+1)*CELLHEIGHT);
}

/**
 * Pega largura da janela de células
 * \return Largura da janela
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
int GRAPHICSCELLS_getWidth(GraphicCells** graphicCells){

    return (((*graphicCells)->columns+1)*CELLWIDTH);
}

/**
 * Pega posição x da janela
 * \return Posição x da janela
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
int GRAPHICSCELLS_getPositionX(GraphicCells** graphicCells){
    return (*graphicCells)->positionX;
}

/**
 * Pega posição y da janela
 * \return Posição y da janela
 * \param graphicCells Ponteiro para objeto GraphicCells
 */
int GRAPHICSCELLS_getPositionY(GraphicCells** graphicCells){
    return (*graphicCells)->positionY;
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

    // habilita teclado
    keypad(stdscr,TRUE);
    cbreak();

    int keyPressed = 0;

    // enquanto não pressionar enter
    while(keyPressed!=10){
        // recebe pressionamento de tecla
        keyPressed = getch();

        if(keyPressed!=10)
            GRAPHICSCELLS_drawBox(&((*graphicCells)->windowCell[index]),OFF);

        // faz algo de acordo com a tecla pressionada
        switch(keyPressed){
        case KEY_UP:
            if((*cellRow)==1) (*cellRow) = (*graphicCells)->rows;
            else (*cellRow)--;
            index = GRAPHICSCELLS_getIndex(*cellRow, *cellColumn, (*graphicCells)->columns);
            break;
        case KEY_DOWN:
            if((*cellRow)==(*graphicCells)->rows) (*cellRow) = 1;
            else (*cellRow)++;
            index = GRAPHICSCELLS_getIndex(*cellRow, *cellColumn, (*graphicCells)->columns);
            break;
        case KEY_LEFT:
            if((*cellColumn)==1) (*cellColumn) = (*graphicCells)->columns;
            else (*cellColumn)--;
            index = GRAPHICSCELLS_getIndex(*cellRow, *cellColumn, (*graphicCells)->columns);
            break;
        case KEY_RIGHT:
            if((*cellColumn)==(*graphicCells)->columns) (*cellColumn) = 1;
            else (*cellColumn)++;
            index = GRAPHICSCELLS_getIndex(*cellRow, *cellColumn, (*graphicCells)->columns);
            break;
        }

        if(keyPressed!=10)
            GRAPHICSCELLS_drawBox(&((*graphicCells)->windowCell[index]),ON);
    }

}
