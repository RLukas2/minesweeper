#ifndef CURSOR_INCLUDED
#define CURSOR_INCLUDED

#include <iostream>
#include <windows.h>

extern HANDLE hOut;

void cls();

void gotoxy(int X, int Y);

void colorBackground(int X, int Y, char TileData, bool isOpened);

void turnCursor(bool showFlag);

void setConsoleSize(int W, int H);

#endif
