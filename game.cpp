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
    /** playingTable[x][y] is either
     *  'M' is Mine
     *  'F' is Flag
     *  '0'-'8' is number of mine surround the square
     */

    bool steppedTable[MAXSIZE + 1][MAXSIZE + 1];
    /** steppedTable[x][y] is either
     *  0 is un-stepped position or you haven't visit this tile yet
     *  1 is stepped position or you have discover this position
     */

    int row_count;
    int col_count;
    int mine_count;
    int remain_count;
    long long seed;
    bool first_step = false;
    std::mt19937 rng;

    // Random a integer in range [l; r]
    int randomInt(int l, int r) {
        return l + rng() % (r - l + 1);
    }

    void _init_(int rows, int cols, int mines, long long seed = 0) {
        std::memset(playingTable, 0, sizeof playingTable);
        std::memset(steppedTable, 0, sizeof playingTable);
        this->row_count = rows;
        this->col_count = cols;
        this->mine_count = mines;
        this->seed = seed;
        this->first_step = false;
        rng.seed(this->seed);

        buildGrid();
    }

    // From index, will return position (x, y) = (index / col_count, index mod col_count)
    std::pair<int, int> getPosition(int Index) {
        return std::make_pair(Index / col_count, Index % col_count);
    }

    void buildGrid() {
        this->remain_count = this->row_count * this->col_count - this->mine_count;

        // Setting up the grid
        for(int i = 0; i < row_count * col_count; ++i) {
            std::pair<int, int> position = getPosition(i);
            int x = position.first, y = position.second;

            if (i < mine_count) {
                playingTable[x][y] = 'M';
            } else {
                playingTable[x][y] = '0';
            }
        }

        // Shuffle the grid
        for(int i = 0; i < row_count * col_count - 1; ++i) {
            int j = randomInt(i + 1, row_count * col_count - 1);

            std::pair<int, int> position_i = getPosition(i), position_j = getPosition(j);
            int xi = position_i.first, yi = position_i.second;
            int xj = position_j.first, yj = position_j.second;

            std::swap(playingTable[xi][yi], playingTable[xj][yj]);
        }
    }

    bool isSafe(int x, int y) {
        return 0 <= x && x < row_count && 0 <= y && y < col_count;
    }

    void CalculateMine() {
        // For all tile in the grid
        for(int i = 0; i < row_count; ++i) {
            for(int j = 0; j < col_count; ++j) {
                if (playingTable[i][j] == 'M') {
                    // Finding all neighbour of tile (i, j)
                    for(int dx = -1; dx <= 1; ++dx)
                    for(int dy = -1; dy <= 1; ++dy) {
                        int new_i = i + dx, new_j = j + dy;

                        // (dx, dy) = (0, 0) or unchanged position
                        if (new_i == i && new_j == j) continue;
                        // Out of border
                        if (!isSafe(new_i, new_j)) continue;
                        // This is another mine
                        if (playingTable[new_i][new_j] == 'M') continue;

                        playingTable[new_i][new_j] += 1;
                    }
                }
            }
        }
    }

    /** \brief Stepping area that is steppable
     *
     * \param sx:int, sy:int - Starting position that we're needing to fill
     * \return updated steppedTable[][]
     *
     *  In minesweeper game, if the position we click is 0 mine. We have to fill out the surrounding the area of 0 mine
     *  Also filling out surrounding area with Stepped by updating steppedTable[][]
     */
    void propagate(int sx, int sy) {
        if (!isSafe(sx, sy) || playingTable[sx][sy] == 'M' || steppedTable[sx][sy] == true) return;

        steppedTable[sx][sy] = true;
        if (playingTable[sx][sy] == '0') {
            for(int dx = -1; dx <= 1; ++dx)
                for(int dy = -1; dy <= 1; ++dy) {
                    int x = sx + dx, y = sy + dy;
                    propagate(x, y);
                }
        }
    }

    /** \brief Return if player stepped on bomb or not
     *
     * \param X: int, X-position the player will step
     * \param Y: int, Y-position the player will step
     * \return true if step without bomb or false if step without bomb
     *
     */
    bool stepping(int x, int y) {
        if (first_step == false) {
            first_step = true;

            // Checking neighbour of the tile has bomb or not.
            // If it does have bomb, we have to change its position to propagate
            for(int dx = -1; dx <= 1; ++dx)
                for(int dy = -1; dy <= 1; ++dy) {
                    int new_x = x + dx, new_y = y + dy;

                    // Change the mine position to other to propagate easily
                    // instead of getting only one tile open
                    if (playingTable[new_x][new_y] == 'M') {
                        int new_xx, new_yy;

                        do {
                            new_xx = randomInt(0, row_count - 1);
                            new_yy = randomInt(0, col_count - 1);

                            // If there is a mine on new spot, random again
                            if (playingTable[new_xx][new_yy] == 'M') continue;
                            if (!(std::abs(new_xx - x) <= 1 && std::abs(new_yy - x) <= 1)) break;
                        } while (true);

                        std::swap(playingTable[new_x][new_y], playingTable[new_xx][new_yy]);
                    }
                }

            CalculateMine();
            propagate(x, y);
            return true;
        }

        // If the player stepped on Bomb then return
        if (playingTable[x][y] == 'M') return false;
        propagate(x, y);
        return true;
    }

    void PrintBoard() {
        for(int i = 0; i < row_count; ++i) {
            for(int j = 0; j < col_count; ++j) {
                if (steppedTable[i][j] == false) std::cout << '*';
                else {
                    if (playingTable[i][j] == '0') std::cout << ' ';
                    else std::cout << playingTable[i][j];
                }
            }
            std::cout << '\n';
        }

        std::cout.flush();
    }
/*
    void PrintBoard(int x = 999, int y = 999) {
        for(int i = 0; i < row_count; ++i) {
            for(int j = 0; j < col_count; ++j) {

                // Coloring the user position
                if (i == x && j == y) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
                    if (steppedTable[i][j] == false) std::cout << '*';
                    else {
                        if (playingTable[i][j] == '0') std::cout << '_';
                        else std::cout << playingTable[i][j];
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                } else {
                    if (steppedTable[i][j] == false) std::cout << '*';
                    else {
                        if (playingTable[i][j] == '0') std::cout << ' ';
                        else std::cout << playingTable[i][j];
                    }
                }
            }

            std::cout << '\n';
        }
        std::cout.flush();
    }
*/
};

