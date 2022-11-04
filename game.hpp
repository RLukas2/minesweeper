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

extern std::map<std::string, std::string> argumentLists;

/// ================       Settings    =================

void __init__();
void Menu();
void SettingsMenu();
void ReadArgument(int argc, char* argv[]);
void ArgumentToSettings();

/// ================ In gameplay mode  =================

bool SetKeyInGame(int vKey, int& x, int& y, bool& isUpdated);
void newGame();

#endif // GAME_HPP_INCLUDED
