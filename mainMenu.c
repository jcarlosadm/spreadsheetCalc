/**
 * \file mainMenu.c
 * Implementação do arquivo mainMenu.h
 */

#include "mainMenu.h"

// Largura total da janela
#define WINDOW_WIDTH 130
// Altura total da janela
#define WINDOW_HEIGHT 40

// Posicionamento e dimensões da tela de instruções
#define WINDOW_INSTRUCTION_X 10
#define WINDOW_INSTRUCTION_Y 24
#define WINDOW_INSTRUCTION_WIDTH 90
#define WINDOW_INSTRUCTION_HEIGHT 12

// Posicionamento e dimensões da tela de opções
#define WINDOW_SELECT_X WINDOW_INSTRUCTION_X+WINDOW_INSTRUCTION_WIDTH
#define WINDOW_SELECT_Y WINDOW_INSTRUCTION_Y
#define WINDOW_SELECT_WIDTH WINDOW_WIDTH-WINDOW_INSTRUCTION_WIDTH
#define WINDOW_SELECT_HEIGHT 16

// opções do menu principal
#define OPTION_NEW_WORKSPACE "Novo espaco de trabalho"
#define OPTION_LOAD_DATA "Carregar espaco de trabalho"
#define OPTION_EXIT "Sair"

// opções YES/NO
#define YES "Sim"
#define NO "Nao"

/*****************************************************************************
 * Funções privadas
 *****************************************************************************/



/*****************************************************************************
 * Funções públicas
 *****************************************************************************/

/**
 * Executa o menu principal
 */
void MAINMENU_run(){

    // inicia tela
    initscr();
    curs_set(FALSE);
    clear();
    refresh();

    // variável que controla o loop principal da função
    int mainLoop = true;

    // variável que controla se dados foram carregados ou não
    int loaded;

    // variável que guarda espaço de trabalho
    char workspaceName[60];

    // variável que guarda opção escolhida
    char option[40];

    // ponteiros para componentes gráficos necessários
    // janela de instruções
    GraphicInstructions* graphic_instructions = NULL;
    // janela de seleção
    GraphicSelect* graphic_select = NULL;

    // ponteiro para a matriz de células
    Matrix* matrix = NULL;

    // loop principal da função
    while(mainLoop){

        // aloca janela de seleção
        graphic_select = GRAPHICSSELECT_create(WINDOW_WIDTH/2,
                WINDOW_SELECT_Y,WINDOW_SELECT_WIDTH, WINDOW_SELECT_HEIGHT);

        // prepara janela de seleção
        GRAPHICSSELECT_clearOptions(&graphic_select);
        GRAPHICSSELECT_addOption(&graphic_select, OPTION_NEW_WORKSPACE);
        if(LOAD_canLoad(SAVEFILE))
            GRAPHICSSELECT_addOption(&graphic_select, OPTION_LOAD_DATA);
        GRAPHICSSELECT_addOption(&graphic_select, OPTION_EXIT);

        // cria janela de instruções para conter título do programa
        graphic_instructions = GRAPHICINST_create(WINDOW_WIDTH/4,WINDOW_HEIGHT/4,
                WINDOW_INSTRUCTION_WIDTH,WINDOW_INSTRUCTION_HEIGHT/2);

        // coloca título
        GRAPHICINST_clear(&graphic_instructions);
        GRAPHICINST_write(&graphic_instructions, "SpreadsheetCalc",WINDOW_INSTRUCTION_WIDTH/2,
                WINDOW_INSTRUCTION_HEIGHT/4);

        // abre opções para o usuário escolher
        GRAPHICSSELECT_selectOption(&graphic_select, option);

        // libera janela de opções
        graphic_select = GRAPHICSSELECT_free(graphic_select);
        // libera janela de instruções
        graphic_instructions = GRAPHICINST_free(graphic_instructions);

        // verifica qual a opção escolhida
        // novo espaço de trabalho
        if(strcmp(option,OPTION_NEW_WORKSPACE)==0){
            // inicia módulo spreadsheet sem dados iniciais
            SPREADSHEET_run(NULL,NULL);
        }

        // carregar dados
        else if(strcmp(option, OPTION_LOAD_DATA)==0){
            // inicia matriz
            matrix = MATRIX_create(ROWS, COLUMNS);

            // inicia janelas
            graphic_instructions = GRAPHICINST_create(WINDOW_INSTRUCTION_X,WINDOW_INSTRUCTION_Y,
                    WINDOW_INSTRUCTION_WIDTH, WINDOW_INSTRUCTION_HEIGHT);
            graphic_select = GRAPHICSSELECT_create(WINDOW_SELECT_X, WINDOW_SELECT_Y,
                    WINDOW_SELECT_WIDTH, WINDOW_SELECT_HEIGHT);

            // tenta carregar dados
            loaded = LOAD_load(&matrix,&graphic_instructions,&graphic_select,SAVEFILE,workspaceName);

            // libera janelas
            graphic_instructions = GRAPHICINST_free(graphic_instructions);
            graphic_select = GRAPHICSSELECT_free(graphic_select);

            // se dados foram carregados com sucesso
            if(loaded){
                SPREADSHEET_run(&matrix,workspaceName);
            }
            // se dados não foram carregados com sucesso
            else{
                // libera matriz
                matrix = MATRIX_free(matrix);
            }
        }

        // sair
        else{
            sleep(2);
            mainLoop = false;
        }
    }

    // libera tela
    endwin();
}
