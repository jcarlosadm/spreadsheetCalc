/**
 * \file matrix.c
 * Implementação do arquivo matrix.h
 */

#include "matrix.h"

// a quantidade máxima de células
#define MAX_CELLS 100

// valores da tabela ascii para referência
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
 * Desaloca células do grafo recursivamente
 * \param cells Array de ponteiros para Cell
 * \param index Valor que será incrementado a cada chamada da função
 * (começa em zero)
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
 * Mostra mensagem de erro
 * \param graphic Ponteiro duplo para objeto GraphicInstructions
 * \param errorMessage Mensagem de erro a ser mostrada
 * \param line Linha em que ocorre o erro (informe 0 para que não apareça a linha
 * e nome do arquivo)
 * \param filename Nome do arquivo em que ocorre o erro
 */
void MATRIX_showError(GraphicInstructions** graphic, const char *errorMessage,
        int line, const char *filename){

    char message[60];
    if(graphic && (*graphic)){
        GRAPHICINST_write(&(*graphic), errorMessage, 1,1);
        if(line){
            sprintf(message,"In line %d and file %s",line, filename);
            GRAPHICINST_write(&(*graphic), message, 1,2);
        }
        return;
    }
}

/**
 * Verifica se um caractere é número
 * \return Um valor diferente de 0 se for número, 0 em caso contrário
 * \param value Caractere a ser testado
 */
int MATRIX_charIsNumber(char value){
    return (MIN_ASCII_NUMBER<= value && value<=MAX_ASCII_NUMBER);
}

/**
 * Verifica se um caractere é letra do alfabeto
 * \return 0 se não for letra de alfabeto, 1 se for letra maiúscula e
 * 2 se for letra minúscula
 * \param value Caractere a ser testado
 */
int MATRIX_charIsAlpha(char value){
    if(MIN_ASCII_CAP_LETTER<= value && value<=MAX_ASCII_CAP_LETTER)
        return 1;
    else if(MIN_ASCII_LETTER<= value && value<=MAX_ASCII_LETTER)
        return 2;
    else
        return 0;
}

/**
 * Verifica se um caractere é um operador (+, -, * ou /)
 * \return Um valor diferente de 0 se for um operador, 0 em caso contrário
 * \param value Caractere a ser testado
 */
int MATRIX_charIsOperator(char value){
    return (value=='+' || value=='-' || value=='*' || value=='/');
}

/**
 * Calcula índice da célula no grafo com base na linha e coluna
 * \return Valor do índice da célula no grafo
 * \param row Linha da célula
 * \param column Coluna da célula
 * \param max_columns Total de colunas da matriz
 */
int MATRIX_evalCellIndex(int row, int column, int max_columns){
    return ((column-1)+(row-1)*(max_columns));
}

/**
 * Calcula a linha da célula com base em seu índice no grafo
 * \return Linha da célula
 * \param cellIndex Índice da célula no grafo
 * \param max_columns Total de colunas da matriz
 */
int MATRIX_getRow(int cellIndex, int max_columns){
    return ((cellIndex/max_columns)+1);
}

/**
 * Calcula a coluna da célula com base em seu índice no grafo
 * \return Coluna da célula
 * \param cellIndex Índice da célula no grafo
 * \param max_columns Total de colunas da matriz
 */
int MATRIX_getColumn(int cellIndex, int max_column){
    int row = MATRIX_getRow(cellIndex, max_column);
    return ((cellIndex - max_column*(row-1))+1);
}

/**
 * Obtém o índice no grafo de uma célula com base na string de referência ('A2' por exemplo)
 * \return Índice da célula no grafo
 * \param expression Expressão que contém a referência
 * \param count Ponteiro para inteiro que indica o índice da referência na expressão
 * \param columns Quantidade de colunas da matriz
 */
int MATRIX_getCellIndex_fromReference(const char *expression, int *count, int columns){

    // variáveis para guardar o valor de uma letra, linha e coluna
    int letter, coords[2];

    // pega valor ascii do caractere e converte para um valor de coluna
    letter = expression[(*count)];
    if(MATRIX_charIsAlpha(letter)==1)
        coords[1] = letter - MIN_ASCII_CAP_LETTER+1;
    else
        coords[1] = letter - MIN_ASCII_LETTER+1;
    // move para o próximo caractere e pega valor da linha
    (*count)++;
    coords[0] = expression[(*count)] - MIN_ASCII_NUMBER;

    // retorna o índice da célula no grafo
    return MATRIX_evalCellIndex(coords[0],coords[1], columns);
}

/**
 * Remove uma dependência da célula
 * \param cell Ponteiro duplo para uma célula da matriz
 * \param value Índice da célula no grafo que irá ser removido
 */
void MATRIX_removeDependency(Cell** cell, int value){
    if(!cell || !(*cell) || !(*cell)->first) return;

    Dependency* current = (*cell)->first;
    // se encontrou de primeira, remove a dependência
    if(current->value == value){
        (*cell)->first = current->next;
        free(current);

        // se a célula não contém dependências e nenhuma expressão, libera memória
        if((*cell)->first == NULL && strcmp((*cell)->expression, "")==0){
            free(*cell);
            (*cell) = NULL;
        }
        return;
    }

    // se não encontrou de primeira, procura
    while(current->next && current->next->value != value)
        current = current->next;

    // se encontrou, remove e libera a memória
    if(current->next){
        Dependency* remove = current->next;
        current->next = current->next->next;
        free(remove);
    }

}

/**
 * Adiciona uma dependência da célula
 * \param cell Ponteiro duplo para uma célula da matriz
 * \param value Índice da célula no grafo que irá ser adicionado
 */
void MATRIX_addDependency(Cell** cell, int value){

    // se a célula não estiver alocada, aloca memória
    if(!(*cell)){
        (*cell) = malloc(sizeof(Cell));
        if(!(*cell)) return;

        (*cell)->first = NULL;
        strcpy((*cell)->expression, "");
    }

    // se a célula não tiver dependências, adiciona
    if(!(*cell)->first){
        (*cell)->first = malloc(sizeof(Dependency));
        if(!(*cell)->first) return;

        (*cell)->first->next = NULL;
        (*cell)->first->value = value;
        return;
    }

    Dependency* current = (*cell)->first;

    // se encontrou a dependência, simplesmente sai
    if(current->value == value) return;

    while(current->next && current->next->value != value)
        current = current->next;

    // se encontrou a dependência, simplesmente sai
    if(current->next) return;

    // não encontrou a dependência. adiciona
    current->next = malloc(sizeof(Dependency));
    if(!current->next) return;

    current->next->next = NULL;
    current->next->value = value;

}

