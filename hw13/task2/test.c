#include <ncurses.h>

int main() {
    // Инициализация ncurses
    initscr();            // Запуск ncurses
    cbreak();             // Отключает буферизацию ввода, чтобы ввод обрабатывался сразу
    noecho();             // Отключает отображение вводимых символов
    keypad(stdscr, TRUE); // Включает обработку функциональных клавиш

    // Проверка поддержки цветов и инициализация цветовых пар
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLUE);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
    }

    // Основной цикл программы
    int ch;
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case KEY_UP:
                attron(COLOR_PAIR(1));
                printw("Pressed UP\n");
                attroff(COLOR_PAIR(1));
                break;
            case KEY_DOWN:
                attron(COLOR_PAIR(2));
                printw("Pressed DOWN\n");
                attroff(COLOR_PAIR(2));
                break;
            default:
                printw("Pressed key: %c\n", ch);
                break;
        }
        refresh(); // Обновление экрана, чтобы отображать изменения
    }

    // Завершение работы с ncurses
    endwin(); // Восстановление нормального режима терминала

    return 0;
}
