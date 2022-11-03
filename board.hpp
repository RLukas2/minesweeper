#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstring>
#include <cctype>
#include <queue>
#include <windows.h>

struct Minesweeper {
    // Definition of the game, randomness and other
    const static int MAXSIZE = 30;
    char playingTable[MAXSIZE + 1][MAXSIZE + 1];
    bool flaggingTable[MAXSIZE + 1][MAXSIZE + 1];
    bool steppedTable[MAXSIZE + 1][MAXSIZE + 1];

    int row_count;
    int col_count;
    int mine_count;
    int remain_count;
    long long seed;
    bool first_step = false;
    std::mt19937 rng;

    int randomInt(int l, int r);

    // ========================== Initializing ==========================

    void _init_(int rows, int cols, int mines, long long seed);
    std::pair<int, int> getPosition(int Index);
    void buildGrid();
    bool isSafe(int x, int y);
    void CalculateMine();

    // ========================== User's Input ==========================

    void setFlag(int x, int y);
    int numberOfFlag(int sx, int sy);
    void propagate(int sx, int sy, bool& OnBomb, bool numberedTile);
    bool stepping(int x, int y);
    char TileData(int x, int y, bool ShowBomb);
    void PrintBoard(bool ShowBomb);
};

#endif