/**
 * Extrai dependências de um intervalo, adicionando ou removendo essas dependências
 * \return Valor atualizado de count
 * \param count Contador que percorre expressão
 * \param expression Expressão que contém o intervalo
 * \param isRemove define se adiciona ou remove a dependência encontrada
 * \param cellIndex Índice da célula que será adicionada/removida da lista
 * de dependências da célula de destino
 * \param matrix Ponteiro para a matriz de células
 * \param firstCell Valor da primeira célula do intervalo (já computada)
 */
int MATRIX_extractDepsFromInterval(int count, const char* expression, int isRemove,
        int cellIndex, Matrix** matrix, int firstCell) {

    // última célula do intervalo
    int lastCell;
    // flag que indica qual a célula já computada anteriormente
    int computeFirstCell;

    // percorre linha e coluna
    int countRow, countColumn;

    // pula os dois pontos
    count++;
    // pula espaços em branco
    while (expression[count] == ' ')
        count++;
    // pega segunda referência
    int cellDestiny = MATRIX_getCellIndex_fromReference(expression, &count,
            (*matrix)->columns);
    // verifica qual a referência maior e configura lastCell e firstCell de acordo
    if (firstCell > cellDestiny) {
        lastCell = firstCell;
        firstCell = cellDestiny;
        computeFirstCell = true;
    } else {
        lastCell = cellDestiny;
        computeFirstCell = false;
    }
    // configura variáveis de intervalo
    int firstRow = MATRIX_getRow(firstCell, (*matrix)->columns);
    int lastRow = MATRIX_getRow(lastCell, (*matrix)->columns);
    int firstColumn = MATRIX_getColumn(firstCell, (*matrix)->columns);
    int lastColumn = MATRIX_getColumn(lastCell, (*matrix)->columns);
    // percorre intervalo de células
    for (countRow = firstRow; countRow <= lastRow; countRow++) {
        for (countColumn = firstColumn; countColumn <= lastColumn; countColumn++) {
            // não computa primeira ou última célula
            if ((!computeFirstCell
                    && (countColumn != firstColumn || countRow != firstRow))
                    || (computeFirstCell
                            && (countColumn != lastColumn
                                    || countRow != lastRow))) {
                // pega índice da célula
                cellDestiny = MATRIX_evalCellIndex(countRow, countColumn,
                        (*matrix)->columns);
                // remove ou adiciona
                if (isRemove)
                    MATRIX_removeDependency(
                            &((*matrix)->graph.cells[cellDestiny]), cellIndex);
                else
                    MATRIX_addDependency(&((*matrix)->graph.cells[cellDestiny]),
                            cellIndex);
            }
        }
    }
    return count;
}

/**
 * Remove ou adiciona todas as dependências em relação a uma célula específica,
 * com base na expressão
 * \param matrix Ponteiro duplo para a matriz de células
 * \param cellIndex Índice da célula que será removida da lista de dependência
 * de outras células com base na expressão
 * \param expression Expressão que contém informações de dependências
 * \param isRemove Se true, deverá remover a dependência. Caso contrário, adiciona
 */
void MATRIX_modDependencies(Matrix** matrix, int cellIndex, const char* expression,
        int isRemove){

    // variáveis para computar intervalos
    // primeira célula
    int firstCell;

    int count=0, cellDestiny;
    // percorre a expressão
    while(expression[count]!=0){

        // se encontrou dois pontos, então computa intervalo
        if(expression[count]==':'){
            // extrair dependências do intervalo e atualiza contador
            count = MATRIX_extractDepsFromInterval(count, expression,isRemove, cellIndex,
                    &(*matrix),firstCell);
        }

        // faz comparações com base na table ascii
        // um caractere não-número seguido de um número?
        else if(MATRIX_charIsAlpha(expression[count])
                && expression[count+1]!=0 && MATRIX_charIsNumber(expression[count+1])){

            // pega índice da célula no grafo
            cellDestiny = MATRIX_getCellIndex_fromReference(expression, &count,
                    (*matrix)->columns);

            // remove ou adiciona
            if(isRemove)
                MATRIX_removeDependency(&((*matrix)->graph.cells[cellDestiny]), cellIndex);
            else
                MATRIX_addDependency(&((*matrix)->graph.cells[cellDestiny]), cellIndex);

            // presume que essa referência é a primeira de um intervalo
            firstCell = cellDestiny;
        }
        // pula um caractere
        count++;
    }
}

/**
 * Adiciona valor numérico na pilha de árvore de expressão binária
 * \return Valor de count atualizado
 * \param count Contador que permanece no começo do número (ele é atualizado
 * para o valor de check ao final do algoritmo)
 * \param expression Expressão que contém a sequência numérica
 * \param stackBin Ponteiro para pilha de árvore de expressão numérica
 */
int MATRIX_pushNumberValue(int count, char* expression,StackBinExpTree** stackBin) {
    // usa check para percorrer a expressão mantendo count no lugar
    int check = count;

    // guarda string do número
    char number_char[60];

    // enquanto for caractere pertencente ao número...
    while (MATRIX_charIsNumber(expression[check]) || expression[check] == '.') {
        // vai preenchendo number_char
        number_char[check - count] = expression[check];
        check++;
    }
    // final de linha
    number_char[check - count] = 0;
    // adiciona valor na pilha de expressão binária
    STACKBINEXPTREE_pushValue(&(*stackBin), atof(number_char));
    // atualiza count
    count = check;
    return count;
}

/**
 * Coloca valor da referência na pilha de árvore de expressão binária
 * \return Valor de count atualizado
 * \param expression Expressão que contém a referência
 * \param count Contador que percorre a expressão
 * \param matrix Ponteiro para a matriz de células
 * \param stackBin Ponteiro para a pilha de árvores de expressão binária
 */
