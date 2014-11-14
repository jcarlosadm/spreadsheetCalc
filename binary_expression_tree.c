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
    char symbol; ///< símbolo do nó. Símbolo 'v' identifica um valor
    double value;
    BinaryExpTree* left;
    BinaryExpTree* right;
};

/**********************************************************************
 * Funções privadas
 **********************************************************************/

/**
 * Calcula recursivamente o valor de uma árvore de expressão binária
 */
void BINARYEXPTREE_evalRecursive(BinaryExpTree** binaryExpTree, StackDouble** stack){

    // percorre de maneira posfixa
    if((*binaryExpTree)->left)
        BINARYEXPTREE_evalRecursive(&(*binaryExpTree)->left, &(*stack));
    if((*binaryExpTree)->right)
        BINARYEXPTREE_evalRecursive(&(*binaryExpTree)->right, &(*stack));

    // preenche pilha
    if((*binaryExpTree)->symbol=='v'){
        STACKDOUBLE_push(&(*stack), (*binaryExpTree)->value);
    }
    else{
        double value2 = STACKDOUBLE_pop(&(*stack));
        double value1 = STACKDOUBLE_pop(&(*stack));

        switch((*binaryExpTree)->symbol){
        case '+':
            STACKDOUBLE_push(&(*stack), value1+value2);
            break;
        case '-':
            STACKDOUBLE_push(&(*stack), value1-value2);
            break;
        case '*':
            STACKDOUBLE_push(&(*stack), value1*value2);
            break;
        case '/':
            if(value2!=0)
                STACKDOUBLE_push(&(*stack), value1/value2);
            else
                STACKDOUBLE_push(&(*stack), value1);
            break;
        default:
            STACKDOUBLE_push(&(*stack), value1);
        }
    }
}

/**********************************************************************
 * Funções públicas
 **********************************************************************/

/**
 * Cria uma árvore com apenas um nó, com filhos nulos
 * \return Ponteiro para BinaryExpTree, ou NULL em caso de falha de alocação
 * \param value Valor double guardado no único nó da árvore
 */
BinaryExpTree* BINARYEXPTREE_create(double value){
    BinaryExpTree* binaryExpTree = malloc(sizeof(BinaryExpTree));
    if(!binaryExpTree) return NULL;

    binaryExpTree->value = value;
    binaryExpTree->symbol = 'v';
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

/**
 * Junta duas árvores de expressão binária sob um nó de valor específico
 * \return Ponteiro para uma árvore de expressão binária, ou NULL em caso de falha de alocação
 * \param left Ponteiro para a árvore a ser colocada como filho esquerdo da nova árvore
 * \param right Ponteiro para a árvore a ser colocada como filho direito da nova árvore
 * \param symbol Caractere do nó pai das duas árvores
 */
BinaryExpTree* BINARYEXPTREE_joinTrees(BinaryExpTree* left, BinaryExpTree* right,
        char symbol){
    BinaryExpTree* binaryExpTree = malloc(sizeof(BinaryExpTree));
    if(!binaryExpTree) return NULL;

    binaryExpTree->symbol = symbol;
    binaryExpTree->value = 0;
    binaryExpTree->left = left;
    binaryExpTree->right = right;

    return binaryExpTree;
}

/**
 * Calcula expressão da árvore
 * \return O resultado da expressão
 * \param binaryExpTree Ponteiro para a árvore de expressão binária
 */
double BINARYEXPTREE_evalExpression(BinaryExpTree* binaryExpTree){
    if(!binaryExpTree) return 0;

    StackDouble* stack = STACKDOUBLE_create();

    BINARYEXPTREE_evalRecursive(&binaryExpTree, &stack);

    double value = STACKDOUBLE_pop(&stack);
    stack = STACKDOUBLE_free(stack);

    return value;
}
