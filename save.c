/**
 * \file save.c
 * Implementação do arquivo save.h
 */

#include "save.h"

// nome do nó principal
#define MAIN_NODE "data"

// Define nome das opções SIM e NÂO
#define YES "Sim"
#define NO "Nao"

// define comprimento da linha e coluna
#define ROW 1
#define COLUMN 1

/******************************************************************************
 * Estruturas
 ******************************************************************************/

/**
 * Estrutura do arquivo de save
 */
struct saveFile{
    mxml_node_t* tree;
    mxml_node_t* firstNode;

    char fileName[60];
    char workspace[60];
};

/******************************************************************************
 * Funções privadas
 ******************************************************************************/

/**
 * Imprime, na tela de listagem, todos os espaços de trabalho salvos (até o limite da tela)
 * \param save Ponteiro para SaveFile
 * \param windowList Ponteiro para a tela de instruções
 */
void SAVE_listWorkSpaces(SaveFile** save,GraphicInstructions** windowList){
    if(!save || !(*save) || !windowList || !(*windowList)) return;

    // limpa tela de listagem
    GRAPHICINST_clear(&(*windowList));

    // pega referência para o primeiro espaço de trabalho
    mxml_node_t* node =  mxmlWalkNext((*save)->firstNode, (*save)->tree, MXML_DESCEND);

    // guarda posição y para imprimir na tela de listagem (posição inicial 3)
    int positionY = ROW*3;
    // guarda posição y da borda inferior da tela de listagem
    int limitWindowList = GRAPHICINST_getPositionY(&(*windowList))
            + GRAPHICINST_getHeight(&(*windowList));

    // imforma título
    GRAPHICINST_write(&(*windowList),"espacos de trabalho atualmente salvos",COLUMN*1,ROW*1);

    // enquanto node não nulo e não atingiu o limite da tela, imprime
    while(node && positionY <= limitWindowList - ROW*3){
        GRAPHICINST_write(&(*windowList),mxmlGetElement(node),COLUMN*1,positionY);
        GRAPHICINST_write(&(*windowList),mxmlElementGetAttr(node,"date"),COLUMN*3,positionY+ROW*1);

        positionY+=ROW*2;
        node = mxmlGetNextSibling(node);
    }
}

/**
 * Verifica se espaço de trabalho especificado existe
 * \return 1 se existir, 0 em caso contrário
 * \param save Ponteiro para SaveFile
 * \param workspaceName Nome do espaço de trabalho
 */
int SAVE_workspaceExist(SaveFile** save, const char* workspaceName){
    if(!save || !(*save)) return 0;

    return mxmlFindElement((*save)->tree,(*save)->tree,workspaceName,NULL,NULL,MXML_DESCEND)? 1:0;
}

/******************************************************************************
 * Funções públicas
 ******************************************************************************/

/**
 * Carrega arquivo de save e aloca memória. Cria arquivo se o mesmo não existir.
 * \return Ponteiro para SaveFile
 * \param fileName Nome do arquivo onde estão os dados
 */
SaveFile* SAVE_create(const char *fileName){
    SaveFile* save = malloc(sizeof(SaveFile));
    if(!save) return NULL;

    save->tree = NULL;

    // tenta abrir arquivo
    FILE* file = fopen(fileName,"r");
    // se não conseguir ou arquivo não existe, cria árvore nova
    if(!file){
        save->tree = mxmlNewXML("1.0");
        if(!save->tree){
            free(save);
            return NULL;
        }

        save->firstNode = mxmlNewElement(save->tree,MAIN_NODE);
    }
    // se conseguir abrir o arquivo
    else{
        save->tree = mxmlLoadFile(NULL, file, MXML_TEXT_CALLBACK);
        fclose(file);

        if(!save->tree){
            free(save);
            return NULL;
        }

        save->firstNode = mxmlWalkNext(save->tree, save->tree, MXML_DESCEND);
    }

    // guarda o nome do arquivo
    strcpy(save->fileName,fileName);

    // guarda o nome do espaço de trabalho como uma string vazia
    strcpy(save->workspace,"");

    return save;
}

/**
 * Libera memória alocada no objeto SaveFile
 * \return NULL
 * \param save Ponteiro para SaveFile
 */
SaveFile* SAVE_free(SaveFile* save){
    if(!save) return save;

    mxmlDelete(save->tree);
    save->firstNode = NULL;
    save->tree = NULL;

    free(save);

    return NULL;
}