int MATRIX_pushReference(char* expression, int count,
        Matrix** matrix, StackBinExpTree** stackBin) {

    // pega índice da célula com base na referência
    int cellTempIndex = MATRIX_getCellIndex_fromReference(expression, &count,
            (*matrix)->columns);
    // coloca valor na pilha de expressão binária
    if (!(*matrix)->graph.cells[cellTempIndex])
        STACKBINEXPTREE_pushValue(&*stackBin, 0);
    else
        STACKBINEXPTREE_pushValue(&*stackBin,
                (*matrix)->graph.cells[cellTempIndex]->value);

    // vai para o próximo caractere
    count++;
    return count;
}

/**
 * Extrai lista de valores de uma sequência de referências
 * \return Valor atualizado de count
 * \param count Contador que percorre a expressão
 * \param expression Expressão que contém o intervalo
 * \param matrix Ponteiro para a matriz de células
 * \param firstCell Índice da primeira célula do intervalo no grafo (já computada)
 * \param list Lista de valores a ser computada na função posteriormente
 */
int MATRIX_extractList(int count, char* expression, Matrix** matrix, int firstCell,
        ListDouble** list) {

    // flag que indica qual a célula já computada anteriormente
    int computeFirstCell;

    // última célula do intervalo
    int lastCell;

    // contadores de linha e coluna
    int countRow, countColumn;

    // pula os dois pontos
    count++;
    // pula espaços em branco
    while (expression[count] == ' ')
        count++;
    // pega segunda referência
    int cellTempIndex = MATRIX_getCellIndex_fromReference(expression, &count,
            (*matrix)->columns);

    // verifica qual a referência maior e configura lastCell e firstCell de acordo
    if (firstCell > cellTempIndex) {
        lastCell = firstCell;
        firstCell = cellTempIndex;
        computeFirstCell = true;
    } else {
        lastCell = cellTempIndex;
        computeFirstCell = false;
    }
    // configura variáveis de intervalo
    int firstRow = MATRIX_getRow(firstCell, (*matrix)->columns);
    int lastRow = MATRIX_getRow(lastCell, (*matrix)->columns);
    int firstColumn = MATRIX_getColumn(firstCell, (*matrix)->columns);
    int lastColumn = MATRIX_getColumn(lastCell, (*matrix)->columns);
    // percorre intervalo de células
    for (countRow = firstRow; countRow <= lastRow; countRow++) {
        for (countColumn = firstColumn; countColumn <= lastColumn;countColumn++) {
            // não computa primeira ou última célula
            if ((!computeFirstCell
                    && (countColumn != firstColumn || countRow != firstRow))
                    || (computeFirstCell
                            && (countColumn != lastColumn
                                    || countRow != lastRow))) {
                // pega índice da célula
                cellTempIndex = MATRIX_evalCellIndex(countRow, countColumn,
                        (*matrix)->columns);
                // adiciona na lista da função
                if (!(*matrix)->graph.cells[cellTempIndex])
                    *list = FUNCTIONS_addValue(*list, 0);
                else
                    *list = FUNCTIONS_addValue(*list,
                            (*matrix)->graph.cells[cellTempIndex]->value);
            }
        }
    }
    // pula para o próximo caractere
    count++;
    return count;
}

/**
 * Adiciona valor da referência na lista que será usada para computar valor da função
 * \return Valor atualizado de count
 * \param count Contador que percorrre a expressão
 * \param countComma Conta quantas vírgulas já apareceram na função
 * \param expression Expressão que contém o número
 * \param matrix Ponteiro para a matriz de células
 * \param list Lista de valores a ser computada pela função posteriormente
 * \param firstCell Guarda índice da primeira célula
 */
int MATRIX_pushReferenceToList(int count, int countComma,char* expression,
        Matrix** matrix, ListDouble** list, int* firstCell) {

    // pega índice da célula no grafo
    int cellTempIndex = MATRIX_getCellIndex_fromReference(expression, &count,
            (*matrix)->columns);
    // adiciona valor na lista
    if (!(*matrix)->graph.cells[cellTempIndex])
        *list = FUNCTIONS_addValue(*list, 0);
    else
        *list = FUNCTIONS_addValue(*list,
                (*matrix)->graph.cells[cellTempIndex]->value);

    // se não encontrou vírgula, é possível primeira célula de um intervalo
    if (!countComma)
        *firstCell = cellTempIndex;

    // pula para o próximo caractere
    count++;
    return count;
}

/**
 * Adiciona valor numérico na lista que será posteriormente processada pela função
 * \return Valor atualizado de count
 * \param count Contador que percorre a expressão
 * \param expression Expressão que possui o valor numérico
 * \para list Lista de valores a ser posteriormente processada pela função
 */
int MATRIX_pushNumberToList(int count, char* expression, ListDouble** list) {
    // usa variável check para percorrer expressão mantendo count no lugar
    int check = count;

    // guarda valor numérico que possa existir na expressão
    char number_char[60];

    // enquanto não encontrar vírgula ou parênteses...
    while (expression[check] != ',' && expression[check] != ')') {
        // preenche number_char
        number_char[check - count] = expression[check];
        check++;
    }
    // final de linha
    number_char[check - count] = 0;
    // adiciona valor na lista (converte number_char para double)
    *list = FUNCTIONS_addValue(*list, atof(number_char));
    // atualiza count
    count = check;
    return count;
}

/**
 * Adiciona valor da função na pilha de árvore de expressão binária
 * \return Valor atualizado de count
 * \param count Contador que percorre a expressão
 * \param expression Expressão que contém a função
 * \param matrix Ponteiro para a matriz de células
 * \param stackBin Ponteiro para a pilha de árvore de expressão binária
 */
