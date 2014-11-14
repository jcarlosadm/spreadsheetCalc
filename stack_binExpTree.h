/**
 * \file stack_binExpTree.h
 * Define uma pilha de árvores de expressão binária
 */

#ifndef STACK_BINEXPTREE_H_
#define STACK_BINEXPTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "binary_expression_tree.h"

/**
 * Estrutura da pilha de árvores de expressão binária
 */
typedef struct stackBinExpTree StackBinExpTree;

/**
 * Cria uma pilha de árvores de expressão binária
 * \return Ponteiro para StackBinTree, ou NULL em caso de falha de alocação
 */
StackBinExpTree* STACKBINEXPTREE_create();

/**
 * Libera a memória alocada na pilha
 * \return NULL
 * \param stack Ponteiro para StackBinExpTree
 */
StackBinExpTree* STACKBINEXPTREE_free(StackBinExpTree* stack);

/**
 * Adiciona uma árvore de expressão binária (que conterá o valor fornecido) na pilha
 * \return 1 em caso de sucesso, ou 0 em caso de falha
 * \param stack Ponteiro duplo para StackBinExpTree
 * \param value Valor a ser adicionado na árvore
 */
int STACKBINEXPTREE_pushValue(StackBinExpTree** stack, double value);

/**
 * Adiciona uma árvore de expressão binária (que conterá o símbolo fornecido)
 * na pilha. Essa função retira duas outras árvores do topo e as "junta" abaixo
 * desta nova, uma à esquerda e outra à direita.
 * \return 1 em caso de sucesso, 0 em caso de falha
 * \param stack Ponteiro duplo para StackBinExpTree
 * \param symbol Símbolo a ser adicionado no nó do topo da árvore a ser adicionada na pilha
 */
int STACKBINEXPTREE_pushSymbol(StackBinExpTree** stack, char symbol);

/**
 * Calcula expressão da árvore do topo, remove essa árvore do topo e retorna o resultado
 * \return O resultado da expressão do topo da pilha
 * \param stack Ponteiro duplo para StackBinExpTree
 */
double STACKBINEXPTREE_pop(StackBinExpTree** stack);

#endif /* STACK_BINEXPTREE_H_ */
