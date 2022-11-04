#ifndef CURSOR_INCLUDED
#define CURSOR_INCLUDED

#include <iostream>
#include <windows.h>

extern HANDLE hOut;

void cls();

void gotoxy(int X, int Y);

#endif