int MATRIX_pushFunctionValue(int count, char *expression, Matrix** matrix,
        StackBinExpTree** stackBin) {

    // usa check para percorrer a expressão mantendo count no lugar
    int check = count;

    // guarda nome da função
    char function[10];

    // Lista de valores para computar em uma função
    ListDouble* list;

    // conta vírgulas
    int countComma;

    // variáveis para computar intervalos
    // primeira célula
    int firstCell;

    // enquanto não encontrar um abre parênteses...
    while (expression[check] != '(') {
        // preenche function
        function[check - count] = expression[check];
        check++;
    }
    // final de linha
    function[check - count] = 0;
    // atualiza count para um caractere a mais que check, para pular o parênteses
    count = check + 1;

    // cria lista de valores a ser usada com a função
    list = FUNCTIONS_createList();
    // inicializa contador de vírgulas
    countComma = 0;
    // enquanto o caractere não for fechar parênteses...
    while (expression[count] != ')') {

        // pula espaços
        if (expression[count] == ' ') {
            count++;
        }

        // pula vírgulas
        else if (expression[count] == ',') {
            count++;
            countComma++;
        }

        // se encontrou dois pontos, inicializa intervalo
        else if (expression[count] == ':') {
            // extrai lista de valores e atualiza valor do contador
            count = MATRIX_extractList(count,expression, &(*matrix), firstCell, &list);
        }

        // uma referência de célula
        else if (MATRIX_charIsAlpha(expression[count])
                && MATRIX_charIsNumber(expression[count + 1])) {

            // Adiciona valor da referência na lista e atualiza count
            count = MATRIX_pushReferenceToList(count, countComma,
                    expression, matrix, &list, &firstCell);
        }
        // um número
        else {
            // usa variável check para percorrer expressão mantendo count no lugar
            count = MATRIX_pushNumberToList(count,expression, &list);
        }
    }
    // pula para o próximo caractere (que está em um fecha parênteses)
    count++;
    // adiciona resultado da função na pilha de árvore de expressão binária
    STACKBINEXPTREE_pushValue(&*stackBin,
            FUNCTIONS_evalFunction(function, &list));
    // libera fila de valores doubles
    list = FUNCTIONS_free(list);
    return count;
}

/**
 * Computa o valor da célula
 * \param matrix Ponteiro duplo para matriz de células
 * \param cellIndex Índice da célula no grafo que terá o valor atualizado
 * \param graphic Ponteiro duplo para GraphicCells
 */
void MATRIX_evalCellValue(Matrix ** matrix, int cellIndex, GraphicCells** graphic){
    if(!matrix || !(*matrix) || !(*matrix)->graph.cells[cellIndex]) return;

    // copia expressão para uma variável
    char expression[60];
    strcpy(expression, (*matrix)->graph.cells[cellIndex]->expression);

    // se expressão vazia, valor da célula é zero
    if(strcmp(expression,"")==0){
        // O valor da célula será o resultado da árvore de expressão binária
        (*matrix)->graph.cells[cellIndex]->value = 0;

        // atualiza valor no gráfico
        if(graphic && (*graphic))
            GRAPHICSCELLS_updateCell(&(*graphic), MATRIX_getRow(cellIndex, (*matrix)->columns),
                    MATRIX_getColumn(cellIndex,(*matrix)->columns),
                    (*matrix)->graph.cells[cellIndex]->value, KEEP_MARK, true);
        return;
    }

    // Pilha de árvore de expressão binária
    StackBinExpTree* stackBin = STACKBINEXPTREE_create();

    // percorre expressão
    int count=0;
    while(expression[count] != 0){

        // pula espaços em branco
        if(expression[count]==' '){
            count++;
        }

        // operador
        else if(MATRIX_charIsOperator(expression[count])){
            // adiciona operador na pilha de expressão binária
            STACKBINEXPTREE_pushSymbol(&stackBin, expression[count]);
            // vai para o próximo caractere
            count++;
        }

        // número
        else if(MATRIX_charIsNumber(expression[count])){
            // coloca valor numérico na pilha de árvore de expressão binário e
            // atualiza count
            count = MATRIX_pushNumberValue(count, expression,&stackBin);
        }

        // referência para uma célula
        else if(MATRIX_charIsAlpha(expression[count]) && expression[count+1]!=0
                && MATRIX_charIsNumber(expression[count+1]) ){

            // coloca valor da referência na pilha de árvore de expressão binária
            // e atualiza valor de count
            count = MATRIX_pushReference(expression, count,
                    &(*matrix), &stackBin);
        }

        // função
        else{
            // Coloca valor da função na pilha de árvore de expressão binária
            // e atualiza count
            count = MATRIX_pushFunctionValue(count, expression,&(*matrix),
                    &stackBin);
        }
    }

    // O valor da célula será o resultado da árvore de expressão binária
    (*matrix)->graph.cells[cellIndex]->value = STACKBINEXPTREE_pop(&stackBin);

    // atualiza valor no gráfico
    if(graphic && (*graphic))
        GRAPHICSCELLS_updateCell(&(*graphic), MATRIX_getRow(cellIndex, (*matrix)->columns),
                MATRIX_getColumn(cellIndex,(*matrix)->columns),
                (*matrix)->graph.cells[cellIndex]->value, KEEP_MARK, false);

    // libera pilha de árvore de expressão binária
    stackBin = STACKBINEXPTREE_free(stackBin);
}

/**
 * Calcula valor de todas as células da lista de dependência
 * \param matrix Ponteiro duplo para matriz de células
 * \param cellIndex Índice da célula que terá células dependentes atualizadas
 * \param originalCell Índice da célula original que começou a recursão (no
 * início, cellIndex e originalCell possuem o mesmo valor)
 * \param graphic Ponteiro duplo para GraphicCells
 */
void MATRIX_evalCellDepsValue(Matrix** matrix, int cellIndex, int originalCell,
        GraphicCells** graphic){

    Dependency* dep = NULL;
    if((*matrix)->graph.cells[cellIndex])
        dep = (*matrix)->graph.cells[cellIndex]->first;

    // enquanto o nó de dependência for diferente de nulo...
    while(dep){
        // se o índice da célula dependente for diferente da célula original
        // (isso evita o problema de loop infinito em referências cíclicas)
        if(dep->value != originalCell && dep->value != cellIndex){
            // atualiza valor da célula
            MATRIX_evalCellValue(&(*matrix), dep->value, &(*graphic));
            // atualiza valor das células dependentes desta
            MATRIX_evalCellDepsValue(&(*matrix), dep->value, originalCell, &(*graphic));
        }
        dep = dep->next;
    }
}

