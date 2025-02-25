#ifndef CLI_H
#define CLI_H

/**
 * @file cli.h
 * @brief Файл содержит объявление функций отрисовки игры.
 */

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "../../brick_game/tetris/brick_game.h"

/**
 * @brief Код клавиши Enter
 */
#define ENTER_KEY 10  // ascii code
/**
 * @brief Высота информационного окна
 */
#define INFO_HEIGHT 10

/**
 * @brief Функция инициализации терминального интерфейса
 */
void initCli();
/**
 * @brief Функция отрисовки прямоугольного окна
 * @param top_y верхняя координата окна
 * @param bottom_y нижняя координата окна
 * @param left_x левая координата окна
 * @param right_x правая координата окна
 */
void printRectangle(int top_y, int bottom_y, int left_x, int right_x);
/**
 * @brief Функция отрисовки текущей фигуры
 * @param y координата y
 * @param x координата x
 * @param game структура состояния игры
 */
void printFigure(int y, int x, GameInfo_t game);
/**
 * @brief Функция отрисовки информационного окна
 * @param game структура состояния игры
 */
void printGameInfo(GameInfo_t game);
/**
 * @brief Функция отрисовки игры
 */
void renderGame(GameInfo_t game);
/**
 * @brief Функция отрисовки сообщения начала игры
 */
void printStartGame();
/**
 * @brief Функция отрисовки сообщения конца игры
 * @param winFlag флаг победы
 */
void printEndGame(int winFlag);
/**
 * @brief Функция, выполняющая логику игры
 */
void logicGame();
/**
 * @brief Функция считывания пользовательского ввода
 * @return тип пользовательсского ввода
 */
UserAction_t getKey();

/**
 * @brief Функция получения текущего времени в миллисекундах
 */
long getCurrentTimeMillis();

#endif  // CLI_H
