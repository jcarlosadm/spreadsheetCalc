/**
 * \file matrix.c
 * Implementação do arquivo matrix.h
 */

#include "matrix.h"

#define MAX_CELLS 100

/****************************************************************************
 * Estruturas
 ****************************************************************************/

/**
 * Estrutura da lista de dependências de cada célula
 */
typedef struct dependency Dependency;
struct dependency{
    int value;
    Dependency *next;
};

/**
 * Estrutura de cada célula
 */
typedef struct cell Cell;
struct cell{
    Dependency* first;
    char expression[60];
    double value;
};

/**
 * Estrutura do grafo que conterá as células
 */
typedef struct graph Graph;
struct graph{
    Cell* cell[MAX_CELLS];
};

/**
 * Estrutura da matriz de células da planilha
 */
struct matrix{
    int rows;
    int columns;
    Graph graph;
};

/****************************************************************************
 * Funções privadas
 ****************************************************************************/



/****************************************************************************
 * Funções públicas
 ****************************************************************************/
