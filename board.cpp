#include "board.hpp"

// Random a integer in range [l; r]
int Minesweeper::randomInt(int l, int r) {
    return l + rng() % (r - l + 1);
}

// ========================== Initializing ==========================

void Minesweeper::_init_(int rows, int cols, int mines, long long seed = 0) {
    std::memset(playingTable, 0, sizeof playingTable);
    std::memset(steppedTable, 0, sizeof playingTable);
    this->row_count = rows;
    this->col_count = cols;
    this->mine_count = mines;
    this->seed = seed;
    this->first_step = true;
    rng.seed(this->seed);

    buildGrid();
}

// From index, will return position (x, y) = (index / col_count, index mod col_count)
std::pair<int, int> Minesweeper::getPosition(int Index) {
    return std::make_pair(Index / col_count, Index % col_count);
}

void Minesweeper::buildGrid() {
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

bool Minesweeper::isSafe(int x, int y) {
    return 0 <= x && x < row_count && 0 <= y && y < col_count;
}

void Minesweeper::CalculateMine() {
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

// ========================== User's Input ==========================

void Minesweeper::setFlag(int x, int y) {
    // Cannot set flag on opened tile
    if (steppedTable[x][y] == true) return;

    // Turning on and off the flagging position
    flaggingTable[x][y] ^= 1;
}

/** \brief Count number of Flag surrounded by a tile
 *
 * \param sx: int, sy: int - Tile that need to count flag surround it
 * \return TotalFlag:int - The number of flag surround the tile
 *  The function will run 3x3 area counting total flag around the tile.
 *  TotalFlag will count the number flaggingTable[x][y] = 1 and return it.
 */

int Minesweeper::numberOfFlag(int sx, int sy) {
    int TotalFlag = 0;

    for(int dx = -1; dx <= 1; ++dx)
        for(int dy = -1; dy <= 1; ++dy) {
            int x = sx + dx, y = sy + dy;

            if (!isSafe(x, y)) continue;
            TotalFlag += flaggingTable[x][y];
        }

    return TotalFlag;
}


/** \brief Stepping area that is steppable
 *
 * \param sx:int, sy:int - Starting position that we're needing to fill
 * \return updated steppedTable[][]
 *
 *  In minesweeper game, if the position we click is 0 mine. We have to fill out the surrounding the area of 0 mine
 *  Also filling out surrounding area with Stepped by updating steppedTable[][]
 */
void Minesweeper::propagate(int sx, int sy, bool& OnBomb, bool numberedTile = false) {
    if (!isSafe(sx, sy)) return;
    if (playingTable[sx][sy] == 'M' || (numberedTile == false && steppedTable[sx][sy] == true)) return;

    if (steppedTable[sx][sy] == false) // Decreasing the number of blank step left
        remain_count--;

    steppedTable[sx][sy] = true;


    if (playingTable[sx][sy] == '0' || numberedTile == true) {
        for(int dx = -1; dx <= 1; ++dx)
            for(int dy = -1; dy <= 1; ++dy) {
                int x = sx + dx, y = sy + dy;

                if (isSafe(x, y) && numberedTile == true
                    && (playingTable[x][y] == 'M' && flaggingTable[x][y] == false))
                        OnBomb = true;

                propagate(x, y, OnBomb);
            }
    }
}


/** \brief Return if player stepped on bomb or not
 *
 * \param x:int, X-position the player will step
 * \param y:int, Y-position the player will step
 * \return true if step without bomb or false if step without bomb
 *
 */
bool Minesweeper::stepping(int x, int y) {
    bool OnBomb = false;

    if (first_step == true) {
        first_step = false;

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
        propagate(x, y, OnBomb);
        return true;
    }

    // The player is clicking on flag and should not be propagate
    if (flaggingTable[x][y] == true) return true;

    // If the player stepped on Bomb then return
    if (playingTable[x][y] == 'M') return false;

    // Checking Player is stepping on number or not (within checking the number is flagged)
    if (steppedTable[x][y] == true
        && playingTable[x][y] - '0' == numberOfFlag(x, y)) {
        propagate(x, y, OnBomb, true);
        return !OnBomb;
    }

    propagate(x, y, OnBomb);

    return true;
}


void Minesweeper::PrintBoard() {
    for(int i = 0; i < row_count; ++i) {
        for(int j = 0; j < col_count; ++j) {
            if (flaggingTable[i][j] == false && steppedTable[i][j] == false) std::cout << '*';
            else {
                if (flaggingTable[i][j] == true) // The position is flagged
                    std::cout << 'F';
                else {
                    if (playingTable[i][j] == '0')
                        std::cout << ' ';
                    else
                        std::cout << playingTable[i][j];
                }

            }
        }
        std::cout << '\n';
    }

    std::cout.flush();
}
