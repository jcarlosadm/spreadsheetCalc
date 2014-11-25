/*
 * \file spreadsheet.c
 * Implementação do arquivo spreadsheet.h
 */

#include "spreadsheet.h"

// Largura total da janela
#define WINDOW_WIDTH 130
// Altura total da janela
#define WINDOW_HEIGHT 40

// Posicionamento da matriz de células
#define WINDOW_CELLS_X 0
#define WINDOW_CELLS_Y 0

// Posicionamento e dimensões da tela de instruções
#define WINDOW_INSTRUCTION_X 10
#define WINDOW_INSTRUCTION_Y 24
#define WINDOW_INSTRUCTION_WIDTH 90
#define WINDOW_INSTRUCTION_HEIGHT 12

// Posicionamento e dimensões da tela de usuário
#define WINDOW_USER_X WINDOW_INSTRUCTION_X
#define WINDOW_USER_Y WINDOW_INSTRUCTION_Y+WINDOW_INSTRUCTION_HEIGHT
#define WINDOW_USER_WIDTH WINDOW_INSTRUCTION_WIDTH
#define WINDOW_USER_HEIGHT 4

// Posicionamento e dimensões da tela de opções
#define WINDOW_SELECT_X WINDOW_INSTRUCTION_X+WINDOW_INSTRUCTION_WIDTH
#define WINDOW_SELECT_Y WINDOW_INSTRUCTION_Y
#define WINDOW_SELECT_WIDTH WINDOW_WIDTH-WINDOW_INSTRUCTION_WIDTH
#define WINDOW_SELECT_HEIGHT 16

// Posicionamento e dimensões da tela de listagem
#define WINDOW_LIST_X 10
#define WINDOW_LIST_Y 0
#define WINDOW_LIST_WIDTH 50
#define WINDOW_LIST_HEIGHT 24

// opções do loop principal da função run()
#define OPTION_SELECT_CELL "Selecionar celula"
#define OPTION_INSERT_EXPRESSION "Inserir expressao"
#define OPTION_UNDO "Desfazer ultima operacao"
#define OPTION_REDO "Refazer ultima operacao"
#define OPTION_SAVE "Salvar espaco de trabalho"
#define OPTION_EXIT "Sair"

// opções YES/NO
#define YES "Sim"
#define NO "Nao"

// dimensões de linha e coluna
#define ROW 1
#define COLUMN 1

/*******************************************************************************
 * Funções privadas
 *******************************************************************************/

/**
 * Atualiza gráfico de células
 * \param matrix Ponteiro para informações da matriz de célula
 * \param graphic Ponteiro para gráfico de células
 */
void SPREADSHEET_updateGraphicCells(Matrix** matrix, GraphicCells** graphic){

    if(!matrix || !(*matrix) || !graphic || !(*graphic)) return;

    // guarda linha e coluna
    int row, column;
    // guarda valor da célula
    double value;
    // guarda expressão da célula
    char expression[70];

    // total de linhas e colunas
    int rows = MATRIX_getRows(&(*matrix)), columns = MATRIX_getColumns(&(*matrix));

    for(row=1 ; row <= rows ; row++){
        for(column=1 ; column <= columns ; column++){
            // pega expressão da célula
            MATRIX_getExpression(&(*matrix), row, column, expression);
            // se expressão não vazia, atualiza gráfico
            if(strcmp(expression,"")!=0){
                value = MATRIX_getValue(&(*matrix), row, column);
                GRAPHICSCELLS_updateCell(&(*graphic), row, column, value, KEEP_MARK, false);
            }
        }
    }
}

/**
 * Algoritmo para alterar expressão de uma célula
 * \param matrix Ponteiro para matriz de células
 * \param currentRow Número da linha atual
 * \param currentColumn Número da coluna atual
 * \param graphic_instructions Ponteiro para a janela de instruções
 * \param graphic_cells Ponteiro para o gráfico da matriz de células
 * \param graphic_select Ponteiro para a janela de seleção
 * \param graphic_user Ponteiro para a janela de entrada do usuário
 * \param undoRedo Ponteiro para fila desfazer/refazer
 */
