/**
 * \file graphics_instructions.c
 * Implementação do arquivo graphics_instructions.h
 */

#include "graphics_instructions.h"

// define o nome da janela
#define WINDOWNAME "instructions"
// define a posição x do nome da janela
#define WINDOWNAMEX 2
// define a posição y do nome da janela
#define WINDOWNAMEY 0

// define a borda lateral da janela
#define WINDOWBORDERLATERAL '*'
// define a borda superior e inferior da janela
#define WINDOWBORDERUPDOWN '*'

/*******************************************************************************
 * Estruturas
 *******************************************************************************/

/**
 * Estrutura da janela de isntruções
 */
struct graphicInstructions{
    int positionX;
    int positionY;
    int width;
    int height;
    WINDOW* instructionsWindow;
};

/*******************************************************************************
 * Funções privadas
 *******************************************************************************/

/**
 * Desenha borda em volta da janela
 * \param graphic Ponteiro duplo para GraphicInstructions
 */
void GRAPHICINST_drawBox(GraphicInstructions** graphic){
    box((*graphic)->instructionsWindow,WINDOWBORDERLATERAL,WINDOWBORDERUPDOWN);
    mvwprintw((*graphic)->instructionsWindow,WINDOWNAMEY,WINDOWNAMEX,WINDOWNAME);
    wrefresh((*graphic)->instructionsWindow);
}

/*******************************************************************************
 * Funções públicas
 *******************************************************************************/

/**
 * Cria a janela de instruções e desenha na tela
 * \return Ponteiro para GraphicInstructions, ou NULL
 * \param positionX Posição x da janela
 * \param positionY Posição y da janela
 * \param width Largura da janela
 * \param height Altura da janela
 */
GraphicInstructions* GRAPHICINST_create(int positionX, int positionY,
        int width,int height){
    GraphicInstructions* graphic = malloc(sizeof(GraphicInstructions));
    if(!graphic) return NULL;

    graphic->positionX = positionX;
    graphic->positionY = positionY;
    graphic->width = width;
    graphic->height = height;

    graphic->instructionsWindow = newwin(height, width, positionY, positionX);
    if(!graphic->instructionsWindow){
        free(graphic);
        return NULL;
    }

    GRAPHICINST_drawBox(&graphic);

    return graphic;
}

/**
 * Libera memória alocada na janela de instruções
 * \return NULL
 * \param graphic Ponteiro para GraphicInstructions
 */
GraphicInstructions* GRAPHICINST_free(GraphicInstructions* graphic){
    if(!graphic) return graphic;

    wclear(graphic->instructionsWindow);
    wrefresh(graphic->instructionsWindow);
    delwin(graphic->instructionsWindow);

    free(graphic);
    return NULL;
}

/**
 * Limpa tela de instruções
 * \param graphic Ponteiro duplo para GraphicInstructions
 */
void GRAPHICINST_clear(GraphicInstructions** graphic){
    if(!graphic || !(*graphic)) return;

    wclear((*graphic)->instructionsWindow);
    GRAPHICINST_drawBox(&(*graphic));
}

/**
 * Escreve na tela de instruções na posição informada
 * \param graphic Ponteiro duplo para GraphicInstructions
 * \param instruction Instrução a ser escrita
 * \param positionX Coordenada x em que aparecerá a instrução
 * \param positionY Coordenada y em que aparecerá a isntrução
 */
void GRAPHICINST_write(GraphicInstructions** graphic, const char *instruction,
        int positionX, int positionY){
    if(!graphic || !(*graphic)) return;

    mvwprintw((*graphic)->instructionsWindow, positionY, positionX, "%s", instruction);
    wrefresh((*graphic)->instructionsWindow);
}

/**
 * Escreve instruções do teclado (up, down, left, right) na posição informada
 * \param graphic Ponteiro duplo para GraphicInstructions
 * \param positionX Coordenada x em que aparecerá a instrução
 * \param positionY Coordenada y em que aparecerá a instrução
 * \param leftRight Se aparece instruções direita e esquerda . booleano
 */
void GRAPHICINST_writeKeyboard(GraphicInstructions** graphic, int positionX,
        int positionY, int leftRight){
    if(!graphic || !(*graphic)) return;

    if(leftRight){
        char keyboardString[] = "LEFT <-   RIGHT ->   UP ^   DOWN v     ENTER";
        mvwprintw((*graphic)->instructionsWindow, positionY, positionX, "%s", keyboardString);
    }

    else {
        char keyboardString[] = "UP ^   DOWN v     ENTER";
        mvwprintw((*graphic)->instructionsWindow, positionY, positionX, "%s", keyboardString);
    }

    wrefresh((*graphic)->instructionsWindow);
}

/**
 * Recebe posição x da janela
 * \return Posição x da janela
 * \param graphic Ponteiro duplo para GraphicInstructions
 */
int GRAPHICINST_getPositionX(GraphicInstructions** graphic){
    return (*graphic)->positionX;
}

/**
 * Recebe posição y da janela
 * \return Posição y da janela
 * \param graphic Ponteiro duplo para GraphicInstructions
 */
int GRAPHICINST_getPositionY(GraphicInstructions** graphic){
    return (*graphic)->positionY;
}

/**
 * Recebe largura da janela
 * \return Largura da janela
 * \param graphic Ponteiro duplo para GraphicInstructions
 */
int GRAPHICINST_getWidth(GraphicInstructions** graphic){
    return (*graphic)->width;
}

/**
 * Recebe altura da janela
 * \return Altura da janela
 * \param graphic Ponteiro duplo para GraphicInstructions
 */
int GRAPHICINST_getHeight(GraphicInstructions** graphic){
    return (*graphic)->height;
}
