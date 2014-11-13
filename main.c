/*
 * main.c
 * (temporariamente usada para testes)
 */

#include <stdio.h>
#include "undo_redo_cells.h"

int main()
{
    UndoRedoCells* urc = UNDOREDOCELLS_create();

    urc = UNDOREDOCELLS_free(urc);

    return 0;
}
