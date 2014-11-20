/**
 * \file graphics_select.c
 * Implementação do arquivo graphics_select.h
 */

#include "graphics_select.h"

// borda lateral e superior e inferior da janela
#define WINDOWBORDERLATERAL '|'
#define WINDOWBORDERUPDOWN '-'
// nome da janela
#define WINDOWNAME "options"

// altura de uma opção
#define OPTIONHEIGHT 3
// borda lateral e superior e inferior de uma opção selecionada
#define OPTIONBORDERLATERAL '*'
#define OPTIONBORDERUPDOWN '*'


/************************************************************************************
 * Estruturas
 ************************************************************************************/

/**
 * Estrutura de uma opção
 */
typedef struct option Option;
struct option{
    char name[30];
    WINDOW* window;
    Option *next;
    Option *previous;
};

/**
 * Estrutura de um menu de seleção
 */
struct graphicSelect{
    int positionX;
    int positionY;
    int width;
    int height;

    WINDOW* window;

    int totalOptions;
    Option* sentinel;
};

/************************************************************************************
 * Funções privadas
 ************************************************************************************/

/**
 * Libera opções
 */
void GRAPHICSSELECT_freeOptions(Option** option, Option** sentinel){
    if(!option || !(*option) || ((*option)==(*sentinel))) return;

    Option* current = (*option);
    (*option) = (*option)->next;
    free(current);
    GRAPHICSSELECT_freeOptions(&(*option),&(*sentinel));
}

/**
 * Desenha caixa em volta da janela
 */
void GRAPHICSSELECT_drawBoxWindow(GraphicSelect** graphic){
    if(!graphic || !(*graphic)) return;

    box((*graphic)->window, WINDOWBORDERLATERAL,WINDOWBORDERUPDOWN);
    mvwprintw((*graphic)->window,0,1,WINDOWNAME);
    wrefresh((*graphic)->window);
}

/************************************************************************************
 * Funções públicas
 ************************************************************************************/

/**
 * Cria uma janela de seleção. Desenha o espaço da janela.
 * \return Ponteiro para a memória alocada, ou NULL em caso de falha de alocação
 * \param positionX Posição x da janela
 * \param positionY Posição y da janela
 * \param width Largura da janela (que será a largura de cada opção)
 * \param height Altura da janela (não de cada opção; as opções estarão contidas na janela).
 * Esse parâmetro determina a quantidade máxima de opções que a janela poderá ter (1 opção
 * para cada 3 unidades de altura).
 */
GraphicSelect* GRAPHICSSELECT_create(int positionX, int positionY, int width, int height){
    GraphicSelect* graphic = malloc(sizeof(GraphicSelect));
    if(!graphic) return NULL;

    graphic->window = newwin(height, width, positionY, positionX);
    if(!graphic->window){
        free(graphic);
        return NULL;
    }

    graphic->sentinel = malloc(sizeof(Option));
    if(!graphic->sentinel){
        delwin(graphic->window);
        free(graphic);
        return NULL;
    }

    graphic->sentinel->next = graphic->sentinel;
    graphic->sentinel->previous = graphic->sentinel;

    graphic->positionX = positionX;
    graphic->positionY = positionY;
    graphic->width = width;
    graphic->height = height;
    graphic->totalOptions = 0;

    GRAPHICSSELECT_drawBoxWindow(&graphic);

    return graphic;
}

/**
 * Libera memória alocada no objeto GraphicSelect
 * \return NULL
 * \param graphic Ponteiro para GraphicSelect
 */
GraphicSelect* GRAPHICSSELECT_free(GraphicSelect* graphic){
    if(!graphic) return NULL;

    GRAPHICSSELECT_freeOptions(&(graphic->sentinel->next),&(graphic->sentinel));
    free(graphic->sentinel);

    delwin(graphic->window);
    free(graphic);

    return NULL;
}

/**
 * Adiciona uma opção no menu de opções. As opções aparecem na ordem de adição
 * \return 1 em caso de sucesso, ou 0 em caso de falha
 * \param graphic Ponteiro duplo para GraphicSelect
 * \param optionName Nome da opção a ser adicionada (no máximo até 30 caracteres)
 */
