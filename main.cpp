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

// #include "Cursor.cpp"
#include "game.cpp"

Minesweeper Game;

mt19937 rng((uint64_t) new char);

long long seed;
int row_count, col_count, numMines;

void __init__() {
    seed = rng();
    row_count = 9;
    col_count = 9;
    numMines = 10;
}

// ================== Windows.h library ==================

bool SetKeyInGame(int vKey, int& x, int& y, bool& isUpdated);
void inGame();

/** \brief From key execute command and return the game is over or not
 *
 * \param vKey:int - Value of the virtual key mapping. Use to determine user key
 * \param (&) x:int, y:int - The position of the user cursor
 * \param (&) isUpdated:bool - Using it to check we need to update the board or not
 * \return True: If the game is still on, False: Game over
 *
 */

bool SetKeyInGame(int vKey, int& x, int& y, bool& isUpdated) {
    switch (vKey) {
    case VK_UP:     // Up Arrow
        x = max(x - 1, 0);
    break;
    case VK_DOWN:   // Down Arrow
        x = min(x + 1, row_count - 1);
    break;
    case VK_LEFT:   // Left Arrow
        y = max(y - 1, 0);
    break;
    case VK_RIGHT:  // Right Arrow
        y = min(y + 1, col_count - 1);
    break;
    case 0x46: // F key - Flag
        Game.setFlag(x, y);
    break;
    case 0x20: {    // Spacebar - using for open flag
            isUpdated = 1;
            return Game.stepping(x, y);
        }
    break;

    default:        // Not any key
        return true;
    }

    isUpdated = 1;
    return true;
}

void inGame() {
    Game._init_(row_count, col_count, numMines, seed);

    int x = 0, y = 0;
    bool isUpdated = 1;
    bool isGameOver = 0;

    // windows.h library - Use to handle user input
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD NumInputs = 0, InputsRead = 0;
    INPUT_RECORD irInput;
    GetNumberOfConsoleInputEvents(hInput, &NumInputs);

    do {
        if (isUpdated) {
            system("cls");

            cout << x << ' ' << y << "\n\n";
            Game.PrintBoard();
            isUpdated = 0;
        }

        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
        if (irInput.Event.KeyEvent.bKeyDown)
            isGameOver = !SetKeyInGame(irInput.Event.KeyEvent.wVirtualKeyCode, x, y, isUpdated);

        /*
        if (GetAsyncKeyState(VK_UP) & 1)
            SetKey(VK_UP, x, y, isUpdated);
        else if (GetAsyncKeyState(VK_DOWN) & 1)
            SetKey(VK_DOWN, x, y, isUpdated);
        else if (GetAsyncKeyState(VK_LEFT) & 1)
            SetKey(VK_LEFT, x, y, isUpdated);
        else if (GetAsyncKeyState(VK_RIGHT) & 1)
            SetKey(VK_RIGHT, x, y, isUpdated);
        else if (GetAsyncKeyState(0x46) & 1)
            SetKey(0x46, x, y, isUpdated);
        else if (GetAsyncKeyState(0x20) & 1)
            isGameOver = !SetKey(0x20, x, y, isUpdated);
        */
    } while (isGameOver == false && Game.remain_count > 0);
    // Checking if the player lose yet or the number of blank tile is all opened.

    cout << (isGameOver == true ? "Game Over!" : "Winned!") << '\n';
}


int main(int argc, char* argv[]) {
    __init__();

    inGame();

    return 0;
}
