/**
 * \file graphics_instructions.c
 * Implementação do arquivo graphics_instructions.h
 */

#include "graphics_instructions.h"


/*******************************************************************************
 * Estruturas
 *******************************************************************************/

/**
 * Estrutura da janela de isntruções
 */
struct graphicInstructions{
    WINDOW* instructionsWindow;
};

/*******************************************************************************
 * Funções privadas
 *******************************************************************************/

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

    graphic->instructionsWindow = newwin(height, width, positionY, positionX);
    if(!graphic->instructionsWindow){
        free(graphic);
        return NULL;
    }

    box(graphic->instructionsWindow,'*','*');
    mvwprintw(graphic->instructionsWindow,0,2,"instructions");
    wrefresh(graphic->instructionsWindow);

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
    box((*graphic)->instructionsWindow, '*', '*');
    mvwprintw((*graphic)->instructionsWindow,0,2,"instructions");
    wrefresh((*graphic)->instructionsWindow);
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
 */
void GRAPHICINST_writeKeyboard(GraphicInstructions** graphic, int positionX,
        int positionY){
    if(!graphic || !(*graphic)) return;

    char keyboardString[] = "LEFT <-   RIGHT ->   UP ^   DOWN v";

    mvwprintw((*graphic)->instructionsWindow, positionY, positionX, "%s", keyboardString);
    wrefresh((*graphic)->instructionsWindow);
}