int GRAPHICSSELECT_addOption(GraphicSelect** graphic, const char* optionName){
    if(!graphic || !(*graphic)) return 0;

    // não há altura suficiente para adicionar mais uma opção
    if(((*graphic)->height - 2 - ((*graphic)->totalOptions * OPTIONHEIGHT)) < OPTIONHEIGHT)
        return 0;

    Option* oldLast = (*graphic)->sentinel->previous;

    oldLast->next = malloc(sizeof(Option));
    if(!(oldLast->next)) {
        oldLast->next = (*graphic)->sentinel;
        return 0;
    }

    oldLast->next->next = (*graphic)->sentinel;
    oldLast->next->previous = oldLast;
    strcpy(oldLast->next->name, optionName);
    oldLast->next->window = NULL;

    (*graphic)->sentinel->previous = oldLast->next;

    ((*graphic)->totalOptions)++;

    return 1;
}

/**
 * Remove uma opção com maior índice do menu de opções.
 * \param graphic Ponteiro duplo para GraphicSelect
 */
void GRAPHICSSELECT_removeOption(GraphicSelect** graphic){
    if(!graphic || !(*graphic) || ((*graphic)->sentinel->previous == (*graphic)->sentinel)) return;

    Option* oldLast = (*graphic)->sentinel->previous;

    oldLast->previous->next = oldLast->next;
    oldLast->next->previous = oldLast->previous;

    free(oldLast);
    ((*graphic)->totalOptions)--;
}

/**
 * Apaga todas as opções do menu de opções
 * \param graphic Ponteiro duplo para GraphicSelect
 */
void GRAPHICSSELECT_clearOptions(GraphicSelect** graphic){
    if(!graphic || !(*graphic) || ((*graphic)->sentinel->next == (*graphic)->sentinel)) return;

    GRAPHICSSELECT_freeOptions(&(*graphic)->sentinel->next, &(*graphic)->sentinel);
    (*graphic)->sentinel->next = (*graphic)->sentinel;
    (*graphic)->sentinel->previous = (*graphic)->sentinel;

    (*graphic)->totalOptions = 0;
}

/**
 * Abre opções e o usuário poderá escolher com ENTER.
 * \param graphic Ponteiro duplo para GraphicSelect
 * \param optionName Variável a ser preenchida com o nome da opção escolhida
 */
void GRAPHICSSELECT_selectOption(GraphicSelect** graphic, char *optionName){
    if(!graphic || !(*graphic) || ((*graphic)->sentinel->next == (*graphic)->sentinel)) return;

    Option* currentOption = (*graphic)->sentinel->next;

    // posição x e y da subjanela de cada opção
    int optionPosX=(*graphic)->positionX+1, optionPosY=(*graphic)->positionY +1;
    // largura de cada opção
    int optionWidth = (*graphic)->width - 2;

    // cria as subjanelas de opções
    while(currentOption!=(*graphic)->sentinel){
        currentOption->window = newwin(OPTIONHEIGHT,optionWidth,optionPosY,optionPosX);
        mvwprintw(currentOption->window,1,1,currentOption->name);

        if(currentOption->previous == (*graphic)->sentinel)
            box(currentOption->window,OPTIONBORDERLATERAL,OPTIONBORDERUPDOWN);

        wrefresh(currentOption->window);
        optionPosY+=OPTIONHEIGHT;

        currentOption = currentOption->next;
    }

    // habilita teclado
    keypad(stdscr,TRUE);
    cbreak();

    int keyPressed=0;

    while(keyPressed!=10){
        // recebe pressionamento de tecla
        keyPressed = getch();

        if(keyPressed!=10){
            box(currentOption->window,' ',' ');
            wrefresh(currentOption->window);
        }

        switch(keyPressed){
        case KEY_UP:
            if(currentOption->previous == (*graphic)->sentinel)
                currentOption = (*graphic)->sentinel->previous;
            else
                currentOption = currentOption->previous;
            break;
        case KEY_DOWN:
            if(currentOption->next == (*graphic)->sentinel)
                currentOption = (*graphic)->sentinel->next;
            else
                currentOption = currentOption->next;
            break;
        }

        if(keyPressed!=10){
            box(currentOption->window,OPTIONBORDERLATERAL,OPTIONBORDERUPDOWN);
            wrefresh(currentOption->window);
        }
    }

    strcpy(optionName,currentOption->name);

    // libera subjanelas de opções
    currentOption = (*graphic)->sentinel->next;
    while(currentOption != (*graphic)->sentinel){
        delwin(currentOption->window);
        currentOption->window = NULL;

        currentOption = currentOption->next;
    }

    // atualiza janela de opções
    GRAPHICSSELECT_drawBoxWindow(&(*graphic));
}
