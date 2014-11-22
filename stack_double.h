/**
 * \file stack_double.h
 * Pilha que guarda valores do tipo double
 */

#ifndef STACK_DOUBLE_H_
#define STACK_DOUBLE_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * Estrutura da pilha de doubles
 */
typedef struct stackDouble StackDouble;

/**
 * Aloca pilha
 * \return Retorna ponteiro para a memória alocada, ou NULL em caso de falha
 */
StackDouble* STACKDOUBLE_create();

/**
 * Desaloca pilha
 * \return Retorna NULL
 * \param stackDouble Ponteiro para StackDouble
 */
StackDouble* STACKDOUBLE_free(StackDouble* stackDouble);

/**
 * Adiciona valor double na pilha
 * \return Retorna 1 em caso de sucesso ou 0 em caso de falha (em
 * geral, problemas de alocação)
 * \param stackDouble Ponteiro duplo para StackDouble
 * \param value Valor a ser adicionado na pilha
 */
int STACKDOUBLE_push(StackDouble** stackDouble, double value);

/**
 * Retira elemento do topo e retorna valor desse elemento
 * \return Valor do elemento removido do topo da pilha
 * \param stackDouble Ponteiro duplo para StackDouble
 */
double STACKDOUBLE_pop(StackDouble** stackDouble);

/**
 * Pega valor do elemento do topo da pilha
 * \return Retorna valor do elemento do topo
 * \param stackDouble Ponteiro duplo para StackDouble
 */
double STACKDOUBLE_peek(StackDouble** stackDouble);

/**
 * Verifica se a pilha se encontra vazia
 * \return Um valor diferente de 0 se estiver vazia, ou 0 caso não esteja vazia
 * \param stackDouble Ponteiro duplo para StackDouble
 */
int STACKDOUBLE_isEmpty(StackDouble** stackDouble);

#endif /* STACK_DOUBLE_H_ */
