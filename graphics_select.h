/**
 * \file graphics_select.h
 * Arquivo que cria um menu de seleção
 */

#ifndef GRAPHICS_SELECT_H_
#define GRAPHICS_SELECT_H_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

/**
 * Estrutura de um menu de seleção
 */
typedef struct graphicSelect GraphicSelect;

/**
 * Cria uma janela de seleção. Desenha o espaço da janela.
 * \return Ponteiro para a memória alocada, ou NULL em caso de falha de alocação
 * \param positionX Posição x da janela
 * \param positionY Posição y da janela
 * \param width Largura da janela (que será a largura de cada opção)
 * \param height Altura da janela (não de cada opção; as opções estarão contidas na janela).
 */
GraphicSelect* GRAPHICSSELECT_create(int positionX, int positionY, int width, int height);

/**
 * Libera memória alocada no objeto GraphicSelect
 * \return NULL
 * \param graphic Ponteiro para GraphicSelect
 */
GraphicSelect* GRAPHICSSELECT_free(GraphicSelect* graphic);

/**
 * Adiciona uma opção no menu de opções. As opções aparecem na ordem de adição
 * \return 1 em caso de sucesso, ou 0 em caso de falha
 * \param graphic Ponteiro duplo para GraphicSelect
 * \param optionName Nome da opção a ser adicionada (no máximo até 30 caracteres)
 */
int GRAPHICSSELECT_addOption(GraphicSelect** graphic, const char* optionName);

/**
 * Remove uma opção com maior índice do menu de opções.
 * \param graphic Ponteiro duplo para GraphicSelect
 */
void GRAPHICSSELECT_removeOption(GraphicSelect** graphic);

/**
 * Apaga todas as opções do menu de opções
 * \param graphic Ponteiro duplo para GraphicSelect
 */
void GRAPHICSSELECT_clearOptions(GraphicSelect** graphic);

/**
 * Abre opções e o usuário poderá escolher com ENTER.
 * \param graphic Ponteiro duplo para GraphicSelect
 * \param optionName Variável a ser preenchida com o nome da opção escolhida
 */
void GRAPHICSSELECT_selectOption(GraphicSelect** graphic, char *optionName);

/**
 * Pega posição x da janela
 * \return Posição x da janela
 * \param graphic Ponteiro para GraphicSelect
 */
int GRAPHICSSELECT_getPositionX(GraphicSelect** graphic);

/**
 * Pega posição y da janela
 * \return Posição y da janela
 * \param graphic Ponteiro para GraphicSelect
 */
int GRAPHICSSELECT_getPositionY(GraphicSelect** graphic);

/**
 * Pega largura da janela
 * \return Largura da janela
 * \param graphic Ponteiro para GraphicSelect
 */
int GRAPHICSSELECT_getWidth(GraphicSelect** graphic);

/**
 * Pega altura da janela
 * \return Altura da janela
 * \param graphic Ponteiro para GraphicSelect
 */
int GRAPHICSSELECT_getHeight(GraphicSelect** graphic);

#endif /* GRAPHICS_SELECT_H_ */
