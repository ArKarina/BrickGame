/**
 * \mainpage
 * \author leytonta
 * \version 1.0
 *
 * \section intro_sec Tetris
 * Проект создан при помощи:
 * 1. C language
 * 2. ncurses
 * 3. Doxygen
 *
 * \section brief_sec Brief
 * BrickGame — популярная портативная консоль 90-ых годов с несколькими тысячами
 * встроенными играми, разработанная в Китае. Изначально была копией
 * разработанной в СССР и выпущенной Nintendo в рамках платформы GameBoy игры
 * «Тетрис», но включала в себя также и множество других игр, которые
 * добавлялись с течением времени. Консоль имела небольшой экранчик с игровым
 * полем размера 10х20, представляющим из себя матрицу «пикселей». Справа от
 * поля находилось табло с цифровой индикацией состояния текущей игры, рекордами
 * и прочей дополнительной информацией. Самыми распространенными играми на
 * BrickGame были: тетрис, танки, гонки, фроггер и змейка.
 *
 * В данном проекте реализована самая популярная игра из серии BrickGame -
 * Tetris.
 *
 */

/**
 * \file main.c
 * \brief Основной файл, содержит точку входа в программу.
 */

#include <time.h>

#include "brick_game/tetris/brick_game.h"
#include "gui/cli/cli.h"

/**
 * \brief Главная функция. Точка входа в программу.
 */
int main(void) {
  srand(time(NULL));
  logicGame();

  return 0;
}