/**
 * Procura recursivamente por dependência cíclica
 * \return 1 se encontrar dependência cíclica, e 0 se não encontrar
 * \param cellIndex índice da célula atual que está realizando a checagem
 * \param indexCheck Índice a ser checado (que pode causar uma referência cíclica)
 * \param matrix Ponteiro duplo para Matrix, que possui informações sobre as células
 */
int MATRIX_checkCyclicDependencyRecursive(int cellIndex, int indexCheck,
        Matrix** matrix){

    if(!(*matrix)->graph.cells[cellIndex]) return 0;

    Dependency* cellDependency = (*matrix)->graph.cells[cellIndex]->first;

    int result;

    // percorre as dependências da célula atual
    while(cellDependency){
        // se a dependência atual bate com o índice da célula atual, erro
        if(cellDependency->value == indexCheck){
            return 1;
        }
        // chama função recursiva para checar na célula dependente
        result = MATRIX_checkCyclicDependencyRecursive(cellDependency->value,
                indexCheck, &(*matrix));

        // encontrou dependência cíclica
        if(result) return 1;

        cellDependency = cellDependency->next;
    }

    // tudo ok
    return 0;
}

/**
 * Verifica se há caractere inválido após o número
 * \return 1 em caso de sucesso, 0 em caso de falha
 * \param expression Expressão que contém o valor numérico
 * \param count Contador que percorre a expressão
 * \param graphic Ponteiro para a tela de instruções
 */
int MATRIX_VAL_checkNumber(const char *expression, int* count, GraphicInstructions** graphic){

    // usado para armazenar uma mensagem
    char message[100];

    // percorre número
    while(MATRIX_charIsNumber(expression[*count]) || expression[*count]=='.')
            (*count)++;

    // se depois de percorrer número encontrar caractere inválido, erro
    if(expression[*count]!=0 && expression[*count]!=' '
            && !MATRIX_charIsAlpha(expression[*count])
            && !MATRIX_charIsOperator(expression[*count])){
        sprintf(message, "caractere nao valido %c na posicao %d",
                expression[*count],*count);
        MATRIX_showError(&(*graphic), message, 0, "");

        return 0;
    }

    return 1;
}

/**
 * Checa referência
 * \return 1 em caso de sucesso, e 0 caso contrário
 * \param expression Expressão que contém a referência
 * \param count Contador que percorre a expressão
 * \param charLimitCap Valor ascii para o maior caractere maiúsculo na referência
 * \param charLimit Valor ascii para o maior caractere minúsculo na referência
 * \param intLimit Maior valor numérico na referência
 * \param graphic Ponteiro para a tela de instruções
 */
int MATRIX_VAL_checkReference(const char *expression, int *count, int charLimitCap,
        int charLimit, int intLimit ,GraphicInstructions** graphic){

    // armazena mensagem
    char message[100];

    if(MATRIX_charIsAlpha(expression[*count])==1 && expression[*count] > charLimitCap){
        sprintf(message,
           "a referencia %c, na posicao %d, vai alem da quantidade de colunas",
           expression[*count],*count);
        MATRIX_showError(&(*graphic), message, 0, __FILE__);
        return 0;
    }
    else if(MATRIX_charIsAlpha(expression[*count])==2
            && expression[*count] > charLimit){
        sprintf(message,
           "a referencia %c, na posicao %d, vai alem da quantidade de colunas",
           expression[*count],*count);
        MATRIX_showError(&(*graphic), message, 0, "");
        return 0;
    }

    // vai para a parte numérica da referência
    (*count)++;
    if(expression[*count] > intLimit || expression[*count]==MIN_ASCII_NUMBER){
        sprintf(message,
           "a referencia %c, na posicao %d, vai alem da quantidade de linhas",
           expression[*count],*count);
        MATRIX_showError(&(*graphic), message, 0,"");
        return 0;
    }
    // vai para o próximo caractere
    (*count)++;

    return 1;
}

/**
 * Checa se não há caracteres inválidos em volta de um número em uma função
 * \return 1 se estiver tudo ok, 0 em caso contrário
 * \param expression Expressão que contém o valor numérico
 * \param countColon Contador de dois pontos
 * \param countComma Contador de vírgulas
 * \param count Contador que percorre expressão
 * \param typeFirstArgument Controla se o primeiro argumento da função é
 * um número ('n') ou uma referência ('r'). a ser preenchido pela função
 * (essa não é uma string literal, e sim um ponteiro para char)
 * \param graphic Ponteiro para a tela de instruções
 * \param function Variável que terá o nome da função atual
 */
int MATRIX_VAL_FUNC_checkNumber(const char *expression, int countColon, int countComma,
        int *count, char * typeFirstArgument, GraphicInstructions** graphic,
        const char* function){

    char message[100];

    // se precedido de dois pontos, erro
    if(countColon){
        sprintf(message,"expressao inadequada na funcao %s",function);
        MATRIX_showError(&(*graphic), message, 0,"");
        return 0;
    }

    // se não encontrou dois pontos ou vírgula, esse é o primeiro
    // argumento
    if(!countColon && !countComma)
        *typeFirstArgument = 'n';

    while(MATRIX_charIsNumber(expression[*count]) || expression[*count]=='.')
        (*count)++;

    return 1;
}

/**
 * Checa se a referência em uma função é válida
 * \return 1 se estiver tudo ok, e 0 em caso contrário
 * \param expression Expressão que contém a referência
 * \param count Contador que percorre a expressão
 * \param countReference Contador de referências na função
 * \param countColon Quantidade de dois pontos na função
 * \param countComma Quantidade de vírgulas na função
 * \param function Variável que contém nome da função atual
 * \param graphic Ponteiro para a tela de instruções
 * \param typeFirstArgument Controla se o primeiro argumento da função é
 * um número ('n') ou uma referência ('r'). a ser preenchido pela função
 * (essa não é uma string literal, e sim um ponteiro para char)
 * \param charLimitCap Valor ascii para o maior caractere maiúsculo na referência
 * \param charLimit Valor ascii para o maior caractere minúsculo na referência
 * \param intLimit Maior valor numérico na referência
 */
