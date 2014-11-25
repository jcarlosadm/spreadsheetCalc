/**
 * \file load.h
 * Módulo responsável por carregar dados de arquivo xml
 */

#ifndef LOAD_H_
#define LOAD_H_

#include <stdio.h>
#include <string.h>
#include <mxml.h>
#include <stdbool.h>
#include "matrix.h"
#include "graphics_instructions.h"
#include "graphics_select.h"

#ifndef SAVEFILE
/**
 * Nome padrão para o arquivo de salvamento
 */
#define SAVEFILE "save.xml"
#endif // SAVEFILE

/**
 * Verifica se existem dados para carregar
 * \return 0 se não existem dados, e diferente de 0 se existem
 * \param fileName Nome do arquivo
 */
int LOAD_canLoad(const char *fileName);

/**
 * Carrega dados do arquivo para a matriz
 * \return 1 se obtiver sucesso em carregar dados, 0 em caso contrário
 * \param matrix Ponteiro para a matriz de células
 * \param instructions Ponteiro para a janela de instruções
 * \param select Ponteiro para a janela de seleção
 * \param fileName Nome do arquivo
 * \param workspaceName Variável que será preenchida com o nome do espaço de trabalho
 * escolhido
 */
int LOAD_load(Matrix** matrix, GraphicInstructions** instructions,
        GraphicSelect** select, const char* fileName, char* workspace);

#endif /* LOAD_H_ */
