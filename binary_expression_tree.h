/*
 * \file binary_expression_tree.h
 * Arquivo que descreve as funcionalidades de uma árvore de expressão binária
 */

#ifndef BINARY_EXPRESSION_TREE_H_
#define BINARY_EXPRESSION_TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "stack_double.h"

/**
 * Estrutura da árvore de expressão binária
 */
typedef struct binaryExpTree BinaryExpTree;

/**
 * Cria uma árvore com apenas um nó, com filhos nulos
 * \return Ponteiro para BinaryExpTree
 * \param value Caractere guardado no único nó da árvore
 */
BinaryExpTree* BINARYEXPTREE_create(char value);

/**
 * Libera memória da árvore de expressão binária
 * \return Retorna NULL
 * \param binaryExpTree Ponteiro para uma árvore de expressão binária
 */
BinaryExpTree* BINARYEXPTREE_free(BinaryExpTree* binaryExpTree);

/**
 * Junta duas árvores de expressão binária sob um nó de valor específico
 * \return Ponteiro para uma árvore de expressão binária
 * \param left Ponteiro para a árvore a ser colocada como filho esquerdo da nova árvore
 * \param right Ponteiro para a árvore a ser colocada como filho direito da nova árvore
 * \param value Valor do nó pai das duas árvores
 */
BinaryExpTree* BINARYEXPTREE_joinTrees(BinaryExpTree* left, BinaryExpTree* right,
        char value);

/**
 * Calcula expressão da árvore
 * \return O resultado da expressão
 * \param binaryExpTree Ponteiro para a árvore de expressão binária
 */
double BINARYEXPTREE_evalExpression(BinaryExpTree* binaryExpTree);

#endif /* BINARY_EXPRESSION_TREE_H_ */