int MATRIX_VAL_FUNC_checkReference(const char *expression, int *count, int *countReference,
        int countColon, int countComma, const char *function, GraphicInstructions** graphic,
        char * typeFirstArgument, int charLimitCap, int charLimit, int intLimit){

    // guarda mensagem
    char message[100];

    // incrementa contador de referências
    (*countReference)++;

    // se apareceu dois pontos e tem mais de duas referências, erro
    if(countColon && (*countReference) > 2){
        sprintf(message,"expressao inadequada na funcao %s",function);
        MATRIX_showError(&(*graphic), message, 0, "");
        return 0;
    }

    // se não encontrou dois pontos ou vírgula, primeiro argumento
    if(!countColon && !countComma){
        *typeFirstArgument = 'r';
    }
    // se precedido de dois pontos, e primeiro argumento é número, erro
    else if(countColon && *typeFirstArgument=='n'){
        sprintf(message,"expressao inadequada na funcao %s",function);
        MATRIX_showError(&(*graphic), message, 0, "");
        return 0;
    }

    // referência vai além da quantidade de colunas
    if(MATRIX_charIsAlpha(expression[*count])==1
            && expression[*count] > charLimitCap){
        sprintf(message,
     "a referencia %c, na posicao %d, vai alem da quantidade de colunas",
           expression[*count],*count);
        MATRIX_showError(&(*graphic), message, 0, "");
        return 0;
    }
    else if(MATRIX_charIsAlpha(expression[*count])==2
            && expression[*count] > charLimit){
        sprintf(message,
    "a referencia %c, na posicao %d, vai alem da quantidade de colunas",
           expression[*count],*count);
        MATRIX_showError(&(*graphic), message, 0, "");
        return 0;
    }

    // vai para a parte numérica da referência
    (*count)++;

    // referência vai além da quantidade de linhas
    if(expression[*count] > intLimit || expression[*count]==MIN_ASCII_NUMBER){
        sprintf(message,
      "a referencia %c, na posicao %d, vai alem da quantidade de linhas",
           expression[*count],*count);
        MATRIX_showError(&(*graphic), message, 0,"");
        return 0;
    }

    // vai para o próximo caractere
    (*count)++;

    return 1;
}

/**
 * Checa função
 * \return 1 se estiver tudo ok, 0 em caso contrário
 * \param expression Expressão que contém a função
 * \param count Contador que percorre a expressão
 * \param charLimitCap Valor ascii para o maior caractere maiúsculo na referência
 * \param charLimit Valor ascii para o maior caractere minúsculo na referência
 * \param intLimit Maior valor numérico na referência
 * \param graphic Ponteiro para a tela de instruções
 */
int MATRIX_VAL_checkFunction(const char *expression, int *count, int charLimitCap,
        int charLimit, int intLimit, GraphicInstructions** graphic){

    // guarda nome da função
    char function[10];

    // guarda mensagem
    char message[100];

    // guarda o tipo do primeiro argumento de uma função
    // se 'n', é número
    // se 'r', é referência
    // se '-', nem número nem referência foi lida ainda
    char typeFirstArgument = '-';

    // usa check para percorrer a expressão mantendo count no lugar
    int check=*count;
    // enquanto não encontrar um abre parênteses...
    while(expression[check]!='('){
        // preenche function
        function[check-(*count)] = expression[check];
        check++;
    }
    // final de linha
    function[check-(*count)] = 0;

    // se a função não existir, sai com erro
    if(!FUNCTIONS_isFunction(function)){
        sprintf(message, "funcao %s nao existente na posicao %d", function,*count);
        MATRIX_showError(&(*graphic), message, 0, "");
        return 0;
    }
    // count é um caractere a mais que check
    *count= check+1;

    // contadores de vírgula e de dois pontos começam em zero
    int countColon = 0;
    int countComma = 0;
    // contador de referências começa em zero
    int countReference = 0;

    // enquanto não encontrar um fecha parênteses...
    while(expression[*count]!=')'){

        // pula espaços
        if(expression[*count]==' '){
            (*count)++;
            continue;
        }

        // pula vírgulas
        if(expression[*count]==','){
            // se já foi usado dois pontos ou primeiro argumento é '-', erro
            if(countColon || typeFirstArgument == '-'){
                sprintf(message,"expressao inadequada na funcao %s",function);
                MATRIX_showError(&(*graphic), message, 0,"");
                return 0;
            }

            countComma++;
            (*count)++;
            continue;
        }

        // pula dois pontos
        if(expression[*count]==':'){
            // se já foi usado vírgula ou dois pontos ou primeiro argumento é '-', erro
            if(countComma || countColon || typeFirstArgument=='-'){
                sprintf(message,"expressao inadequada na funcao %s",function);
                MATRIX_showError(&(*graphic), message, 0,"");
                return 0;
            }

            countColon++;
            (*count)++;
            continue;
        }

        // número
        if(MATRIX_charIsNumber(expression[*count])){

            // verifica se número é válido
            if(!MATRIX_VAL_FUNC_checkNumber(expression, countColon, countComma,
                    &(*count), &typeFirstArgument, &(*graphic), function)) return 0;

        }
        // referência de célula
        else if(MATRIX_charIsAlpha(expression[*count]) && expression[(*count)+1]!=0
                && MATRIX_charIsNumber(expression[(*count)+1])){

            // verifica se a referência é válida
            if(!MATRIX_VAL_FUNC_checkReference(expression,&(*count),&countReference,countColon,
                    countComma, function, &(*graphic), &typeFirstArgument,charLimitCap,
                    charLimit, intLimit)) return 0;
        }
        // caractere inválido
        else{

            if(expression[*count]==0){
                sprintf(message, "parenteses nao fechou antes do final da expressao");
                MATRIX_showError(&(*graphic), message, 0, "");
                return 0;
            }

            sprintf(message, "caractere nao valido %c na posicao %d",
                    expression[*count],*count);
            MATRIX_showError(&(*graphic), message, 0, "");
            return 0;
        }
    }
    // está em um parênteses
    (*count)++;

    return 1;
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
 * Pega número de linhas da matriz
 * \return Linhas da matriz. -1 em caso de erro
 * \param matrix Ponteiro duplo para matriz Matrix
 */
int MATRIX_getRows(Matrix** matrix){
    if(!matrix || !(*matrix)) return -1;

    return (*matrix)->rows;
}

/**
 * Pega número de colunas da matriz
 * \return Colunas da matriz. -1 em caso de erro
 * \param matrix Ponteiro duplo para matriz Matrix
 */
int MATRIX_getColumns(Matrix** matrix){
    if(!matrix || !(*matrix)) return -1;

    return (*matrix)->columns;
}

/**
 * Obtém expressão de uma célula específica da matriz
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param row Linha da célula
 * \param column  Coluna da célula
 * \param expression String a ser preenchida com a expressão da célula.
 * Se a célula não estiver alocada ou não houver expressão, essa variável
 * é preenchida com uma string vazia ("")
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
 * \param graphic Ponteiro duplo para GraphicCells
 */
int MATRIX_setExpression(Matrix** matrix, int row, int column, const char* expression,
        UndoRedoCells** undoRedo, GraphicCells** graphic){
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
        if(graphic)
            GRAPHICSCELLS_updateCell(&(*graphic), row, column, 0, KEEP_MARK, true);
        return 1;
    }

    // computa o valor da célula
    // necessário mesmo quando célula não contém expressão, pois o valor precisa,
    // neste caso, ser atualizado para 0
    MATRIX_evalCellValue(&(*matrix), cellIndex, &(*graphic));

    // se a célula não contém expressão, então é célula vazia
    if(strcmp((*matrix)->graph.cells[cellIndex]->expression, "")==0
            && graphic)
        GRAPHICSCELLS_updateCell(&(*graphic), row, column, 0, KEEP_MARK, true);

    // percorre todas as dependências para atualizar todas as células que dependem desta
    MATRIX_evalCellDepsValue(&(*matrix), cellIndex, cellIndex, &(*graphic));

    return 1;
}

