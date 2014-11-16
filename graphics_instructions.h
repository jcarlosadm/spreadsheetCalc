/**
 * \file graphics_instructions.h
 * Descreve a janela de instruções da planilha
 */

#ifndef GRAPHICS_INSTRUCTIONS_H_
#define GRAPHICS_INSTRUCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

/**
 * Estrutura da janela de isntruções
 */
typedef struct graphicInstructions GraphicInstructions;

/**
 * Cria a janela de instruções e desenha na tela
 * \return Ponteiro para GraphicInstructions ou NULL
 * \param positionX Posição x da janela
 * \param positionY Posição y da janela
 * \param width Largura da janela
 * \param height Altura da janela
 */
GraphicInstructions* GRAPHICINST_create(int positionX, int positionY,
        int width,int height);

/**
 * Libera memória alocada na janela de instruções
 * \return NULL
 * \param graphic Ponteiro para GraphicInstructions
 */
GraphicInstructions* GRAPHICINST_free(GraphicInstructions* graphic);

/**
 * Limpa tela de instruções
 * \param graphic Ponteiro duplo para GraphicInstructions
 */
void GRAPHICINST_clear(GraphicInstructions** graphic);

/**
 * Escreve na tela de instruções na posição informada
 * \param graphic Ponteiro duplo para GraphicInstructions
 * \param instruction Instrução a ser escrita
 * \param positionX Coordenada x em que aparecerá a instrução
 * \param positionY Coordenada y em que aparecerá a isntrução
 */
void GRAPHICINST_write(GraphicInstructions** graphic, const char *instruction,
        int positionX, int positionY);

/**
 * Escreve instruções do teclado (up, down, left, right) na posição informada
 * \param graphic Ponteiro duplo para GraphicInstructions
 * \param positionX Coordenada x em que aparecerá a instrução
 * \param positionY Coordenada y em que aparecerá a instrução
 */
void GRAPHICINST_writeKeyboard(GraphicInstructions** graphic, int positionX,
        int positionY);

#endif /* GRAPHICS_INSTRUCTIONS_H_ */
