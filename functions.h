/**
 * \file functions.h
 * Arquivo que descreve as funções que o usuário pode usar na planilha
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdio.h>

/**
 * Lista funções disponíveis para o usuário
 */
void FUNCTIONS_listFunctions();

/**
 * Calcula a soma de uma lista de valores
 * \return Retorna a soma
 * \param values Ponteiro para o começo de um array de doubles
 * \param size Tamanho do array de doubles
 */
double FUNCTIONS_sum(double *values, int size);

/**
 * Calcula a média de uma lista de valores
 * \return Retorna a média
 * \param values Ponteiro para o começo de um array de doubles
 * \param size Tamanho do array de doubles
 */
double FUNCTIONS_mean(double *values, int size);

/**
 * Procura pelo maior valor de uma lista de valores
 * \return Retorna o maior valor
 * \param values Ponteiro para o começo de um array de doubles
 * \param size Tamanho do array de doubles
 */
double FUNCTIONS_max(double *values, int size);

/**
 * Procura pelo menor valor de uma lista de valores
 * \return Retorna o menor valor
 * \param values Ponteiro para o começo de um array de doubles
 * \param size Tamanho do array de doubles
 */
double FUNCTIONS_min(double *values, int size);

#endif /* FUNCTIONS_H_ */
