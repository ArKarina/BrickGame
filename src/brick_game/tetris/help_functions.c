/**
 * @file help_functions.c
 * @brief Файл содержит реализацию вспомогательных функций работы с памятью.
 */
#include "help_functions.h"

int **create_matrix(int rows, int cols) {
  if (rows <= 0 || cols <= 0) {
    return NULL;
  }

  int **matrix = calloc(rows, sizeof(int *));
  if (matrix) {
    int *data = calloc(rows * cols, sizeof(int));
    if (data) {
      for (int i = 0; i < rows; i++) {
        matrix[i] = data + i * cols;
      }
    } else {
      free(matrix);
    }
  }

  return matrix;
}

void free_matrix(int **matrix) {
  if (matrix) {
    free(matrix[0]);
    free(matrix);
    matrix = NULL;
  }
}
