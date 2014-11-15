/**
 * \file matrix.c
 * Implementação do arquivo matrix.h
 */

#include "matrix.h"

#define MAX_CELLS 100

#define MIN_ASCII_NUMBER 48
#define MAX_ASCII_NUMBER 57
#define MIN_ASCII_CAP_LETTER 65
#define MAX_ASCII_CAP_LETTER 90
#define MIN_ASCII_LETTER 97
#define MAX_ASCII_LETTER 122

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
    Cell* cells[MAX_CELLS];
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

/**
 * Desaloca células do graph
 */
void MATRIX_freeGraphCells(Cell** cells, int index){
    if(index >= MAX_CELLS) return;

    if(cells[index]){
        Cell* cell = cells[index];
        if(cell->first){
            Dependency* current = cell->first;
            Dependency* previous = NULL;
            while(current){
                previous = current;
                current = current->next;
                free(previous);
            }
            cell->first = NULL;
        }
        free(cell);
        cells[index] = NULL;
    }
    MATRIX_freeGraphCells(cells, index+1);
}

/**
 * Calcula índice da célula
 */
int MATRIX_evalCellIndex(int row, int column, int max_columns){
    return ((column-1)+(row-1)*(max_columns));
}

/**
 * Obtém o índice de uma célula com base em sua string ('A2' por exemplo)
 */
int MATRIX_getCellIndex_fromReference(const char *expression, int *count, int columns){

    int letter, coords[2];

    // pega valor ascii do caractere e converte para um valor de coluna
    letter = expression[(*count)];
    if(MIN_ASCII_CAP_LETTER<= letter && letter <= MAX_ASCII_CAP_LETTER)
        coords[1] = letter - MIN_ASCII_CAP_LETTER+1;
    else
        coords[1] = letter - MIN_ASCII_LETTER+1;
    // move para o próximo caractere e pega valor da linha
    (*count)++;
    coords[0] = expression[(*count)] - MIN_ASCII_NUMBER;

    return MATRIX_evalCellIndex(coords[0],coords[1], columns);
}

/**
 * Remove uma dependência da célula
 */
void MATRIX_removeDependency(Cell** cell, int value){
    if(!cell || !(*cell) || !(*cell)->first) return;

    Dependency* current = (*cell)->first;
    if(current->value == value){
        (*cell)->first = current->next;
        free(current);

        if((*cell)->first == NULL && strcmp((*cell)->expression, "")==0){
            free(*cell);
            (*cell) = NULL;
        }

        return;
    }

    while(current->next && current->next->value != value)
        current = current->next;

    if(current->next){
        Dependency* remove = current->next;
        current->next = current->next->next;
        free(remove);
    }

}

/**
 * adiciona uma dependência da célula
 */
void MATRIX_addDependency(Cell** cell, int value){

    if(!(*cell)){
        (*cell) = malloc(sizeof(Cell));
        if(!(*cell)) return;

        (*cell)->first = NULL;
        strcpy((*cell)->expression, "");
    }

    if(!(*cell)->first){
        (*cell)->first = malloc(sizeof(Dependency));
        if(!(*cell)->first) return;

        (*cell)->first->next = NULL;
        (*cell)->first->value = value;
        return;
    }

    Dependency* current = (*cell)->first;

    if(current->value == value) return;

    while(current->next && current->next->value != value)
        current = current->next;

    if(current->next) return;

    current->next = malloc(sizeof(Dependency));
    if(!current->next) return;

    current->next->next = NULL;
    current->next->value = value;

}

/**
 * Remove ou adiciona todas as dependências em relação a uma célula específica,
 * com base na expressão
 * \param isRemove Se true, deverá remover a dependência. Caso contrário, adiciona
 */
void MATRIX_modDependencies(Matrix** matrix, int cellIndex, const char* expression,
        int isRemove){
    // retira dependências em relação à célula atual, com base na antiga expressão
    int count=0, cellDestiny;
    while(expression[count]!=0){

        // faz comparações com base na table ascii
        // um parênteses e o próximo caractere não número?
        if(expression[count]=='(' && !(MIN_ASCII_NUMBER<= expression[count+1] &&
                expression[count+1]<=MAX_ASCII_NUMBER)){
            // move para o próximo caractere
            count++;

            cellDestiny = MATRIX_getCellIndex_fromReference(expression, &count,
                    (*matrix)->columns);

            if(isRemove)
                MATRIX_removeDependency(&((*matrix)->graph.cells[cellDestiny]), cellIndex);
            else
                MATRIX_addDependency(&((*matrix)->graph.cells[cellDestiny]), cellIndex);

            count++;
        }
        else if(expression[count]=='+' || expression[count]=='-'
                || expression[count]=='*' || expression[count]=='/'){
            count++;
        }
        else{
            while(expression[count]!=0 && expression[count]!=')')
                count++;
        }
        count++;
    }
}