/**
 * Tenta realizar operação de desfazer na matriz de células
 * \return 1 se obtiver sucesso e 0 em caso contrário
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param undoRedo Ponteiro duplo para fila de desfazer/refazer
 * \param graphic Ponteiro duplo para GraphicCells
 */
int MATRIX_undo(Matrix** matrix, UndoRedoCells** undoRedo, GraphicCells** graphic){
    if(!matrix || !(*matrix) || !undoRedo || !(*undoRedo)) return 0;

    if(!UNDOREDOCELLS_canUndo(&(*undoRedo))) return 0;

    // variáveis a serem obtidas da fila undoRedo
    char expression[60];
    int cellIndex;

    if(!UNDOREDOCELLS_undo(&(*undoRedo), expression, &cellIndex)) return 0;

    // obtém linha e coluna correspondentes
    int row = MATRIX_getRow(cellIndex, (*matrix)->columns);
    int column = MATRIX_getColumn(cellIndex, (*matrix)->columns);

    // preenche expressão da célula correta, sem colocar na pilha novamente
    if(!MATRIX_setExpression(&(*matrix), row, column, expression, NULL, &(*graphic)))
        return 0;

    return 1;
}

/**
 * Tenta realizar operação de refazer na matriz de células
 * \return 1 se obtiver sucesso e 0 em caso contrário
 * \param matrix Ponteiro duplo para matriz Matrix
 * \param undoRedo Ponteiro duplo para fila de desfazer/refazer
 * \param graphic Ponteiro duplo para GraphicCells
 */
int MATRIX_redo(Matrix** matrix, UndoRedoCells** undoRedo, GraphicCells** graphic){
    if(!matrix || !(*matrix) || !undoRedo || !(*undoRedo)) return 0;

    if(!UNDOREDOCELLS_canRedo(&(*undoRedo))) return 0;

    // variáveis a serem obtidas da fila undoRedo
    char expression[60];
    int cellIndex;

    if(!UNDOREDOCELLS_redo(&(*undoRedo), expression, &cellIndex)) return 0;

    // obtém linha e coluna correspondentes
    int row = MATRIX_getRow(cellIndex, (*matrix)->columns);
    int column = MATRIX_getColumn(cellIndex, (*matrix)->columns);

    // preenche expressão da célula correta, sem colocar na pilha novamente
    if(!MATRIX_setExpression(&(*matrix), row, column, expression, NULL, &(*graphic))) return 0;

    return 1;
}

/**
 * Valida expressão
 * \return 1 se a expressão for válida, 0 em caso contrário
 * \param graphic Ponteiro duplo para GraphicInstructions (para informar o usuário
 * de possíveis erros). Informe NULL se não desejar imprimir essas informações
 * \param rows Quantidade de linhas da matriz de células
 * \param columns Quantidade de colunas da matriz de células
 * \param expression Expressão a ser validada
 */
int MATRIX_validateExpression(GraphicInstructions** graphic, int rows, int columns,
        const char *expression){

    // expressão vazia é automaticamente aprovada
    if(strcmp(expression, "")==0) return 1;

    if(graphic && (*graphic))
        GRAPHICINST_clear(&(*graphic));

    // guarda mensagem de erro a ser informada para o usuário
    char message[100];

    // maior caractere maiúsculo e minúsculo possível para a coluna
    int charLimitCap, charLimit;
    charLimitCap = columns + MIN_ASCII_CAP_LETTER - 1;
    charLimit = columns + MIN_ASCII_LETTER - 1;

    // maior número para a linha
    int intLimit = rows + MIN_ASCII_NUMBER;

    // guarda a quantidade de elementos da pilha (cada número, referência
    // ou função aumenta a pilha em um; cada operação pega dois elementos
    // da pilha e coloca o resultado no lugar)
    int stack=0;

    int count=0;
    while(expression[count] != 0){

        // pula espaços em branco
        if(expression[count]==' '){
            count++;
            continue;
        }

        // operador
        if(MATRIX_charIsOperator(expression[count])){
            // a pilha diminui em um
            stack--;
            count++;
        }

        // número
        else if(MATRIX_charIsNumber(expression[count])){
            // a pilha aumenta em um
            stack++;

            // se houver algum caractere inválido, retorna erro
            if(!MATRIX_VAL_checkNumber(expression,&count,&(*graphic)))
                return 0;
        }

        // referência de célula
        else if(MATRIX_charIsAlpha(expression[count]) && expression[count+1]!=0
                && MATRIX_charIsNumber(expression[count+1])){

            // a pilha aumenta em um
            stack++;

            // checa referência
            if(!MATRIX_VAL_checkReference(expression, &count, charLimitCap, charLimit,
                    intLimit, &(*graphic))) return 0;
        }

        // função
        else if(MATRIX_charIsAlpha(expression[count]) && expression[count+1]!=0
                && MATRIX_charIsAlpha(expression[count+1])){

            // a pilha aumenta em um
            stack++;

            // verifica se a função é válida
            if(!MATRIX_VAL_checkFunction(expression,&count, charLimitCap,charLimit,
                    intLimit, &(*graphic))) return 0;
        }

        // caractere inválido
        else{
            sprintf(message, "caractere nao valido %c na posicao %d",
                    expression[count],count);
            MATRIX_showError(&(*graphic), message, 0, "");
            return 0;
        }

        // ao final de cada iteração stack deve ser maior ou igual a 1
        // se for menor que 1, a quantidade de elementos vs operadores está desbalanceada
        if(stack<1){
            sprintf(message, "desbalanceamento entre operadores e operandos");
            MATRIX_showError(&(*graphic), message, 0,"");
            return 0;
        }
    }

    // a variável stack deve ser 1
    // se for maior que 1, alguns operandos não foram usados
    // se for menor que 1, não há operandos suficientes
    if(stack!=1){
        sprintf(message, "desbalanceamento entre operadores e operandos");
        MATRIX_showError(&(*graphic), message, 0,"");
        return 0;
    }

    return 1;
}

