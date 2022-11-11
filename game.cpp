#include "board.hpp"
#include "cursor.hpp"
#include "game.hpp"
using namespace std;

/// Game variable - Use to set settings or gameplay
Minesweeper Game;
std::mt19937 rng((uint64_t) new char);
long long seed = 0;
int row_count;
int col_count;
int numMines;

// windows.h library - Use to handle user input
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
DWORD NumInputs = 0, InputsRead = 0;
INPUT_RECORD irInput;

/// Default settings
map<string, string> argumentLists;
void __init__() {
    // Set maximum input per once
    GetNumberOfConsoleInputEvents(hInput, &NumInputs);

    argumentLists["--difficulty"] = "expert";
    argumentLists["--rows"] = "16";
    argumentLists["--columns"] = "30";
    argumentLists["--mines"] = "99";
}

bool ReadKeyMenu(int vKey) {
    switch (vKey) {
    case VK_NUMPAD0: // NUMPAD 0 or Key 0
    case 0x30: // Continue old game (not supported yet)
        newGame();
    break;

    case VK_NUMPAD1: // NUMPAD 1 or Key 1
    case 0x31: { // Start new game
        thread t1(newGame);
        thread t2(timer);
        t1.join();
        t2.join();
    }
    break;

    case VK_NUMPAD2: // NUMPAD 2 or Key 2
    case 0x32: // Open settings Menu
        SettingsMenu();
    break;

    case VK_NUMPAD3: // NUMPAD 3 or Key 3
    case 0x33: // Exit games
        exit(0);
    break;

    default:
        return false;
    }

    return true;
}

void Menu() {
    system("cls");
    SetConsoleTextAttribute(hOut, 15);
    setConsoleSize(200, 200);

    gotoxy(0, 0);
    cout << "**********************************************************" << '\n';
    cout << "*                                                        *" << '\n';
    cout << "*                       MINESWEEPER                      *" << '\n';
    cout << "*                                                        *" << '\n';
    cout << "**********************************************************" << '\n';

    gotoxy(5, 0);
    cout << "Press number to choose!" << '\n';

    gotoxy(6, 10);
    if (FILE *file = fopen("1.sav", "r")) { // Check if have save game or not to continue
        cout << 0 << ". Continue old game." << '\n';
    }
    gotoxy(7, 10);
    cout << 1 << ". New game" << '\n';
    gotoxy(8, 10);
    cout << 2 << ". Settings" << '\n';
    gotoxy(9, 10);
    cout << 3 << ". Exit" << '\n';

    int vKey = 0;
    do {
        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);

        vKey = irInput.Event.KeyEvent.wVirtualKeyCode;

        if (irInput.Event.KeyEvent.bKeyDown) {
            bool ok = ReadKeyMenu(vKey);
            Sleep(200);

            if (ok) {
                break;
            }
        }
    } while (true);
}


/// SETTINGS
int difficultyIndexes = 2;
std::string difficultySettings[] = {"beginner", "intermediate", "expert", "custom"};
int rowsSettings[] = {9, 16, 16, 16};
int colsSettings[] = {9, 16, 30, 30};
int minesSettings[] = {10, 40, 99, 99};
// =====================

void ReadKeySettings(int vKey) {
    switch (vKey) {
    case VK_NUMPAD1: // NUMPAD 1 or Key 1
    case 0x31: { // Select Difficulty
        do {
            gotoxy(9, 10);
            ReadConsoleInput(hInput, &irInput, 1, &InputsRead);

            if (irInput.Event.KeyEvent.bKeyDown) {
                int vkey = irInput.Event.KeyEvent.wVirtualKeyCode;
                if (vkey == VK_LEFT)
                    difficultyIndexes = max(0, difficultyIndexes - 1);
                else if (vkey == VK_RIGHT)
                    difficultyIndexes = min(3, difficultyIndexes + 1);

                gotoxy(9, 32);
                cout << difficultySettings[difficultyIndexes] << "                      ";
            }
        } while (irInput.Event.KeyEvent.wVirtualKeyCode != VK_RETURN);
    }
    break;

    case VK_NUMPAD2: // NUMPAD 2 or Key 2
    case 0x32: // Choose rows
        {
            if (difficultyIndexes == 3) {
                gotoxy(10, 41);
                cin >> rowsSettings[3];
            }
        }
    break;

    case VK_NUMPAD3: // NUMPAD 3 or Key 3
    case 0x33: // Choose columns
        {
            if (difficultyIndexes == 3) {
                gotoxy(11, 41);
                cin >> colsSettings[3];
            }
        }
    break;

    case VK_NUMPAD4:
    case 0x34:
        {
            if (difficultyIndexes == 3) {
                gotoxy(12, 41);
                cin >> minesSettings[3];
            }
        }
    break;

    case VK_NUMPAD5:
    case 0x35:
        {
            return;
        }

    default:
        return;
    }


    /// Normalize if the difficulty is 30
    rowsSettings[3] = max(4, min(rowsSettings[3], 30));
    colsSettings[3] = max(4, min(colsSettings[3], 30));
    minesSettings[3] = max(1, min(minesSettings[3], min(int(rowsSettings[3] * colsSettings[3] / 1.5), 600)));

    if (difficultyIndexes == 3) {
        gotoxy(12, 32);

        int maximumMines = int(rowsSettings[3] * colsSettings[3] / 1.5);
        if (maximumMines < 10) cout << "00" << maximumMines;
        else if (maximumMines < 100) cout << '0' << maximumMines;
        else cout << maximumMines;
    }

    /// Print the settings for user
    gotoxy(10, 41); cout << rowsSettings[difficultyIndexes] << "  \n";
    gotoxy(11, 41); cout << colsSettings[difficultyIndexes] << "  \n";
    gotoxy(12, 41); cout << minesSettings[difficultyIndexes] << "  \n";
}

