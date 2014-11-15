/**
 * \file functions.h
 * Arquivo que descreve as funções que o usuário pode usar na planilha
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Estrutura de uma lista de valores
 */
typedef struct listDouble ListDouble;

/**
 * Lista funções disponíveis para o usuário
 */
void FUNCTIONS_listFunctions();

/**
 * Cria a lista de doubles
 * \return NULL
 */
ListDouble* FUNCTIONS_createList();

/**
 * Desaloca memória da lista de doubles
 * \return NULL
 * \param list Ponteiro para a lista de doubles
 */
ListDouble* FUNCTIONS_free(ListDouble* list);

/**
 * Adiciona valor na lista de doubles
 * \return Ponteiro para a lista de double
 * \param list Ponteiro para a lista de doubles
 * \param value Valor a ser adicionado na lista
 */
ListDouble* FUNCTIONS_addValue(ListDouble* list, double value);

/**
 * Executa uma função do módulo
 * \return O resultado da execução da função na lista fornecida
 * \param function string literal do nome da função
 * \param list Ponteiro duplo para lista de doubles
 */
double FUNCTIONS_evalFunction(const char* function, ListDouble** list);

#endif /* FUNCTIONS_H_ */