/**
 * Computa o valor da célula
 */
void MATRIX_evalCellValue(Matrix ** matrix, int cellIndex){
    if(!matrix || !(*matrix)) return;

    // copia expressão para uma variável
    char expression[60];
    strcpy(expression, (*matrix)->graph.cells[cellIndex]->expression);

    // guarda nome de função que possa existir na expressão
    char function[10];
    // guarda valor numérico que possa existir na expressão
    char number_char[50];

    // guarda índice temporário
    int cellTempIndex;

    // Para o módulo de funções
    ListDouble* list;

    // para o módulo stack_binExpTree
    StackBinExpTree* stackBin = STACKBINEXPTREE_create();

    // percorre expressão
    int count=0, check;
    while(expression[count] != 0){
        // número ou referência para uma célula
        if(expression[count]=='('){
            count++;
            if(!(MIN_ASCII_NUMBER<= expression[count] &&
                    expression[count]<=MAX_ASCII_NUMBER)){
                cellTempIndex = MATRIX_getCellIndex_fromReference(expression, &count,
                        (*matrix)->columns);

                if(!(*matrix)->graph.cells[cellTempIndex])
                    STACKBINEXPTREE_pushValue(&stackBin, 0);
                else
                    STACKBINEXPTREE_pushValue(&stackBin,
                            (*matrix)->graph.cells[cellTempIndex]->value);
                count++;
            }
            else{
                check = count;
                while(expression[check]!=')'){
                    number_char[check-count]=expression[check];
                    check++;
                }
                number_char[check-count]=0;

                STACKBINEXPTREE_pushValue(&stackBin, atof(number_char));

                count = check;
            }
        }
        // operador
        else if(expression[count]=='+' || expression[count]=='-'
                || expression[count]=='*' || expression[count]=='/'){
            STACKBINEXPTREE_pushSymbol(&stackBin, expression[count]);
        }
        // função
        else{
            check=count;
            while(expression[check]!='('){
                function[check-count] = expression[check];
                check++;
            }

            function[check-count]=0;

            count=check+1;
            list = FUNCTIONS_createList();

            while(1){

                if(expression[count]==')' && expression[count-1]==')')
                    break;

                if(expression[count]==')' || expression[count]=='('
                        || expression[count]==','){
                    count++;

                    continue;
                }
                if(!(MIN_ASCII_NUMBER<= expression[count] &&
                        expression[count]<=MAX_ASCII_NUMBER)){
                    cellTempIndex = MATRIX_getCellIndex_fromReference(expression, &count,
                            (*matrix)->columns);
                    if(!(*matrix)->graph.cells[cellTempIndex])
                        list = FUNCTIONS_addValue(list, 0);
                    else
                        list = FUNCTIONS_addValue(list,
                                (*matrix)->graph.cells[cellTempIndex]->value);
                    count++;

                }
                else{
                    check = count;
                    while(expression[check]!=',' && expression[check]!=')'){
                        number_char[check-count]=expression[check];
                        check++;
                    }
                    number_char[check-count]=0;
                    list = FUNCTIONS_addValue(list, atof(number_char));
                    if(expression[check]!=')')
                        count = check;
                    else
                        count = check-1;
                }
                count++;
            }
            STACKBINEXPTREE_pushValue(&stackBin, FUNCTIONS_evalFunction(function, &list));
            list = FUNCTIONS_free(list);
        }

        count++;
    }

    (*matrix)->graph.cells[cellIndex]->value = STACKBINEXPTREE_pop(&stackBin);

    stackBin = STACKBINEXPTREE_free(stackBin);
}

void MATRIX_evalCellDepsValue(Matrix** matrix, int cellIndex, int originalCell){

    Dependency* dep = (*matrix)->graph.cells[cellIndex]->first;

    while(dep){
        if(dep->value != originalCell){
            MATRIX_evalCellValue(&(*matrix), dep->value);
            MATRIX_evalCellDepsValue(&(*matrix), dep->value, originalCell);
        }
        dep = dep->next;
    }
}

/****************************************************************************
 * Funções públicas
 ****************************************************************************/

