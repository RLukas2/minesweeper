#include "cursor.hpp"

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

void cls() {
    COORD Position;

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void gotoxy(int X, int Y) {
    COORD Position = {Y, X};
    SetConsoleCursorPosition(hOut, Position);
}
