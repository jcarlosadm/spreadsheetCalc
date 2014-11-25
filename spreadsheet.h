/**
 * \file spreadsheet.h
 * Define sistema principal da planilha eletrônica
 */

#ifndef SPREADSHEET_H_
#define SPREADSHEET_H_

#include <stdio.h>
#include "matrix.h"
#include "undo_redo_cells.h"
#include "save.h"
#include "graphics_cells.h"
#include "graphics_instructions.h"
#include "graphics_select.h"
#include "graphics_user.h"

/**
 * Inicia sistema principal de planilha
 * \param matrix Ponteiro para matriz de células. Informe NULL no caso de
 * novo espaço de trabalho
 * \param workspaceName Nome do espaço de trabalho. Informe NULL no caso de
 * novo espaço de trabalho
 */
void SPREADSHEET_run(Matrix** matrix, const char* workspaceName);

#endif /* SPREADSHEET_H_ */