void SPREADSHEET_changeExpression(Matrix** matrix, int currentRow, int currentColumn,
        GraphicInstructions** graphic_instructions, GraphicCells** graphic_cells,
        GraphicSelect** graphic_select, GraphicUser** graphic_user,
        UndoRedoCells** undoRedo){

    if(!matrix || !(*matrix) || !graphic_instructions || !(*graphic_instructions)
            || !graphic_cells || !(*graphic_cells) || !graphic_select
            || !(*graphic_select) || !graphic_user || !(*graphic_user)
            || !undoRedo || !(*undoRedo)) return;

    // guarda a entrada do usuário
    char userText[70];
    // guarda a opção escolhida
    char option[30];

    // guarda expressão atual
    char expression[70];

    // controla loop principal da função
    int mainLoop = true;

    while(mainLoop){

        // pega expressão atual
        MATRIX_getExpression(&(*matrix), currentRow, currentColumn, expression);

        // pede para o usuário digitar uma expressão ou digitar 00 para cancelar
        GRAPHICINST_clear(&(*graphic_instructions));
        GRAPHICINST_write(&(*graphic_instructions),
                "Digite a expressao, ou 00 para cancelar", COLUMN*1, ROW*1);
        GRAPHICINST_write(&(*graphic_instructions),
                "  A expressao devera vir na forma pos-fixa ('2 2+ ao inves de 2 + 2')",
                COLUMN*1, ROW*2);
        GRAPHICINST_write(&(*graphic_instructions),
                "  Separe dois numeros consecutivos por meio de espaco",
                COLUMN*1, ROW*3);
        GRAPHICINST_write(&(*graphic_instructions),
                "  As referencias sao da forma [letra][numero]. exemplo: a1 ou A1",
                COLUMN*1, ROW*4);
        GRAPHICINST_write(&(*graphic_instructions),
                "  As funcoes que podem ser usadas sao: sum, mean, max e min",
                COLUMN*1, ROW*5);
        GRAPHICINST_write(&(*graphic_instructions),
                "  Essas funcoes aceitam valores separados por virgula,",
                COLUMN*1, ROW*6);
        GRAPHICINST_write(&(*graphic_instructions),
                "    ou intervalos (duas referencias separadas por dois pontos)",
                COLUMN*1, ROW*7);
        GRAPHICINST_write(&(*graphic_instructions),
                "  Exemplo: 2sum(a1,4.5,b2)+ ou a3mean(b1:d2)*",
                COLUMN*1, ROW*8);
        GRAPHICINST_write(&(*graphic_instructions),"Expressão atual: ",COLUMN*1, ROW*9);
        GRAPHICINST_write(&(*graphic_instructions),expression,COLUMN*18, ROW*9);

        // Abre entrada do usuário
        GRAPHICUSER_clear(&(*graphic_user));
        GRAPHICUSER_get(&(*graphic_user), userText, COLUMN*1, ROW*1);

        // escolheu 00?
        if(strcmp(userText, "00")==0){

            // sair?
            GRAPHICINST_clear(&(*graphic_instructions));
            GRAPHICINST_write(&(*graphic_instructions), "Desejar cancelar a operacao?",
                    COLUMN*1, ROW*1);
            // prepara opções
            GRAPHICSSELECT_clearOptions(&(*graphic_select));
            GRAPHICSSELECT_addOption(&(*graphic_select), YES);
            GRAPHICSSELECT_addOption(&(*graphic_select), NO);

            // abre opções
            GRAPHICSSELECT_selectOption(&(*graphic_select), option);

            // se sim, sai
            if(strcmp(option, YES)==0)
                mainLoop = false;
        }
        // inseriu uma expressão
        else{

            // limpa instruções
            GRAPHICINST_clear(&(*graphic_instructions));

            // verifica se a expressão é válida
            if(MATRIX_validateExpression(&(*graphic_instructions), MATRIX_getRows(&(*matrix)),
                MATRIX_getColumns(&(*matrix)),userText)){

                // verifica se a expressão possui referência cíclica
                if(MATRIX_checkCyclicDependency(currentRow,currentColumn,userText, &(*matrix))){
                    // informa usuário
                    GRAPHICINST_clear(&(*graphic_instructions));
                    GRAPHICINST_write(&(*graphic_instructions), "Ha referencia ciclica na expressao",
                            COLUMN*1, ROW*1);

                    // espera 1 segundo
                    sleep(1);
                }
                // se não possuir referência cíclica
                else{
                    // configura expressão na célula
                    MATRIX_setExpression(&(*matrix), currentRow, currentColumn, userText,
                            &(*undoRedo), &(*graphic_cells));

                    // informa usuário
                    GRAPHICINST_clear(&(*graphic_instructions));
                    GRAPHICINST_write(&(*graphic_instructions), "Expressao definida com sucesso",
                            COLUMN*1, ROW*1);

                    // espera 1 segundo
                    sleep(1);

                    // sai
                    mainLoop = false;
                }
            }
            else
                sleep(2);
        }
    }

    // limpa janelas
    GRAPHICINST_clear(&(*graphic_instructions));
    GRAPHICUSER_clear(&(*graphic_user));
    GRAPHICSSELECT_clearOptions(&(*graphic_select));

}

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
    GraphicUser* graphic_user = NULL;
    // Ponteiro para a janela de instruções
    GraphicInstructions* graphic_instructions = NULL;
    // Ponteiro para a janela de listagem
    GraphicInstructions* graphic_list = NULL;
    // Ponteiro para a janela de opções
    GraphicSelect* graphic_select = NULL;
    // Ponteiro para a janela de células
    GraphicCells* graphic_cells = NULL;

    // Ponteiro para dados de salvamento
    SaveFile* save = SAVE_create(SAVEFILE);

    // Cria janelas de usuário, de instruções e de opções
    graphic_user = GRAPHICUSER_create(WINDOW_USER_X,WINDOW_USER_Y, WINDOW_USER_WIDTH,
            WINDOW_USER_HEIGHT);
    graphic_instructions = GRAPHICINST_create(WINDOW_INSTRUCTION_X, WINDOW_INSTRUCTION_Y,
            WINDOW_INSTRUCTION_WIDTH, WINDOW_INSTRUCTION_HEIGHT);
    graphic_select = GRAPHICSSELECT_create(WINDOW_SELECT_X,WINDOW_SELECT_Y,
            WINDOW_SELECT_WIDTH, WINDOW_SELECT_HEIGHT);

    // se nome de espaço de trabalho não definido, define
    if(!workspaceName){

        // cria janela de listagem
        graphic_list = GRAPHICINST_create(WINDOW_LIST_X,WINDOW_LIST_Y,
                WINDOW_LIST_WIDTH, WINDOW_LIST_HEIGHT);

        // define espaço de trabalho
        while(SAVE_workspaceIsNULL(&save))
            SAVE_defineWorkspace(&save, &graphic_instructions, &graphic_list,
                    &graphic_user, &graphic_select, NULL);

        // desaloca janela de listagem
        graphic_list = GRAPHICINST_free(graphic_list);

    }

    // nome do espaço de trabalho definido
    else
        SAVE_defineWorkspace(&save, NULL, NULL, NULL, NULL, workspaceName);

    // cria o gráfico da matriz
    if(matrix)
        graphic_cells = GRAPHICSCELLS_create(WINDOW_CELLS_X,WINDOW_CELLS_Y,
                MATRIX_getRows(&(*matrix)),MATRIX_getColumns(&(*matrix)));
    else
        graphic_cells = GRAPHICSCELLS_create(WINDOW_CELLS_X,WINDOW_CELLS_Y,
                        ROWS,COLUMNS);

    // ponteiro para a matriz
    Matrix* newMatrix = NULL;

    // Inicializa matriz se não inicializada
    if(!matrix){
        // cria a matriz
        newMatrix = MATRIX_create(ROWS, COLUMNS);
    }
    // caso já tenha sido inicializada
    else{
        newMatrix = *matrix;
        // neste caso devemos percorrer a matriz e atualizar o gráfico das células
        SPREADSHEET_updateGraphicCells(&newMatrix,&graphic_cells);
    }

    // Ponteiro para undo_redo_cells
    UndoRedoCells* undoRedo = UNDOREDOCELLS_create();

    // guarda opção escolhida
    char option[30];

    // controla loop principal
    int mainLoop = true;

    // loop principal
    while(mainLoop){

        // prepara janela de seleção
        GRAPHICSSELECT_clearOptions(&graphic_select);

        // adiciona opções
        GRAPHICSSELECT_addOption(&graphic_select, OPTION_SELECT_CELL);
        GRAPHICSSELECT_addOption(&graphic_select, OPTION_INSERT_EXPRESSION);
        if(UNDOREDOCELLS_canUndo(&undoRedo))
            GRAPHICSSELECT_addOption(&graphic_select, OPTION_UNDO);
        if(UNDOREDOCELLS_canRedo(&undoRedo))
            GRAPHICSSELECT_addOption(&graphic_select, OPTION_REDO);
        GRAPHICSSELECT_addOption(&graphic_select, OPTION_SAVE);
        GRAPHICSSELECT_addOption(&graphic_select, OPTION_EXIT);

        // pede para o usuário escolher
        GRAPHICINST_clear(&graphic_instructions);
        GRAPHICINST_write(&graphic_instructions, "Escolha uma opcao:",
                COLUMN*1, ROW*1);
        GRAPHICINST_writeKeyboard(&graphic_instructions,COLUMN*1, ROW*2, false);

        // abre a janela de opções e guarda resultado da escolha em option
        GRAPHICSSELECT_selectOption(&graphic_select, option);

        // verifica opção escolhida

        // se for selecionar célula
        if(strcmp(option,OPTION_SELECT_CELL)==0){

            // cria loop interno com mainLoop
            while(mainLoop){
                // informa o usuário para escolher célula
                GRAPHICINST_clear(&graphic_instructions);
                GRAPHICINST_write(&graphic_instructions, "Escolha uma celula com os direcionais.",
                        COLUMN*1, ROW*1);
                GRAPHICINST_writeKeyboard(&graphic_instructions, COLUMN*1, ROW*2, true);

                // inicializa escolha de célula
                GRAPHICSCELL_selectCell(&graphic_cells, &currentRow, &currentColumn);

                // pergunta se deseja modificar a expressão
                GRAPHICINST_clear(&graphic_instructions);
                GRAPHICINST_write(&graphic_instructions, "Deseja modificar a expressao desta celula?",
                        COLUMN*1, ROW*1);
                GRAPHICINST_writeKeyboard(&graphic_instructions, COLUMN*1, ROW*2,false);

                // prepara opções
                GRAPHICSSELECT_clearOptions(&graphic_select);
                GRAPHICSSELECT_addOption(&graphic_select, YES);
                GRAPHICSSELECT_addOption(&graphic_select, NO);

                // abre opções
                GRAPHICSSELECT_selectOption(&graphic_select, option);

                // se sim
                if(strcmp(option,YES)==0){
                    // abre interface para mudança de expressão
                    SPREADSHEET_changeExpression(&newMatrix, currentRow,currentColumn,
                            &graphic_instructions, &graphic_cells, &graphic_select,
                            &graphic_user, &undoRedo);
                }

                // pergunta se deseja selecionar outra célula
                GRAPHICINST_clear(&graphic_instructions);
                GRAPHICINST_write(&graphic_instructions, "Deseja selecionar outra celula?",
                        COLUMN*1, ROW*1);
                GRAPHICINST_writeKeyboard(&graphic_instructions, COLUMN*1, ROW*2,false);

                // prepara opções
                GRAPHICSSELECT_clearOptions(&graphic_select);
                GRAPHICSSELECT_addOption(&graphic_select, YES);
                GRAPHICSSELECT_addOption(&graphic_select, NO);

                // abre opções
                GRAPHICSSELECT_selectOption(&graphic_select, option);

                // se não, sai deste loop interno
                if(strcmp(option,NO)==0)
                    mainLoop=false;
            }

            // loop principal volta a ser true
            mainLoop = true;
        }

        // se for inserir expressão
        else if(strcmp(option, OPTION_INSERT_EXPRESSION)==0){
            // abre interface para mudança de expressão
            SPREADSHEET_changeExpression(&newMatrix, currentRow,currentColumn,
                    &graphic_instructions, &graphic_cells, &graphic_select,
                    &graphic_user, &undoRedo);
        }

        // se for desfazer
        else if(strcmp(option, OPTION_UNDO)==0){
            MATRIX_undo(&newMatrix,&undoRedo, &graphic_cells);
        }

        // se for refazer
        else if(strcmp(option, OPTION_REDO)==0){
            MATRIX_redo(&newMatrix, &undoRedo, &graphic_cells);
        }

        // se for salvar espaço de trabalho
        else if(strcmp(option, OPTION_SAVE)==0){
            SAVE_init(&save,&graphic_instructions, &graphic_select, &newMatrix);
        }

        // se for sair
        else{
            // pergunta se deseja selecionar outra célula
            GRAPHICINST_clear(&graphic_instructions);
            GRAPHICINST_write(&graphic_instructions, "Deseja mesmo sair?",
                    COLUMN*1, ROW*1);
            GRAPHICINST_writeKeyboard(&graphic_instructions, COLUMN*1, ROW*2,false);

            // prepara opções
            GRAPHICSSELECT_clearOptions(&graphic_select);
            GRAPHICSSELECT_addOption(&graphic_select, YES);
            GRAPHICSSELECT_addOption(&graphic_select, NO);

            // abre opções
            GRAPHICSSELECT_selectOption(&graphic_select, option);

            // se não, sai deste loop interno
            if(strcmp(option,YES)==0){
                GRAPHICINST_clear(&graphic_instructions);
                GRAPHICINST_write(&graphic_instructions, "Ate mais!",
                        COLUMN*1, ROW*1);
                sleep(2);
                mainLoop=false;
            }
        }
    }

    // limpa telas
    GRAPHICINST_clear(&graphic_instructions);
    GRAPHICSSELECT_clearOptions(&graphic_select);
    GRAPHICUSER_clear(&graphic_user);

    // libera memória da matriz de célula
    newMatrix = MATRIX_free(newMatrix);

    // libera memória do save
    save = SAVE_free(save);

    // libera memória do undoRedo
    undoRedo = UNDOREDOCELLS_free(undoRedo);

    // libera memória de todos os objetos gráficos
    graphic_instructions = GRAPHICINST_free(graphic_instructions);
    graphic_cells = GRAPHICSCELLS_free(graphic_cells);
    graphic_select = GRAPHICSSELECT_free(graphic_select);
    graphic_user = GRAPHICUSER_free(graphic_user);

}
