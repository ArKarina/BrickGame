# Имя проекта
PROJECT = BrickGame

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

# Библиотеки и исполняемый файл
BRICK_LIB = libbrick.a
GUI_LIB = libgui.a
EXE_TARGET = tetris

# Папки с исходниками
BRICK_DIR = brick_game/tetris
CLI_DIR = gui/cli
TEST_DIR = brick_game/tests
DOC_DIR = doxygen

# Объектные файлы
BRICK_MODULES = $(wildcard $(BRICK_DIR)/*.c)
BRICK_OBJS = $(patsubst $(BRICK_DIR)/%.c, $(BRICK_DIR)/%.o, $(wildcard $(BRICK_DIR)/*.c))
CLI_OBJS = $(patsubst $(CLI_DIR)/%.c, $(CLI_DIR)/%.o, $(wildcard $(CLI_DIR)/*.c))

# Библиотеки для тестов
ifeq ($(shell uname), Linux)
    LDLIBS = -lcheck_pic -pthread -lrt -lm -lsubunit
else
    LDLIBS = -lcheck
endif

# Стандартные цели
.PHONY: all install uninstall clean dvi dist test gcov_report style_check format_style

# Сборка проекта
all: clean $(BRICK_LIB) $(GUI_LIB) $(EXE_TARGET)

# Библиотека логики игры
$(BRICK_LIB): $(BRICK_OBJS)
	@ar rcs $@ $^

# Библиотека GUI
$(GUI_LIB): $(CLI_OBJS)
	@ar rcs $@ $^

# Игровое приложение
$(EXE_TARGET): $(BRICK_LIB) $(GUI_LIB) tetris.c
	$(CC) $(CFLAGS) -lncurses -L. -lbrick -lgui tetris.c -o $@

# Компиляция объектных файлов
$(BRICK_DIR)/%.o: $(BRICK_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CLI_DIR)/%.o: $(CLI_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Установка программы
install: all
	@mkdir -p install_dir
	@cp $(EXE_TARGET) install_dir/$(PROJECT)

# Удаление программы
uninstall:
	@rm -rf install_dir

# Очистка
clean: uninstall
	@rm -f $(BRICK_DIR)/*.o $(CLI_DIR)/*.o $(BRICK_LIB) $(GUI_LIB) $(EXE_TARGET) *.gcda *.gcno *.info
	@rm -rf report $(DOC_DIR)
	@rm -rf *.tar.gz $(PROJECT)

# Документация
dvi:
	@echo "Generating documentation..."
	@doxygen Doxyfile
	@open doxygen/html/index.html

# Распространение исходников
dist:
	@tar -czvf $(PROJECT).tar.gz $(BRICK_DIR) $(CLI_DIR) $(TEST_DIR) tetris.c Makefile Doxyfile
	
# Тестирование
test: $(BRICK_LIB)
	$(CC) $(CFLAGS) $(wildcard $(TEST_DIR)/*.c) $(LDLIBS) -L. -lbrick -o $(TEST_DIR)/test_runner
	$(TEST_DIR)/test_runner

# Покрытие тестами и отчет
gcov_report: $(BRICK_LIB)
	@echo "Generating coverage report..."
	@$(CC) $(CFLAGS) --coverage $(wildcard $(TEST_DIR)/*.c) $(LDLIBS) $(BRICK_MODULES) -o ./$(TEST_DIR)/test_runner
	@./$(TEST_DIR)/test_runner
	@lcov -t "test_coverage" -o test_coverage.info -c -d .
	@genhtml -o report test_coverage.info
	@open report/index.html
	@rm -rf *.gcda *.gcno $(TEST_DIR)/test_runner test_coverage.info

# Проверка стиля
style_check:
	@echo "┏=========================================┓"
	@echo "┃  Checking your code for Google Style    ┃"
	@echo "┗=========================================┛"
	@cp ../materials/linters/.clang-format .
	@clang-format -n *.c brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c gui/cli/*.h brick_game/tests/*.c
	@rm ./.clang-format

format_style:
	@echo "┏=========================================┓"
	@echo "┃  Formatting your code for Google Style  ┃"
	@echo "┗=========================================┛"
	@cp ../materials/linters/.clang-format .
	@clang-format -i *.c brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c gui/cli/*.h brick_game/tests/*.c
	@rm ./.clang-format