/**
 * Define nome do espaço de trabalho ou abre interface com o usuário para definir
 * \param save Ponteiro para SaveFile
 * \param window_instructions Ponteiro para a janela de intruções. Pode informar
 * NULL caso workspaceName seja diferente de NULL
 * \param window_list Ponteiro para a janela que listará espaços salvos até o momento.
 * Pode informar NULL caso workspaceName seja diferente de NULL
 * \param window_user Ponteiro para a janela que recebe texto digitado pelo usuário.
 * Pode informar NULL caso workspaceName seja diferente de NULL
 * \param window_select Ponteiro para a janela de opções. Pode informar
 * NULL caso workspaceName seja diferente de NULL
 * \param workspaceName Nome do espaço de trabalho a ser salvo. Informe NULL caso
 * o usuário precise definir esse nome
 */
void SAVE_defineWorkspace(SaveFile** save, GraphicInstructions** window_instructions,
        GraphicInstructions** window_list, GraphicUser** window_user,
        GraphicSelect** window_select, const char* workspaceName){
    if(!save || !(*save)) return;

    // se foi definido um nome de espaço de trabalho, guarda esse nome
    if(workspaceName!=NULL){
        strcpy((*save)->workspace,workspaceName);
        return;
    }

    // verifica se os ponteiros para as telas não são nulos
    if(!window_instructions || !(*window_instructions) || !window_list
            || !(*window_list) || !window_user || !(*window_user)
            || !window_select || !(*window_select))
        return;

    // lista todos os espaços de trabalho presentes na árvore
    SAVE_listWorkSpaces(&(*save),&(*window_list));

    // guardará o nome do espaço de trabalho
    char workspaceTempName[60];

    // variável que controla loop da decisão de reescrita do usuário
    int continueLoop = true;

    // limpa opções e adiciona SIM e NÃO
    GRAPHICSSELECT_clearOptions(&(*window_select));
    GRAPHICSSELECT_addOption(&(*window_select),YES);
    GRAPHICSSELECT_addOption(&(*window_select),NO);
    // guarda decisão da tela de opções
    char option[10];

    // enquanto não sobrescreve...
    while(continueLoop){

        // pede para o usuário digitar um nome para o espaço de trabalho que quer salvar
        GRAPHICINST_clear(&(*window_instructions));
        GRAPHICINST_write(&(*window_instructions),
                "Digite o nome do espaco de trabalho que deseja salvar:",COLUMN*1,ROW*1);

        // inicializa janela para digitação do usuário
        GRAPHICUSER_clear(&(*window_user));
        GRAPHICUSER_get(&(*window_user),workspaceTempName,COLUMN*1,ROW*1);

        // verifica se o nome digitado já existe
        // se existir, pergunta ao usuário se deve sobrescrever os dados
        if(SAVE_workspaceExist(&(*save),workspaceTempName)){
            GRAPHICINST_clear(&(*window_instructions));
            GRAPHICINST_write(&(*window_instructions),"nome ja existe. sobrescrever?",COLUMN*1,ROW*1);
            GRAPHICINST_writeKeyboard(&(*window_instructions),COLUMN*1,ROW*2,false);

            // abre opções
            GRAPHICSSELECT_selectOption(&(*window_select),option);

            // verifica qual opção escolhida
            //sim
            if(strcmp(option,YES)==0){
                strcpy((*save)->workspace, workspaceTempName);

                GRAPHICINST_clear(&(*window_instructions));
                GRAPHICINST_write(&(*window_instructions),"nome definido com sucesso!",COLUMN*1,
                        ROW*1);
                sleep(2);

                // quebra o loop
                continueLoop = false;
            }

        }
        // se não existe, guarda e informa
        else{
            strcpy((*save)->workspace, workspaceTempName);

            GRAPHICINST_clear(&(*window_instructions));
            GRAPHICINST_write(&(*window_instructions),"nome definido com sucesso!",COLUMN*1,ROW*1);
            sleep(2);

            continueLoop = false;
        }
    }

    // limpa todas as janelas
    GRAPHICINST_clear(&(*window_instructions));
    GRAPHICINST_clear(&(*window_list));
    GRAPHICUSER_clear(&(*window_user));
    GRAPHICSSELECT_clearOptions(&(*window_select));
}

/**
 * Verifica se o espaço de trabalho ainda não foi definido
 * \return Um valor diferente de 0 caso ainda não tenha sido. 0 caso contrário
 * \param save Ponteiro para SaveFile
 */
int SAVE_workspaceIsNULL(SaveFile** save){
    if(!save || !(*save)) return 0;

    return (strcmp((*save)->workspace, "")==0);
}
