#include <check.h>

#include "../tetris/brick_game.h"

// Тестируем инициализацию игры
START_TEST(test_initGame) {
  initGame();
  ck_assert_int_eq(getScore(), 0);
  ck_assert_int_eq(getLevel(), 1);
  ck_assert_int_eq(getSpeed(), SPEED);
}
END_TEST

// Тестируем завершение игры
START_TEST(test_terminateGame) {
  initGame();
  terminateGame();
  ck_assert_int_eq(getScore(), 0);
  ck_assert_int_eq(getHighScore(), 0);
  ck_assert_int_eq(getLevel(), 1);
  ck_assert_int_eq(getSpeed(), SPEED);
}
END_TEST

// Тестируем добавление очков
START_TEST(test_addScore) {
  initGame();
  addScore(1);
  ck_assert_int_eq(getScore(), 100);
  addScore(2);
  ck_assert_int_eq(getScore(), 400);
  addScore(3);
  ck_assert_int_eq(getScore(), 1100);
  addScore(4);
  ck_assert_int_eq(getScore(), 2600);

  terminateGame();
}
END_TEST

// Тестируем обновление уровня
START_TEST(test_changeLevel) {
  initGame();
  addScore(4);
  ck_assert_int_eq(getLevel(), 3);

  terminateGame();
}
END_TEST

// Тестируем завершение игры
START_TEST(test_endGame) {
  initGame();
  setLevel(11);
  ck_assert_int_eq(getLevel(), 11);
  ck_assert_int_eq(playerWin(), true);
  ck_assert_int_eq(isGameOver(), true);

  terminateGame();
}
END_TEST

// Тестируем очистку линий со сдвигом
START_TEST(test_clearLines) {
  initGame();

  for (int x = 0; x < FIELD_WIDTH; x++) {
    setFieldElem(0, x);
    setFieldElem(1, x);
  }

  clearLines();
  for (int x = 0; x < FIELD_WIDTH; x++) {
    ck_assert_int_eq(getFieldElem(0, x), 0);
    ck_assert_int_eq(getFieldElem(1, x), 0);
  }

  terminateGame();
}
END_TEST

// Тестируем сдвиг фигуры вниз
START_TEST(test_moveFigureDown) {
  initGame();

  getNextFigure();

  bool moved = moveFigureDown();
  ck_assert(moved);
  ck_assert_int_eq(getFigureY(), 0);

  terminateGame();
}
END_TEST

// Тестируем полный сдвиг фигуры вниз
START_TEST(test_moveFigureDownFull) {
  initGame();

  getNextFigure();
  moveFigureDownFull();

  ck_assert_int_eq(getFigureY(), FIELD_HEIGHT - BLOCKS_N + 1);

  terminateGame();
}
END_TEST

// Тестируем сдвиг фигуры
START_TEST(test_moveFigureLeft_and_Right) {
  initGame();

  getNextFigure();

  bool movedLeft = moveFigureLeft();
  ck_assert(movedLeft);
  ck_assert_int_eq(getFigureX(), FIELD_WIDTH / 2 - 3);

  bool movedRight = moveFigureRight();
  ck_assert(movedRight);
  ck_assert_int_eq(getFigureX(), FIELD_WIDTH / 2 - 2);

  terminateGame();
}
END_TEST

// Тестируем вращение фигуры
START_TEST(test_rotateFigure) {
  initGame();

  setFigureElem(1, 0);
  setFigureElem(1, 1);
  setFigureElem(1, 2);

  rotateFigureRight();
  ck_assert_int_eq(getFigureElem(0, 2), 1);
  ck_assert_int_eq(getFigureElem(1, 2), 1);
  ck_assert_int_eq(getFigureElem(2, 2), 1);

  rotateFigureLeft();
  ck_assert_int_eq(getFigureElem(1, 0), 1);
  ck_assert_int_eq(getFigureElem(1, 1), 1);
  ck_assert_int_eq(getFigureElem(1, 2), 1);

  rotateFigure();
  ck_assert_int_eq(getFigureElem(0, 2), 1);
  ck_assert_int_eq(getFigureElem(1, 2), 1);
  ck_assert_int_eq(getFigureElem(2, 2), 1);

  terminateGame();
}
END_TEST

