/**
 * @file cli.c
 * @brief Файл содержит реализацию функций отрисовки игры.
 */
#include "cli.h"

/**
 * @brief Инициализация терминального интерфейса
 *
 * Инициализирует ncurses, отключает буферизацию ввода,
 * отключает эхо ввода, включает обработку специальных
 * клавиш (стрелки, управление курсором и т.д.) и скрывает
 * курсор.
 */
void initCli() {
  initscr();             // Инициализация окна ncurses
  noecho();              // Отключение эха ввода
  curs_set(0);           // Скрыть курсор
  keypad(stdscr, TRUE);  // Включить обработку спец. клавиш (стрелки и т.д.)
  timeout(50);
}

void printRectangle(int top_y, int bottom_y, int left_x, int right_x) {
  // Отрисовка верхней границы
  mvaddch(top_y, left_x, ACS_ULCORNER);
  for (int i = left_x + 1; i < right_x; i++) {
    mvaddch(top_y, i, ACS_HLINE);
  }
  mvaddch(top_y, right_x, ACS_URCORNER);

  // Отрисовка боковых границ
  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }

  // Отрисовка нижней границы
  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  for (int i = left_x + 1; i < right_x; i++) {
    mvaddch(bottom_y, i, ACS_HLINE);
  }
  mvaddch(bottom_y, right_x, ACS_LRCORNER);
}

void printFigure(int y, int x, GameInfo_t game) {
  for (int i = 0; i < BLOCKS_N; i++) {
    for (int j = 0; j < BLOCKS_N; j++) {
      if (game.next[i][j]) {
        mvprintw(y + i + 1, x + j * 2 + 1, "[]");
      } else {
        mvprintw(y + i + 1, x + j * 2 + 1, "..");
      }
    }
  }
}

void printGameInfo(GameInfo_t game) {
  printRectangle(0, FIELD_HEIGHT + 1, FIELD_WIDTH * 2 + 2, FIELD_WIDTH * 4 + 1);
  mvprintw(1, FIELD_WIDTH * 2 + 3, "Score: %d", game.score);
  mvprintw(3, FIELD_WIDTH * 2 + 3, "Level: %d", game.level);
  mvprintw(5, FIELD_WIDTH * 2 + 3, "High Score: %d", game.high_score);
  mvprintw(7, FIELD_WIDTH * 2 + 3, "Next figure:");
  printRectangle(8, 8 + BLOCKS_N + 1, FIELD_WIDTH * 2 + 3,
                 FIELD_WIDTH * 2 + 4 + BLOCKS_N * 2);
  printFigure(8, FIELD_WIDTH * 2 + 3, game);
  if (game.pause) {
    mvprintw(11 + BLOCKS_N, FIELD_WIDTH * 2 + 3, "Game paused");
  }

  printRectangle(FIELD_HEIGHT + 2, FIELD_HEIGHT + INFO_HEIGHT, 0,
                 FIELD_WIDTH * 4 + 1);
  mvprintw(FIELD_HEIGHT + 3, 1, "Controls");
  mvprintw(FIELD_HEIGHT + 4, 1, "Left: Left arrow");
  mvprintw(FIELD_HEIGHT + 5, 1, "Right: Right arrow");
  mvprintw(FIELD_HEIGHT + 6, 1, "Down: Down arrow");
  mvprintw(FIELD_HEIGHT + 7, 1, "Rotate: Space");
  mvprintw(FIELD_HEIGHT + 8, 1, "Pause: p");
  mvprintw(FIELD_HEIGHT + 9, 1, "Quit: q");
}

/**
 * @brief Отрисовывает игровое поле и информацию об игре
 *
 * Очищает терминальное окно ncurses, отрисовывает игровое
 * поле, состоящее из фигур, и информацию об игре:
 * текущий счет, уровень и рекорд.
 */
void renderGame(GameInfo_t game) {
  if (!game.field) {
    return;
  }

  clear();

  printRectangle(0, FIELD_HEIGHT + 1, 0, FIELD_WIDTH * 2 + 1);

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (game.field[i][j]) {
        mvprintw(i + 1, j * 2 + 1, "[]");
      } else {
        mvprintw(i + 1, j * 2 + 1, "..");
      }
    }
  }

  printGameInfo(game);
  refresh();
}

void printStartGame() {
  mvprintw(0, 0, "Press 's' to start, 'p' to pause, 'q' to quit\n");
  refresh();
}

void printEndGame(int winFlag) {
  if (winFlag) {
    mvprintw(13 + BLOCKS_N, FIELD_WIDTH * 2 + 3, "You win!");
  } else {
    mvprintw(13 + BLOCKS_N, FIELD_WIDTH * 2 + 3, "Game Over!");
  }
  mvprintw(14 + BLOCKS_N, FIELD_WIDTH * 2 + 3, "Press 'q' to quit");
  refresh();
}

void logicGame() {
  initCli();
  printStartGame();

  UserAction_t action = getKey();
  while (action != Start && action != Terminate) {
    action = getKey();
  }

  if (action == Terminate) {
    endwin();
    printf("Game Over!\n");
  } else {
    GameInfo_t game = updateCurrentState();  // Инициализация игры
    renderGame(game);
    long lastFallTime = getCurrentTimeMillis();

    int winFlag = 0;
    while (action != Terminate) {
      if (action >= 0) {
        userInput(action, false);
        renderGame(game);
      }

      long currentTime = getCurrentTimeMillis();  // сдвиг по таймеру
      if (currentTime - lastFallTime >= game.speed) {
        game = updateCurrentState();  // фигура падает
        renderGame(game);
        lastFallTime = currentTime;
      }

      action = getKey();
      if (isGameOver()) {
        action = Terminate;
        winFlag = playerWin();
      }
    }

    printEndGame(winFlag);
    action = getKey();
    while (action != Terminate) {
      action = getKey();
    }
    userInput(action, false);
    endwin();
    printf("Game Over!\n");
  }
}

UserAction_t getKey() {
  UserAction_t action = -1;
  switch (getch()) {
    case 's':
    case 'S':
      action = Start;
      break;
    case 'p':
    case 'P':
      action = Pause;
      break;
    case 'q':
    case 'Q':
      action = Terminate;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_UP:
      action = Up;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case KEY_ENTER:
    case ENTER_KEY:
    case ' ':
      action = Action;
      break;
    default:
      break;
  }
  return action;
}

long getCurrentTimeMillis() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
