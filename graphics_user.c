/*
 * \file graphics_user.c
 * Implementação de graphics_user.h
 */

#include "graphics_user.h"

// define o nome da janela
#define WINDOWNAME "user"
// define a posição x do nome da janela
#define WINDOWNAMEX 2
// define a posição y do nome da janela
#define WINDOWNAMEY 0

// define a borda lateral da janela
#define WINDOWBORDERLATERAL '*'
// define a borda superior e inferior da janela
#define WINDOWBORDERUPDOWN '*'

/**********************************************************************************
 * Estruturas
 **********************************************************************************/

/**
 * Estrutura da janela de usuário
 */
struct graphicUser{
    WINDOW* userWindow;
};

/**********************************************************************************
 * Funções privadas
 **********************************************************************************/

/**
 * Desenha borda em volta da janela
 * \param graphic Ponteiro duplo para GraphicUser
 */
void GRAPHICUSER_drawBox(GraphicUser** graphic){
    box((*graphic)->userWindow,WINDOWBORDERLATERAL,WINDOWBORDERUPDOWN);
    mvwprintw((*graphic)->userWindow,WINDOWNAMEY,WINDOWNAMEX,WINDOWNAME);
    wrefresh((*graphic)->userWindow);
}

/**********************************************************************************
 * Funções públicas
 **********************************************************************************/

/**
 * Aloca memória para a janela de usuário e desenha na tela
 * \return Ponteiro para a memória alocada ou NULL (em caso de falha de alocação)
 * \param positionX Coordenada x da janela
 * \param positionY Coordenada y da janela
 * \param width Largura da janela
 * \param height Altura da janela
 */
GraphicUser* GRAPHICUSER_create(int positionX, int positionY, int width, int height){
    GraphicUser* graphic = malloc(sizeof(GraphicUser));
    if(!graphic) return NULL;

    graphic->userWindow = newwin(height, width, positionY, positionX);
    if(!graphic->userWindow){
        free(graphic);
        return NULL;
    }

    GRAPHICUSER_drawBox(&graphic);

    return graphic;
}

/**
 * Desaloca memória de graphic
 * \return NULL
 * \param graphic Ponteiro para GraphicUSer (a janela de usuário)
 */
GraphicUser* GRAPHICUSER_free(GraphicUser* graphic){
    if(!graphic) return graphic;

    wclear(graphic->userWindow);
    wrefresh(graphic->userWindow);
    delwin(graphic->userWindow);

    free(graphic);
    return NULL;
}

/**
 * Limpa janela de usuário
 * \param graphic Ponteiro duplo para GraphicUSer (a janela de usuário)
 */
void GRAPHICUSER_clear(GraphicUser** graphic){
    if(!graphic || !(*graphic)) return;

    wclear((*graphic)->userWindow);
    GRAPHICUSER_drawBox(&(*graphic));
}

/**
 * Escreve algo na janela de usuário, numa posição específica
 * \param graphic Ponteiro duplo para GraphicUSer (a janela de usuário)
 * \param text Texto a ser escrito na janela de usuário
 * \param positionX Coordenada x em que será inserido o texto na janela
 * \param positionY Coordenada y em que será inserido o texto na janela
 */
void GRAPHICUSER_write(GraphicUser** graphic, const char *text, int positionX, int positionY){
    if(!graphic || !(*graphic)) return;

    mvwprintw((*graphic)->userWindow, positionY, positionX, "%s", text);
    wrefresh((*graphic)->userWindow);
}

/**
 * Recebe entrada do usuário, guardando o resultado em text
 * \param graphic Ponteiro duplo para GraphicUSer (a janela de usuário)
 * \param text Variável a ser preenchida com a entrada do usuário (máximo 60 caracteres)
 * \param positionX Coordenada x da entrada do usuário
 * \param positionY Coordenada y da entrada do usuário
 */
void GRAPHICUSER_get(GraphicUser** graphic, char* text, int positionX, int positionY){
    if(!graphic || !(*graphic)) return;

    mvwgetnstr((*graphic)->userWindow,positionY, positionX, text, 60);
}
