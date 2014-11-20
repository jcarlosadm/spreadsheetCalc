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
    Option *next;
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
    Option* firstOption;
};

/************************************************************************************
 * Funções privadas
 ************************************************************************************/

/**
 * Libera opções
 */
void GRAPHICSSELECT_freeOptions(Option** option){
    if(!option || !(*option)) return;

    Option* current = (*option);
    (*option) = (*option)->next;
    free(current);
    GRAPHICSSELECT_freeOptions(&(*option));
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

    graphic->firstOption = NULL;
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

    GRAPHICSSELECT_freeOptions(&(graphic->firstOption));
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
    if(((*graphic)->height - ((*graphic)->totalOptions * OPTIONHEIGHT)) < OPTIONHEIGHT)
        return 0;

    if((*graphic)->firstOption){
        Option* current = (*graphic)->firstOption;
        while(current->next)
            current = current->next;
        current->next = malloc(sizeof(Option));
        if(!(current->next)) return 0;

        current->next->next = NULL;
        strcpy(current->next->name, optionName);
    }
    else{
        (*graphic)->firstOption = malloc(sizeof(Option));
        if(!(*graphic)->firstOption) return 0;

        (*graphic)->firstOption->next = NULL;
        strcpy((*graphic)->firstOption->name, optionName);
    }

    ((*graphic)->totalOptions)++;

    return 1;
}

/**
 * Remove uma opção com maior índice do menu de opções.
 * \param graphic Ponteiro duplo para GraphicSelect
 */
void GRAPHICSSELECT_removeOption(GraphicSelect** graphic){
    if(!graphic || !(*graphic) || !(*graphic)->firstOption) return;

    if(!(*graphic)->firstOption->next){
        free((*graphic)->firstOption);
        (*graphic)->firstOption = NULL;
        ((*graphic)->totalOptions)--;
        return;
    }

    Option* current = (*graphic)->firstOption;

    while(current->next->next)
        current = current->next;

    free(current->next);
    current->next = NULL;
    ((*graphic)->totalOptions)--;
}

/**
 * Apaga todas as opções do menu de opções
 * \param graphic Ponteiro duplo para GraphicSelect
 */
void GRAPHICSSELECT_clearOptions(GraphicSelect** graphic){
    if(!graphic || !(*graphic) || !(*graphic)->firstOption) return;

    GRAPHICSSELECT_freeOptions(&((*graphic)->firstOption));
    (*graphic)->firstOption = NULL;
    (*graphic)->totalOptions = 0;
}