// Тестируем сдвиг фигуры
START_TEST(test_updateCurrentState) {
  initGame();

  getNextFigure();
  updateCurrentState();
  ck_assert_int_eq(getFigureY(), -1);

  runGame();
  updateCurrentState();
  ck_assert_int_eq(getFigureY(), 0);
  updateCurrentState();
  ck_assert_int_eq(getFigureY(), 1);

  moveFigureDownFull();
  updateCurrentState();
  ck_assert_int_eq(getFigureY(), -1);

  terminateGame();
}
END_TEST

// Тестируем сохранение рекорда
START_TEST(test_saveHighScore) {
  initGame();

  saveHighScore();

  FILE *file = fopen(HIGH_SCORE_FILE, "r");
  ck_assert_ptr_ne(file, NULL);
  int savedHighScore;
  ck_assert_int_eq(fscanf(file, "%d", &savedHighScore), 1);
  ck_assert_int_eq(savedHighScore, getHighScore());
  fclose(file);

  terminateGame();
}
END_TEST

// Тестируем запуск игры
START_TEST(test_userInput_Start) {
  ck_assert_int_eq(isGameOver(), true);
  userInput(Start, false);
  ck_assert_int_eq(isGameOver(), false);
}
END_TEST

// Тестируем паузу игры
START_TEST(test_userInput_Pause) {
  userInput(Start, false);
  userInput(Pause, false);
  ck_assert_int_eq(isPaused(), true);
  userInput(Pause, false);
  ck_assert_int_eq(isPaused(), false);
}
END_TEST

// Тестируем завершение игры
START_TEST(test_userInput_Terminate) {
  userInput(Start, false);
  userInput(Terminate, false);
  ck_assert_int_eq(isGameOver(), true);
}
END_TEST

// Тестируем перемещение фигур
START_TEST(test_userInput_MoveLeft) {
  userInput(Start, false);
  userInput(Left, false);
  ck_assert_int_eq(getFigureX(), FIELD_WIDTH / 2 - 3);
}
END_TEST

// Тестируем перемещение фигур
START_TEST(test_userInput_MoveRight) {
  userInput(Start, false);
  userInput(Right, false);
  ck_assert_int_eq(getFigureX(), FIELD_WIDTH / 2 - 1);
}
END_TEST

// Тестируем перемещение фигур
START_TEST(test_userInput_MoveDown) {
  userInput(Start, false);
  userInput(Down, false);
  ck_assert_int_eq(getFigureY(), FIELD_HEIGHT - BLOCKS_N + 1);
}
END_TEST

// Тестируем вращение фигур
START_TEST(test_userInput_Action) {
  userInput(Start, false);
  userInput(Action, false);
  userInput(Right, false);
  ck_assert_int_eq(getFigureX(), FIELD_WIDTH / 2 - 1);
}
END_TEST

// Основная функция для запуска тестов
Suite *brick_game_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Brick Game");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_initGame);
  tcase_add_test(tc_core, test_terminateGame);
  tcase_add_test(tc_core, test_addScore);
  tcase_add_test(tc_core, test_changeLevel);
  tcase_add_test(tc_core, test_endGame);
  tcase_add_test(tc_core, test_clearLines);
  tcase_add_test(tc_core, test_moveFigureDown);
  tcase_add_test(tc_core, test_moveFigureDownFull);
  tcase_add_test(tc_core, test_moveFigureLeft_and_Right);
  tcase_add_test(tc_core, test_rotateFigure);
  tcase_add_test(tc_core, test_updateCurrentState);
  tcase_add_test(tc_core, test_saveHighScore);
  tcase_add_test(tc_core, test_userInput_Start);
  tcase_add_test(tc_core, test_userInput_Pause);
  tcase_add_test(tc_core, test_userInput_Terminate);
  tcase_add_test(tc_core, test_userInput_MoveLeft);
  tcase_add_test(tc_core, test_userInput_MoveRight);
  tcase_add_test(tc_core, test_userInput_MoveDown);
  tcase_add_test(tc_core, test_userInput_Action);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = brick_game_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}
