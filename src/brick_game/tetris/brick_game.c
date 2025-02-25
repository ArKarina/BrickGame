/**
 * @file brick_game.c
 * @brief Файл содержит реализацию функций основной логики игры.
 */
#include "brick_game.h"

/**
 * @brief Инициализация игры
 */
static GameInfo_t game;

/**
 * @brief Инициализация текущей фигуры
 */
static Figure_t figure;

/**
 * @brief Флаг запуска игры
 */
static bool game_running = false;

void initGame() {
  game.field = create_matrix(FIELD_HEIGHT, FIELD_WIDTH);
  game.next = create_matrix(4, 4);
  generateNextFigure();
  game.score = 0;
  loadHighScore();
  game.level = 1;
  game.speed = SPEED;
  game.pause = 0;
}

void terminateGame() {
  free_matrix(game.field);
  free_matrix(game.next);
  game.score = 0;
  game.high_score = 0;
  game.level = 1;
  game.speed = SPEED;
  game.pause = 0;
}

void userInput(UserAction_t action, bool hold) {
  if (!game_running && action != Start) {
    return;
  }

  switch (action) {
    case Start:
      if (!game_running) {
        game_running = true;
        initGame();
        getNextFigure();
      }
      break;
    case Pause:
      game.pause = !game.pause;
      break;
    case Terminate:
      game_running = false;
      saveHighScore();
      terminateGame();
      break;
    case Left:
      if (!game.pause && !hold) {
        moveFigureLeft();
      }
      break;
    case Right:
      if (!game.pause && !hold) {
        moveFigureRight();
      }
      break;
    case Down:
      if (!game.pause && !hold) {
        moveFigureDownFull();
      }
      break;
    case Action:
      if (!game.pause && !hold) {
        rotateFigure();
      }
      break;
    default:
      break;
  }
}

GameInfo_t updateCurrentState() {
  if (!game_running || game.pause) {
    return game;
  }

  if (!moveFigureDown()) {
    clearLines();

    getNextFigure();
    if (!canPutFigure()) {
      game_running = false;
    } else {
      addFigure();
    }
  }

  return game;
}

void clearLines() {
  int full_lines = 0;
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    bool line_full = true;
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (game.field[y][x] == 0) {
        line_full = false;
      }
    }

    if (line_full) {
      full_lines++;
      for (int i = y; i > 0; i--) {
        memcpy(game.field[i], game.field[i - 1], FIELD_WIDTH * sizeof(int));
      }
      memset(game.field[0], 0, FIELD_WIDTH * sizeof(int));
    }
  }
  addScore(full_lines);
}

void addScore(int lines) {
  switch (lines) {
    case 1:
      game.score += 100;
      break;
    case 2:
      game.score += 300;
      break;
    case 3:
      game.score += 700;
      break;
    case 4:
      game.score += 1500;
      break;
    default:
      break;
  }

  if (game.score > game.high_score) {
    game.high_score = game.score;
  }

  changeLevel();
}

void changeLevel() {
  while (game.score >= game.level * 600) {
    game.level++;
    game.speed -= DELTA_SPEED;
    if (game.speed < 100) {
      game.speed = 100;
    }
  }
}

void generateNextFigure() {
  int figures[FIGURES_N][BLOCKS_N][BLOCKS_N] = {{// O-образная фигура
                                                 {0, 0, 0, 0},
                                                 {0, 1, 1, 0},
                                                 {0, 1, 1, 0},
                                                 {0, 0, 0, 0}},
                                                {// I-образная фигура
                                                 {0, 0, 0, 0},
                                                 {1, 1, 1, 1},
                                                 {0, 0, 0, 0},
                                                 {0, 0, 0, 0}},
                                                {// T-образная фигура
                                                 {0, 0, 0, 0},
                                                 {0, 1, 0, 0},
                                                 {1, 1, 1, 0},
                                                 {0, 0, 0, 0}},
                                                {// S-образная фигура
                                                 {0, 0, 0, 0},
                                                 {0, 1, 1, 0},
                                                 {1, 1, 0, 0},
                                                 {0, 0, 0, 0}},
                                                {// Z-образная фигура
                                                 {0, 0, 0, 0},
                                                 {1, 1, 0, 0},
                                                 {0, 1, 1, 0},
                                                 {0, 0, 0, 0}},
                                                {// L-образная фигура
                                                 {0, 0, 0, 0},
                                                 {1, 0, 0, 0},
                                                 {1, 1, 1, 0},
                                                 {0, 0, 0, 0}},
                                                {// J-образная фигура
                                                 {0, 0, 0, 0},
                                                 {0, 0, 1, 0},
                                                 {1, 1, 1, 0},
                                                 {0, 0, 0, 0}}};

  int choice = rand() % FIGURES_N;
  for (int i = 0; i < BLOCKS_N; i++) {
    for (int j = 0; j < BLOCKS_N; j++) {
      game.next[i][j] = figures[choice][i][j];
    }
  }
}