void SettingsMenu() {
    system("cls");
    SetConsoleTextAttribute(hOut, 15);

    gotoxy(0, 0);
    cout << "**********************************************************" << '\n';
    cout << "*                                                        *" << '\n';
    cout << "*                        SETTINGS                        *" << '\n';
    cout << "*                                                        *" << '\n';
    cout << "**********************************************************" << '\n';

    gotoxy(5, 0);
    cout << "Press number to choose!" << '\n';
    cout << "Press Left_ARROW | RIGHT_ARROW to decrease/increase. (ENTER) to set." << '\n';

    gotoxy(9, 10);
    cout << 1 << ". Select difficulty: " << argumentLists["--difficulty"] << '\n';
    gotoxy(10, 10);
    cout << 2 << ". Number of rows (4-30):      " << rowsSettings[difficultyIndexes] << '\n';
    gotoxy(11, 10);
    cout << 3 << ". Number of columns (4-30):   " << colsSettings[difficultyIndexes] << '\n';
    gotoxy(12, 10);
    cout << 4 << ". Number of mines (1-600):    " << minesSettings[difficultyIndexes] << '\n';
    gotoxy(13, 10);
    cout << 5 << ". Exit" << '\n';

    if (difficultyIndexes == 3) {
        gotoxy(12, 32);

        int maximumMines = int(rowsSettings[3] * colsSettings[3] / 1.5);
        if (maximumMines < 10) cout << "00" << maximumMines;
        else if (maximumMines < 100) cout << '0' << maximumMines;
        else cout << maximumMines;

        gotoxy(0, 0);
    }


    int vKey = 0;
    do {
        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);

        vKey = irInput.Event.KeyEvent.wVirtualKeyCode;
        if (vKey == 0x35 || vKey == VK_NUMPAD5) break;

        if (irInput.Event.KeyEvent.bKeyDown) {
            ReadKeySettings(vKey);
            Sleep(200);
        }
    } while (true);

    /// Save the settings
    argumentLists["--difficulty"] = difficultySettings[difficultyIndexes];
    argumentLists["--rows"] =       to_string(rowsSettings[difficultyIndexes]);
    argumentLists["--columns"] =    to_string(colsSettings[difficultyIndexes]);
    argumentLists["--mines"] =      to_string(minesSettings[difficultyIndexes]);
}

/// Read Argument (To be progress)
void ReadArgument(int argc, char* argv[]) {

}

/// This will be the last thing after settings or argument
void ArgumentToSettings() {
    argumentLists["--seed"] = to_string(rng());
    seed = std::atoll(argumentLists["--seed"].c_str());

    std::string Difficulty = argumentLists["--difficulty"];
    switch (Difficulty[0]) {
        case 'b': // Beginner
        row_count = 9, col_count = 9, numMines = 10;
        break;

        case 'i': // Intermediate
        row_count = 16, col_count = 16, numMines = 40;
        break;

        case 'e': // Expert
        row_count = 16, col_count = 30, numMines = 99;
        break;

        default: // Custom
        row_count = atoi(argumentLists["--rows"].c_str()),
        col_count = atoi(argumentLists["--columns"].c_str()),
        numMines = atoi(argumentLists["--mines"].c_str());
    }
}


/// ================ In gameplay mode  =================

/// Global variable
int x, y;
bool isGameOver = 0;
int currentTime = 0;

void timer() {
    currentTime = 0;
    isGameOver = 0;

    while (!isGameOver) {
        // gotoxy(0, 70); turnCursor(false);
        // cout << currentTime;
        // gotoxy(x, 2 * y + 1); turnCursor(true);

        currentTime++;
        Sleep(1000);
    }

    return;
}

/** \brief From key execute command and return the game is over or not
 *
 * \param vKey:int - Value of the virtual key mapping. Use to determine user key
 * \param (&) x:int, y:int - The position of the user cursor
 * \param (&) isUpdated:bool - Using it to check we need to update the board or not
 * \return True: If the game is still on, False: Game over
 *
 */

bool ReadKeyInGame(int vKey, int& x, int& y, bool& isUpdated) {
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

    case 0x46: { // F key - Flag
        Game.setFlag(x, y);
        Game.PrintBoard(false);
        isUpdated = 1;

        return true;
    }
    break;
    case 0x20: {    // Spacebar - using for open flag
        if (Game.flaggingTable[x][y] == true)
            return true;

        bool OK = Game.stepping(x, y);
        Game.PrintBoard(false);
        isUpdated = 1;

        return OK;
    }
    break;

    default:        // Not any key
        return true;
    }

    isUpdated = 1;
    return true;
}

void newGame() {
    system("cls");

    ArgumentToSettings();
    Game._init_(row_count, col_count, numMines, seed);

    x = 0;
    y = 0;
    isGameOver = false;
    bool isUpdated = 1;

    Game.PrintBoard(false);

    do {
        if (isUpdated) {
            gotoxy(x, 2 * y + 1); // Update cursor position
            isUpdated = 0;
        }

        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
        if (irInput.Event.KeyEvent.bKeyDown) {
            isGameOver = !ReadKeyInGame(irInput.Event.KeyEvent.wVirtualKeyCode, x, y, isUpdated);
            Sleep(100);
        }

    } while (isGameOver == false && Game.remain_count > 0);
    // Checking if the player lose yet or the number of blank tile is all opened.

    gotoxy(0, 70);
    cout << currentTime;

    {
        Game.PrintBoard(isGameOver);
        cout << '\n';
    }


    cout << (isGameOver == true ? "Game Over!" : "Winned!") << '\n';
    cout << "Press Enter to return to menu!\n";


    char temp = 'x'; while (temp != '\n') cin.get(temp);
}

