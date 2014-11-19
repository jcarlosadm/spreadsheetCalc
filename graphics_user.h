/**
 * \file graphics_user.h
 * Descreve a janela de usuário
 */

#ifndef GRAPHICS_USER_H_
#define GRAPHICS_USER_H_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

/**
 * Estrutura da janela de usuário
 */
typedef struct graphicUser GraphicUser;

/**
 * Aloca memória para a janela de usuário e desenha na tela
 * \return Ponteiro para a memória alocada ou NULL (em caso de falha de alocação)
 * \param positionX Coordenada x da janela
 * \param positionY Coordenada y da janela
 * \param width Largura da janela
 * \param height Altura da janela
 */
GraphicUser* GRAPHICUSER_create(int positionX, int positionY, int width, int height);

/**
 * Desaloca memória de graphic
 * \return NULL
 * \param graphic Ponteiro para GraphicUSer (a janela de usuário)
 */
GraphicUser* GRAPHICUSER_free(GraphicUser* graphic);

/**
 * Limpa janela de usuário
 * \param graphic Ponteiro duplo para GraphicUSer (a janela de usuário)
 */
void GRAPHICUSER_clear(GraphicUser** graphic);

/**
 * Escreve algo na janela de usuário, numa posição específica
 * \param graphic Ponteiro duplo para GraphicUSer (a janela de usuário)
 * \param text Texto a ser escrito na janela de usuário
 * \param positionX Coordenada x em que será inserido o texto na janela
 * \param positionY Coordenada y em que será inserido o texto na janela
 */
void GRAPHICUSER_write(GraphicUser** graphic, const char *text, int positionX, int positionY);

/**
 * Recebe entrada do usuário, guardando o resultado em text
 * \param graphic Ponteiro duplo para GraphicUSer (a janela de usuário)
 * \param text Variável a ser preenchida com a entrada do usuário (máximo 60 caracteres)
 * \param positionX Coordenada x da entrada do usuário
 * \param positionY Coordenada y da entrada do usuário
 */
void GRAPHICUSER_get(GraphicUser** graphic, char* text, int positionX, int positionY);

#endif /* GRAPHICS_USER_H_ */
