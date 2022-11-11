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

void turnCursor(bool showFlag) {
    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

void setConsoleSize(int W, int H) {
     SMALL_RECT DisplayArea = {0, 0, 0, 0};
     int x = W, y = W;
     DisplayArea.Right  = x;
     DisplayArea.Bottom = y;
     SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
}

void colorBackground(int X, int Y, char TileData, bool isOpened) {
    int color = 0, BGColor = 8 - ((X ^ Y) & 1);
    switch (TileData) {
        case '0': BGColor = 6, color = BGColor + (BGColor & 1); // No color
        break;
        case '1': color = 9; // Blue
        break;
        case '2': color = 10; // Green
        break;
        case '3': color = 12; // Red
        break;
        case '4': color = 5; // Purple
        break;
        case '5': color = 4; // Maroon or dark red
        break;
        case '6': color = 11; // Turquoise
        break;
        case '7': color = 0; // Black;
        break;
        case '8': color = 6; // Gray;
        break;
        case 'F': color = 14; // Flag
        break;
        case 'B': BGColor = 12, color = 15;
        break;
        case 'X': color = 14;
        break;
        case ' ': BGColor = (isOpened ? 14 : BGColor), color = BGColor + (BGColor & 1);
        break;
        default: BGColor = 0, color = 15; // Set to DEBUG color mode
    }

    if (isdigit(TileData) && color > 8) color -= 8; // Set to unsaturated color

    SetConsoleTextAttribute(hOut, BGColor * 16 + color);

    return;
}