/**
 * Checa se haverá dependência cíclica se uma expressão for configurada em uma célula
 * \return 1 se houver dependência na expressão, 0 em caso contrário
 * \param rows Quantidade de linhas da matriz de células
 * \param columns Quantidade de colunas da matriz de células
 * \param expression Expressão a ser validada
 * \param matrix Ponteiro duplo para matriz Matrix
 */
int MATRIX_checkCyclicDependency(int row, int column, const char* expression, Matrix** matrix){

    if(!matrix || !(*matrix)) return 0;

    // calcula o índice da célula atual
    int cellIndex = MATRIX_evalCellIndex(row,column, (*matrix)->columns);

    // Ponteiro usado para realizar iterações nas dependências da célula
    Dependency* cellDependency = NULL;

    // variáveis para computar intervalos
    // primeira célula e última célula
    int firstCell, lastCell;
    // guarda primeira e última linha, primeira e última coluna
    int firstRow, lastRow, firstColumn, lastColumn;
    // percorre linha e coluna
    int countRow, countColumn;
    // flag que indica qual a célula já computada anteriormente
    int computeFirstCell;

    int count=0, cellDestiny;
    // percorre a expressão
    while(expression[count]!=0){

        // se encontrou dois pontos, então computa intervalo
        if(expression[count]==':'){
            // pula os dois pontos
            count++;
            // pula espaços em branco
            while(expression[count]==' ')
                count++;

            // pega segunda referência
            cellDestiny = MATRIX_getCellIndex_fromReference(expression, &count,
                    (*matrix)->columns);

            // verifica qual a referência maior e configura lastCell e firstCell de acordo
            if(firstCell > cellDestiny){
                lastCell = firstCell;
                firstCell = cellDestiny;
                computeFirstCell = true;
            }else{
                lastCell = cellDestiny;
                computeFirstCell=false;
            }

            // configura variáveis de intervalo
            firstRow = MATRIX_getRow(firstCell,(*matrix)->columns);
            lastRow = MATRIX_getRow(lastCell,(*matrix)->columns);
            firstColumn = MATRIX_getColumn(firstCell, (*matrix)->columns);
            lastColumn = MATRIX_getColumn(lastCell, (*matrix)->columns);

            // percorre intervalo de células
            for(countRow = firstRow; countRow<=lastRow; countRow++){
                for(countColumn = firstColumn; countColumn <= lastColumn; countColumn++){
                    // não computa primeira ou última célula
                    if((!computeFirstCell && (countColumn!=firstColumn || countRow!=firstRow))
                        || (computeFirstCell
                            && ( countColumn!=lastColumn || countRow!=lastRow ))){
                        // pega índice da célula
                        cellDestiny = MATRIX_evalCellIndex(countRow, countColumn,
                                (*matrix)->columns);

                        // se a célula destino for a própria célula, erro
                        if(cellIndex == cellDestiny)
                            return 1;

                        // percorre dependências da célula
                        if((*matrix)->graph.cells[cellIndex]){
                            cellDependency = (*matrix)->graph.cells[cellIndex]->first;
                            while(cellDependency){
                                // se a dependência atual bate com o índice da célula atual, erro
                                if(cellDependency->value == cellDestiny)
                                    return 1;

                                // chama função recursiva para checar na célula dependente
                                if(MATRIX_checkCyclicDependencyRecursive(cellDependency->value,
                                        cellDestiny, &(*matrix))) return 1;

                                cellDependency = cellDependency->next;
                            }
                        }
                    }
                }
            }
        }

        // faz comparações com base na table ascii
        // um caractere não-número seguido de um número?
        else if(MATRIX_charIsAlpha(expression[count])
                && expression[count+1]!=0 && MATRIX_charIsNumber(expression[count+1])){

            // pega índice da célula no grafo
            cellDestiny = MATRIX_getCellIndex_fromReference(expression, &count,
                    (*matrix)->columns);

            // se a célula destino for a própria célula, erro
            if(cellIndex == cellDestiny)
                return 1;

            // percorre dependências da célula
            if((*matrix)->graph.cells[cellIndex]){
                cellDependency = (*matrix)->graph.cells[cellIndex]->first;
                while(cellDependency){
                    // se a dependência atual bate com o índice da célula atual, erro
                    if(cellDependency->value == cellDestiny)
                        return 1;

                    // chama função recursiva para checar na célula dependente
                    if(MATRIX_checkCyclicDependencyRecursive(cellDependency->value,
                            cellDestiny, &(*matrix))) return 1;

                    cellDependency = cellDependency->next;
                }
            }

            // presume que essa referência é a primeira de um intervalo
            firstCell = cellDestiny;
        }
        // pula um caractere
        count++;
    }

    // tudo ok
    return 0;
}
