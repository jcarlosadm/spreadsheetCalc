/**
 * \file mainMenu.h
 * Descreve o menu principal
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdbool.h>
#include "spreadsheet.h"
#include "graphics_instructions.h"
#include "graphics_select.h"
#include "load.h"

/**
 * Executa o menu principal
 */
void MAINMENU_run();

#endif /* MAINMENU_H_ */
