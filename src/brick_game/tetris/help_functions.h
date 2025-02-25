#ifndef HELP_FUNCTIONS_H
#define HELP_FUNCTIONS_H

/**
 * @file help_functions.h
 * @brief Файл содержит объявление вспомогательных функций работы с памятью.
 */
#include <stdlib.h>

/**
 * @brief Функция создания двумерного массива
 * @param rows количество строк
 * @param cols количество столбцов
 * @return указатель на двумерный массив
 */
int **create_matrix(int rows, int cols);
/**
 * @brief Функция освобождения двумерного массива
 * @param matrix указатель на двумерный массив
 */
void free_matrix(int **matrix);

#endif  // HELP_FUNCTIONS_H
