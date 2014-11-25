/*
 * \file spreadsheet.c
 * Implementação do arquivo spreadsheet.h
 */

#include "spreadsheet.h"

/*******************************************************************************
 * Funções privadas
 *******************************************************************************/



/*******************************************************************************
 * Funções públicas
 *******************************************************************************/

/**
 * Inicia sistema principal de planilha
 * \param matrix Ponteiro para matriz de células. Informe NULL no caso de
 * novo espaço de trabalho
 * \param workspaceName Nome do espaço de trabalho. Informe NULL no caso de
 * novo espaço de trabalho
 */
void SPREADSHEET_run(Matrix** matrix, const char* workspaceName){

    // variáveis que guardam a linha inicial e coluna inicial
    int currentRow=1, currentColumn=1;

    // Ponteiro para a janela de usuário
    GraphicUser* graphic_user;
    // Ponteiro para a janela de instruções
    GraphicInstructions* graphic_instructions;
    // Ponteiro para a janela de listagem
    GraphicInstructions* graphic_list;
    // Ponteiro para a janela de opções
    GraphicSelect* graphic_select;
    // Ponteiro para a janela de células
    GraphicCells* graphic_cells;

    // Ponteiro para dados de salvamento
    SaveFile* save;

    // Inicializa matriz se não inicializada
    if(!matrix || !(*matrix)){
        (*matrix) = MATRIX_create(ROWS, COLUMNS);
    }
    // caso já tenha sido inicializada

}










