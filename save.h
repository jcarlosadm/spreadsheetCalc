/**
 * \file save.h
 * Possui operações para salvar dados em um arquivo xml
 */

#ifndef SAVE_H_
#define SAVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <mxml.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
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
 * O nome do espaço de trabalho atual é definido como uma string vazia ""
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
 * Define nome do espaço de trabalho ou abre interface com o usuário para definir
 * \param save Ponteiro para SaveFile
 * \param window_instructions Ponteiro para a janela de intruções. Pode informar
 * NULL caso workspaceName seja diferente de NULL
 * \param window_list Ponteiro para a janela que listará espaços salvos até o momento.
 * Pode informar NULL caso workspaceName seja diferente de NULL
 * \param window_user Ponteiro para a janela que recebe texto digitado pelo usuário.
 * Pode informar NULL caso workspaceName seja diferente de NULL
 * \param window_select Ponteiro para a janela de opções. Pode informar
 * NULL caso workspaceName seja diferente de NULL
 * \param workspaceName Nome do espaço de trabalho a ser salvo. Informe NULL caso
 * o usuário precise definir esse nome
 */
void SAVE_defineWorkspace(SaveFile** save, GraphicInstructions** window_instructions,
        GraphicInstructions** window_list, GraphicUser** window_user,
        GraphicSelect** window_select, const char* workspaceName);

/**
 * Verifica se o espaço de trabalho ainda não foi definido
 * \return Um valor diferente de 0 caso ainda não tenha sido definido. 0 caso contrário
 * \param save Ponteiro para SaveFile
 */
int SAVE_workspaceIsNULL(SaveFile** save);

/**
 * Salva dados no espaço de trabalho atual
 * \return 1 se obtiver sucesso e 0 em caso contrário
 * \param save Ponteiro para o arquivo de salvamento
 * \param window_instructions Ponteiro para a janela de intruções
 * \param window_select Ponteiro para a janela de opções
 * \param matrix Ponteiro para a matriz de células
 */
int SAVE_init(SaveFile** save, GraphicInstructions** window_instructions,
        GraphicSelect** window_select, Matrix** matrix);

#endif /* SAVE_H_ */
