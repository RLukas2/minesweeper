#include <bits/stdc++.h>
#include <time.h>
#include <windows.h>
#include <iomanip>
#include <stdio.h>
#include <conio.h>

using namespace std;

// Windows difficulties:
// + Beginner:      9,  9, 10
// + Intermediate: 16, 16, 40
// + Expert:       16, 30, 99

#include "game.cpp"
#include "Cursor.cpp"

Minesweeper Game;

int main(int argc, char* argv[]) {
    mt19937 rng((uint64_t) new char);

    long long seed = rng();

    Game._init_(9, 9, 10, seed);

    int x = 0, y = 0;
    bool IsUpdated = 0;


    return 0;
}
