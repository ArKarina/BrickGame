#ifndef BRICK_GAME_H
#define BRICK_GAME_H

/**
 * @file brick_game.h
 * @brief Файл содержит объявление функций основной логики игры.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "help_functions.h"

/**
 * @brief Ширина игрового поля
 */
#define FIELD_WIDTH 10
/**
 * @brief Высота игрового поля
 */
#define FIELD_HEIGHT 20

/**
 * @brief Количество фигур
 */
#define FIGURES_N 7
/**
 * @brief Количество блоков в фигуре
 */
#define BLOCKS_N 4

/**
 * @brief Начальная скорость
 */
#define SPEED 1000
/**
 * @brief Дельта скорости
 */
#define DELTA_SPEED 100
/**
 * @brief Максимальный уровень
 */
#define MAX_LEVEL 10

/**
 * @brief Файл для хранения рекорда
 */
#define HIGH_SCORE_FILE "high_score.txt"

/**
 * @brief Тип пользовательского ввода
 */
typedef enum {
  Start,      ///< начало игры
  Pause,      ///< пауза
  Terminate,  ///< завершение игры
  Left,       ///< движение фигуры влево
  Right,      ///< движение фигуры вправо
  Up,         ///< не используется
  Down,  ///< быстрый спуск фигуры вниз до столкновения
  Action  ///< вращение фигуры
} UserAction_t;

/**
 * @brief Структура, содержащая информацию о состоянии игры
 * @param field игровое поле
 * @param next следующая фигура
 * @param score текущий счет
 * @param high_score рекорд
 * @param level текущий уровень
 * @param speed текущая скорость
 * @param pause флаг паузы
 */
typedef struct {
  int **field;     ///< игровое поле
  int **next;      ///< следующая фигура
  int score;       ///< текущий счет
  int high_score;  ///< рекорд
  int level;       ///< текущий уровень
  int speed;       ///< текущая скорость
  int pause;       ///< флаг паузы
} GameInfo_t;

/**
 * @brief Структура, содержащая информацию о текущей фигуре
 * @param figure текущая фигура
 * @param y координата y
 * @param x координата x
 */
typedef struct {
  int figure[BLOCKS_N][BLOCKS_N];  ///< текущая фигура
  int y;                           ///< координата y
  int x;                           ///< координата x
} Figure_t;

/**
 * @brief Функция инициализации игры
 */
void initGame();
/**
 * @brief Функция завершения игры
 */
void terminateGame();

/**
 * @brief Функция обработки пользовательского ввода
 * @param action пользовательский ввод
 * @param hold флаг удерживания клавиши
 */
void userInput(UserAction_t action, bool hold);
/**
 * @brief Функция обновления состояния игры
 */
GameInfo_t updateCurrentState();

/**
 * @brief Функция очистки линий
 */
void clearLines();
/**
 * @brief Функция добавления очков
 * @param lines количество вычернутых линий
 */
void addScore(int lines);
/**
 * @brief Функция изменения уровня
 */
void changeLevel();

/**
 * @brief Функция генерации следующей фигуры
 */
void generateNextFigure();
/**
 * @brief Функция получения следующей фигуры
 */
void getNextFigure();
/**
 * @brief Функция добавления текущей фигуры
 */
void addFigure();
/**
 * @brief Функция удаления текущей фигуры
 */
void deleteFigure();

/**
 * @brief Функция перемещения текущей фигуры вниз
 * @return флаг успешности перемещения
 */
bool moveFigureDown();
/**
 * @brief Функция перемещения текущей фигуры вниз до столкновения
 */
void moveFigureDownFull();
/**
 * @brief Функция перемещения текущей фигуры влево
 * @return флаг успешности перемещения
 */
bool moveFigureLeft();
/**
 * @brief Функция перемещения текущей фигуры вправо
 * @return флаг успешности перемещения
 */
bool moveFigureRight();
/**
 * @brief Функция вращения текущей фигуры
 * @return флаг успешности вращения
 */
bool rotateFigure();
/**
 * @brief Функция вращения текущей фигуры вправо
 */
void rotateFigureRight();
/**
 * @brief Функция вращения текущей фигуры влево
 */
void rotateFigureLeft();

/**
 * @brief Функция проверки завершения игры
 * @return флаг завершения
 */
bool isGameOver();
/**
 * @brief Функция проверки победы
 * @return флаг победы
 */
bool playerWin();
/**
 * @brief Функция проверки возможности размещения текущей фигуры
 * @return флаг возможности размещения фигуры
 */
bool canPutFigure();

/**
 * @brief Функция сохранения рекорда
 */
void saveHighScore();
/**
 * @brief Функция загрузки рекорда
 */
void loadHighScore();

// функции получения информации об игре (используются для тестов)
int getScore();
int getHighScore();
int getLevel();
void setLevel(int level);
int getSpeed();
int getFigureX();
int getFigureY();
void setFigureElem(int i, int j);
int getFigureElem(int i, int j);
void setFieldElem(int i, int j);
int getFieldElem(int i, int j);
void runGame();
bool isPaused();

#endif  // BRICK_GAME_H
