/**
 * \file load.c
 * Implementação do arquivo load.h
 */

#include "load.h"

// definição de linha e coluna
#define COLUMN 1
#define ROW 1

// definição dos valores sim e não
#define YES "Sim"
#define NO "Nao"

// definição do valor de cancelar
#define CANCEL "Cancelar"

/***********************************************************************
 * Funções privadas
 ***********************************************************************/

/**
 * Cria opções com base nos nomes dos espaços de trabalho
 * \param select Ponteiro para a tela de seleção
 * \param top Ponteiro para uma árvore xml
 */
void LOAD_makeWorkspaceOptions(GraphicSelect** select, mxml_node_t** top){
    if(!select || !(*select)) return;

    // pega o primeiro espaço de trabalho
    mxml_node_t* workspaceNode = mxmlWalkNext(*top,*top,MXML_DESCEND);
    workspaceNode = mxmlWalkNext(workspaceNode,*top,MXML_DESCEND);

    while(workspaceNode){
        // adiciona opção
        GRAPHICSSELECT_addOption(&(*select), mxmlGetElement(workspaceNode));

        workspaceNode = mxmlGetNextSibling(workspaceNode);
    }

    GRAPHICSSELECT_addOption(&(*select), CANCEL);
}

/**
 * Preenche dados na matriz de acordo com o nome do espaço de trabalho
 * \param Matrix Ponteiro para a matriz de células
 * \param top Ponteiro para árvore xml
 * \param workspaceName Nome do espaço de trabalho escolhido
 */
void LOAD_loadData(Matrix** matrix, mxml_node_t** top, const char* workspaceName){
    if(!matrix || !(*matrix)) return;

    // procura pelo nó com o nome do espaço de trabalho escolhido
    mxml_node_t* node = mxmlFindElement(*top, *top, workspaceName,
            NULL, NULL, MXML_DESCEND);

    // guarda dados da linha e coluna
    int row, column;

    // pega primeiro filho
    mxml_node_t* child = mxmlGetFirstChild(node);

    // enquanto child não é nulo, preenche dados na matriz
    while(child){
        // pega linha e coluna
        row = atoi(mxmlElementGetAttr(child, "row"));
        column = atoi(mxmlElementGetAttr(child, "column"));
        // preenche matriz
        MATRIX_setExpression(&(*matrix), row, column, mxmlElementGetAttr(child, "expression"),
                NULL, NULL);

        child = mxmlGetNextSibling(child);
    }
}

/***********************************************************************
 * Funções públicas
 ***********************************************************************/

/**
 * Verifica se existem dados para carregar
 * \return 0 se não existem dados, e diferente de 0 se existem
 * \param fileName Nome do arquivo
 */
int LOAD_canLoad(const char *fileName){
    if(!fileName) return 0;

    // verifica se arquivo existe
    FILE* file = fopen(fileName, "r");
    if(!file) return 0;

    // verifica se existem dados neste arquivo
    mxml_node_t* tree = mxmlLoadFile(NULL, file, MXML_TEXT_CALLBACK);
    if(!tree) {
        fclose(file);
        return 0;
    }

    fclose(file);

    mxml_node_t* node = mxmlWalkNext(tree, tree, MXML_DESCEND);
    if(!node){
        mxmlDelete(tree);
        return 0;
    }
    node = mxmlWalkNext(node, tree, MXML_DESCEND);
    if(!node){
        mxmlDelete(tree);
        return 0;
    }

    // existem dados que possam ser carregados
    mxmlDelete(tree);
    return 1;
}

/**
 * Carrega dados do arquivo para a matriz
 * \return 1 se obtiver sucesso em carregar dados, 0 em caso contrário
 * \param matrix Ponteiro para a matriz de células
 * \param instructions Ponteiro para a janela de instruções
 * \param select Ponteiro para a janela de seleção
 * \param fileName Nome do arquivo
 */
int LOAD_load(Matrix** matrix, GraphicInstructions** instructions,
        GraphicSelect** select, const char* fileName){
    if(!matrix || !(*matrix) || !instructions || !(*instructions)
            || !select || !(*select)) return 0;

    // variável que controla loop de decisão
    int continueLoop = true;

    // guarda a opção escolhida
    char workspaceName[30];
    char option[10];

    // carrega árvore de dados
    FILE* file = fopen(fileName, "r");
    mxml_node_t* tree = mxmlLoadFile(NULL, file, MXML_TEXT_CALLBACK);
    fclose(file);

    while(continueLoop){
        // gera opções com base nos nomes dos espaços de trabalho
        GRAPHICSSELECT_clearOptions(&(*select));
        LOAD_makeWorkspaceOptions(&(*select),&tree);

        // fala para o usuário escolher um espaço de trabalho
        GRAPHICINST_clear(&(*instructions));
        GRAPHICINST_write(&(*instructions), "Escolha um espaco de trabalho", COLUMN*1, ROW*1);
        GRAPHICINST_writeKeyboard(&(*instructions), COLUMN*1, ROW*2, false);

        // abre opções
        GRAPHICSSELECT_selectOption(&(*select), workspaceName);

        // se escolheu cancelar, retorna 0
        if(strcmp(workspaceName,CANCEL)==0){
            GRAPHICINST_clear(&(*instructions));
            GRAPHICSSELECT_clearOptions(&(*select));
            mxmlDelete(tree);
            return 0;
        }

        // caso contrário, pede confirmação da opção escolhida
        GRAPHICINST_clear(&(*instructions));
        GRAPHICINST_write(&(*instructions), "Confirmar escolha?", COLUMN*1, ROW*1);
        GRAPHICINST_writeKeyboard(&(*instructions), COLUMN*1, ROW*2, false);

        GRAPHICSSELECT_clearOptions(&(*select));
        GRAPHICSSELECT_addOption(&(*select), YES);
        GRAPHICSSELECT_addOption(&(*select), NO);

        GRAPHICSSELECT_selectOption(&(*select), option);

        // escolheu sim
        if(strcmp(option, YES)==0){
            LOAD_loadData(&(*matrix), &tree, workspaceName);

            GRAPHICINST_clear(&(*instructions));
            GRAPHICINST_write(&(*instructions), "Dados carregados.", COLUMN*1, ROW*1);
            sleep(2);

            continueLoop = false;
        }
    }

    GRAPHICINST_clear(&(*instructions));
    GRAPHICSSELECT_clearOptions(&(*select));
    mxmlDelete(tree);
    return 1;

}
