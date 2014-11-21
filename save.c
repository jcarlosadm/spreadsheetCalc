/**
 * \file save.c
 * Implementação do arquivo save.h
 */

#include "save.h"

// nome do nó principal
#define MAIN_NODE "data"

/******************************************************************************
 * Estruturas
 ******************************************************************************/

/**
 * Estrutura do arquivo de save
 */
struct saveFile{
    mxml_node_t* tree;
    mxml_node_t* firstNode;
};

/******************************************************************************
 * Funções privadas
 ******************************************************************************/



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