/**
 * Cria uma matriz com a quantidade de linhas e colunas especificadas
 * \return Ponteiro para a matriz criada
 * \param rows Quantidade de linhas da matriz
 * \param columns Quantidade de colunas da matriz
 */
Matrix* MATRIX_create(int rows, int columns){
    Matrix* matrix = malloc(sizeof(Matrix));
    if(!matrix) return NULL;

    matrix->rows = rows;
    matrix->columns = columns;

    int count;
    for(count=0; count<MAX_CELLS; count++)
        matrix->graph.cells[count] = NULL;

    return matrix;
}

/**
 * Libera memória alocada na matriz
 * \return NULL
 * \param matrix Ponteiro para matriz Matrix
 */
Matrix* MATRIX_free(Matrix* matrix){
    if(!matrix) return NULL;

    MATRIX_freeGraphCells(matrix->graph.cells, 0);
    free(matrix);
    matrix = NULL;

    return matrix;
}

/**
 * Obtém expressão de uma célula específica da matriz
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param row Linha da célula
 * \param column  Coluna da célula
 * \param expression String a ser preenchida com a expressão da célula
 */
void MATRIX_getExpression(Matrix** matrix, int row, int column, char *expression){
    if(!matrix || !(*matrix)){
        strcpy(expression,"");
        return;
    }

    int cellIndex = MATRIX_evalCellIndex(row, column, (*matrix)->columns);

    if((*matrix)->graph.cells[cellIndex])
        strcpy(expression, (*matrix)->graph.cells[cellIndex]->expression);
    else
        strcpy(expression, "");
}

/**
 * Obtém valor da célula
 * \return Valor da célula
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param row Linha da célula
 * \param column Coluna da célula
 */
double MATRIX_getValue(Matrix** matrix, int row, int column){
    if(!matrix || !(*matrix)) return 0;

    int cellIndex = MATRIX_evalCellIndex(row, column, (*matrix)->columns);

    if((*matrix)->graph.cells[cellIndex])
        return (*matrix)->graph.cells[cellIndex]->value;
    else
        return 0;
}

/**
 * Define uma expressão para uma célula específica, calculando o seu valor no processo
 * \return 1 se obtiver sucesso, e 0 caso contrário
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param row Linha da célula
 * \param column Coluna da célula
 * \param expression expressão a ser colocada na célula
 * \param undoRedo Ponteiro duplo para fila de desfazer/refazer. Informe NULL caso
 * não queira guarda a informação na fila de desfazer/refazer
 */
int MATRIX_setExpression(Matrix** matrix, int row, int column, const char* expression,
        UndoRedoCells** undoRedo){
    if(!matrix || !(*matrix)) return 0;

    int cellIndex = MATRIX_evalCellIndex(row, column, (*matrix)->columns);

    // ponteiro para a célula de interesse
    Cell* cell;
    if((*matrix)->graph.cells[cellIndex])
        cell = (*matrix)->graph.cells[cellIndex];
    else{
        cell = malloc(sizeof(Cell));
        if(!cell) return 0;

        cell->first = NULL;
        strcpy(cell->expression, "");

        (*matrix)->graph.cells[cellIndex] = cell;
    }

    char oldExpression[60];

    // guarda expressão atual (que será anterior) da célula
    strcpy(oldExpression, cell->expression);

    // retira dependências em relação à célula atual, com base na antiga expressão
    MATRIX_modDependencies(&(*matrix), cellIndex, oldExpression, true);

    // adiciona dependências em relação à célula atual, com base na nova expressão
    MATRIX_modDependencies(&(*matrix), cellIndex, expression, false);

    // se undoRedo não nulo, adiciona na fila de desfazer/refazer
    if(undoRedo && (*undoRedo)){
        UNDOREDOCELLS_newItem(&(*undoRedo), oldExpression, expression, cellIndex);
    }

    // guarda nova expressão
    strcpy((*matrix)->graph.cells[cellIndex]->expression, expression);

    // se a célula possui expressão vazia e nenhuma outra depende dela,
    // desaloca e sai
    if(strcmp((*matrix)->graph.cells[cellIndex]->expression, "")==0
            && !(*matrix)->graph.cells[cellIndex]->first){
        free((*matrix)->graph.cells[cellIndex]);
        (*matrix)->graph.cells[cellIndex] = NULL;
        return 1;
    }

    // computa o valor da célula
    MATRIX_evalCellValue(&(*matrix), cellIndex);

    // percorre todas as dependências para atualizar todas as células que dependem desta
    MATRIX_evalCellDepsValue(&(*matrix), cellIndex, cellIndex);

    return 1;

}