void getNextFigure() {
  figure.x = FIELD_WIDTH / 2 - 2;
  figure.y = -1;

  for (int i = 0; i < BLOCKS_N; i++) {
    for (int j = 0; j < BLOCKS_N; j++) {
      figure.figure[i][j] = game.next[i][j];
    }
  }

  generateNextFigure();
}

void addFigure() {
  for (int i = 0; i < BLOCKS_N; i++) {
    for (int j = 0; j < BLOCKS_N; j++) {
      if (figure.figure[i][j]) {
        game.field[figure.y + i][figure.x + j] = 1;
      }
    }
  }
}

void deleteFigure() {
  for (int i = 0; i < BLOCKS_N; i++) {
    for (int j = 0; j < BLOCKS_N; j++) {
      if (figure.figure[i][j]) {
        game.field[figure.y + i][figure.x + j] = 0;
      }
    }
  }
}

bool moveFigureDown() {
  bool res = true;

  deleteFigure();
  figure.y++;

  if (!canPutFigure()) {
    figure.y--;
    res = false;
  }
  addFigure();
  return res;
}

void moveFigureDownFull() {
  while (moveFigureDown()) {
    moveFigureDown();
  }
}

bool moveFigureLeft() {
  bool res = true;

  deleteFigure();
  figure.x--;

  if (!canPutFigure()) {
    figure.x++;
    res = false;
  }
  addFigure();
  return res;
}

bool moveFigureRight() {
  bool res = true;

  deleteFigure();
  figure.x++;

  if (!canPutFigure()) {
    figure.x--;
    res = false;
  }
  addFigure();
  return res;
}

bool rotateFigure() {
  if (figure.y < 0) {
    return false;
  }

  bool res = true;

  deleteFigure();
  rotateFigureRight();

  if (!canPutFigure()) {
    rotateFigureLeft();
    res = false;
  }
  addFigure();
  return res;
}

void rotateFigureRight() {
  int tmp[BLOCKS_N][BLOCKS_N];

  for (int i = 0; i < BLOCKS_N; i++) {
    for (int j = 0; j < BLOCKS_N; j++) {
      tmp[i][j] = figure.figure[j][i];
    }
  }

  for (int i = 0; i < BLOCKS_N; i++) {
    for (int j = 0; j < BLOCKS_N; j++) {
      figure.figure[i][BLOCKS_N - j - 1] = tmp[i][j];
    }
  }
}

void rotateFigureLeft() {
  int tmp[BLOCKS_N][BLOCKS_N];

  for (int i = 0; i < BLOCKS_N; i++) {
    for (int j = 0; j < BLOCKS_N; j++) {
      tmp[i][j] = figure.figure[j][i];
    }
  }

  for (int i = 0; i < BLOCKS_N; i++) {
    for (int j = 0; j < BLOCKS_N; j++) {
      figure.figure[BLOCKS_N - i - 1][j] = tmp[i][j];
    }
  }
}

bool isGameOver() { return game_running == false || game.level > MAX_LEVEL; }
bool playerWin() { return game.level > MAX_LEVEL; }

bool canPutFigure() {
  bool res = true;

  for (int i = 0; i < BLOCKS_N; i++) {
    for (int j = 0; j < BLOCKS_N; j++) {
      int new_x = figure.x + j;
      int new_y = figure.y + i;
      if (figure.figure[i][j] &&
          (new_y >= FIELD_HEIGHT || new_x < 0 || new_x >= FIELD_WIDTH ||
           game.field[new_y][new_x])) {
        res = false;
      }
    }
  }

  return res;
}

void saveHighScore() {
  FILE *file = fopen(HIGH_SCORE_FILE, "w");
  if (file) {
    if (fprintf(file, "%d", game.high_score) < 0) {
      perror("Error writing to high score file");
    }
    fclose(file);
  } else {
    perror("Error opening high score file");
  }
}

void loadHighScore() {
  FILE *file = fopen(HIGH_SCORE_FILE, "r");
  if (file) {
    if (fscanf(file, "%d", &game.high_score) != 1) {
      perror("Error reading high score file, resetting to 0.");
      game.high_score = 0;
    }
    fclose(file);
  }
}

int getScore() { return game.score; }
int getHighScore() { return game.high_score; }
int getLevel() { return game.level; }
void setLevel(int level) { game.level = level; }
int getSpeed() { return game.speed; }
int getFigureX() { return figure.x; }
int getFigureY() { return figure.y; }
void setFigureElem(int i, int j) { figure.figure[i][j] = 1; }
int getFigureElem(int i, int j) { return figure.figure[i][j]; }
void setFieldElem(int i, int j) { game.field[i][j] = 1; }
int getFieldElem(int i, int j) { return game.field[i][j]; }
void runGame() { game_running = true; }
bool isPaused() { return game.pause == 1; }