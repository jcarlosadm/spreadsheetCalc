/**
 * \file save.h
 * Possui operações para salvar so dados em um arquivo xml
 */

#ifndef SAVE_H_
#define SAVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <mxml.h>
#include <string.h>
#include <time.h>
#include "matrix.h"
#include "graphics_instructions.h"
#include "graphics_select.h"
#include "graphics_user.h"

/**
 * Estrutura do arquivo de save
 */
typedef struct saveFile SaveFile;

/**
 * Carrega arquivo de save e aloca memória. Cria arquivo se o mesmo não existir.
 * \return Ponteiro para SaveFile
 * \param fileName Nome do arquivo onde estão os dados
 */
SaveFile* SAVE_create(const char *fileName);

/**
 * Libera memória alocada no objeto SaveFile
 * \return NULL
 * \param save Ponteiro para SaveFile
 */
SaveFile* SAVE_free(SaveFile* save);

/**
 * Inicia sistema de salvar espaços de trabalho em arquivo
 * \param save Ponteiro para o arquivo de salvamento
 * \param window_instructions POnteiro para a janela de intruções
 * \param window_list Ponteiro para a janela que listará espaços salvos até o momento
 * \param window_user Ponteiro para a janela que recebe texto digitado pelo usuário
 * \param window_select Ponteiro para a janela de opções
 */
void SAVE_init(SaveFile** save, GraphicInstructions** window_instructions,
        GraphicInstructions** window_list, GraphicUser** window_user,
        GraphicSelect** window_select);

#endif /* SAVE_H_ */