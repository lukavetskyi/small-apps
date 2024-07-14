#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include <vector>

class Sand {
  std::vector<std::vector<bool>> field;
  int rows;
  int cols;

public:
  Sand(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    for (size_t y = 0; y < rows; ++y) {
      field.push_back({});
      for (size_t x = 0; x < cols; ++x) {
        field[y].push_back(false);
      }
    }
  }

  bool get_state(int row, int col) { return field[row][col]; }
  void set_state(int row, int col, bool state) { field[row][col] = state; }
  void resize(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    while (field.size() > rows)
      field.pop_back();
    while (field.size() < rows)
      field.push_back({});

    for (auto &c : field) {
      while (c.size() > cols)
        c.pop_back();
      while (c.size() < cols)
        c.push_back(false);
    }
  }
  void clear() {
    for (auto &c : field)
      for (int i = 0; i < c.size(); ++i)
        c[i] = false;
  }

  void next() {
    for (int y = rows - 1; y >= 0; --y)
      for (int x = 0; x < cols; ++x) {
        if (!field[y][x] || rows <= y + 1)
          continue;
        if (!field[y + 1][x]) {
          field[y + 1][x] = true;
          field[y][x] = false;
        } else {
          bool below_l_available = false, below_r_avalible = false;
          if (x > 0)
            below_l_available = !field[y + 1][x - 1];
          if (x < cols - 1)
            below_r_avalible = !field[y + 1][x + 1];

          if (below_l_available && !below_r_avalible) {
            field[y + 1][x - 1] = true;
            field[y][x] = false;
          } else if (!below_l_available && below_r_avalible) {
            field[y + 1][x + 1] = true;
            field[y][x] = false;
          } else if (below_l_available && below_r_avalible) {
            int s = std::rand() % 2;
            if (s) {
              field[y + 1][x + 1] = true;
              field[y][x] = false;
            } else {
              field[y + 1][x - 1] = true;
              field[y][x] = false;
            }
          }
        }
      }
  }
};

int main() {
  bool dragging = false;
  int ch, rows, cols;
  MEVENT event;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  timeout(50);
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

  getmaxyx(stdscr, rows, cols);
  Sand sand(rows, cols);

  while ((ch = getch())) {
    switch (ch) {
    case KEY_F(1):
      endwin();
      return 0;
      break;

    case KEY_F(2):
      sand.clear();
      break;

    case KEY_RESIZE:
      getmaxyx(stdscr, rows, cols);
      sand.resize(rows, cols);
      break;

    case KEY_MOUSE:
      if (getmouse(&event) == OK) {
        if (event.bstate & BUTTON1_PRESSED)
          dragging = true;
        else if (event.bstate & BUTTON1_RELEASED)
          dragging = false;
      }
      break;
    }

    if (dragging)
      sand.set_state(event.y, event.x, true);

    sand.next();

    for (size_t y = 0; y < rows; ++y) {
      for (size_t x = 0; x < cols; ++x) {
        if (sand.get_state(y, x))
          mvaddch(y, x, 'O');
        else
          mvaddch(y, x, ' ');
      }
    }

    refresh();
  }
}
