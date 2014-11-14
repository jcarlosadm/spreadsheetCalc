/*
 * \file binary_expression_tree.c
 * Arquivo que implementa binary_expression_tree.h
 */

#include "binary_expression_tree.h"

/**********************************************************************
 * Estruturas
 **********************************************************************/

/**
 * Estrutura da árvore de expressão binária
 */
struct binaryExpTree{
    char value;
    BinaryExpTree* left;
    BinaryExpTree* right;
};

/**********************************************************************
 * Funções privadas
 **********************************************************************/



/**********************************************************************
 * Funções públicas
 **********************************************************************/

/**
 * Cria uma árvore com apenas um nó, com filhos nulos
 * \return Ponteiro para BinaryExpTree
 * \param value Caractere guardado no único nó da árvore
 */
BinaryExpTree* BINARYEXPTREE_create(char value){
    BinaryExpTree* binaryExpTree = malloc(sizeof(BinaryExpTree));
    if(!binaryExpTree) return NULL;

    binaryExpTree->value = value;
    binaryExpTree->left = NULL;
    binaryExpTree->right = NULL;

    return binaryExpTree;
}

/**
 * Libera memória da árvore de expressão binária
 * \return Retorna NULL
 * \param binaryExpTree Ponteiro para uma árvore de expressão binária
 */
BinaryExpTree* BINARYEXPTREE_free(BinaryExpTree* binaryExpTree){
    if(!binaryExpTree) return NULL;

    if(binaryExpTree->left)
        binaryExpTree->left = BINARYEXPTREE_free(binaryExpTree->left);
    if(binaryExpTree->right)
        binaryExpTree->right = BINARYEXPTREE_free(binaryExpTree->right);

    free(binaryExpTree);
    return NULL;
}

