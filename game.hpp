#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <bits/stdc++.h>
#include <time.h>
#include <windows.h>
#include <iomanip>
#include <stdio.h>
#include <conio.h>

extern std::mt19937 rng;
extern long long seed;
extern int row_count, col_count, numMines;

void __init__();

/// ================ In gameplay mode  =================
// ================== Windows.h library ==================

bool SetKeyInGame(int vKey, int& x, int& y, bool& isUpdated);
void inGame();

#endif // GAME_HPP_INCLUDED
