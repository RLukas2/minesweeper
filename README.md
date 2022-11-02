# Minesweeper

This is game project for CS161 - Minesweeper all using C++ normal libraries.

For first upload (or BETA), there is not a lot of settings to choose nor optimization. So the game may buggy, crashes,...

Also this is the first game project that I have created. So any feedbacks could help me.

## TODO
- Improve game optimization and flickering problem.
- Game arguments for checking or without using in-game settings.
- In-game settings (with change color, game difficulty, custom size board,... etc).
- Save game with high score timer.
- Moving the game to GUI or console.
- Guess-free board (lowest priority right now).
- Having fun.


## Command line arguments (TODO)

| flag                  | shorthand         | type                                                          | default                        | description                                                                                                                                                                              |
|-----------------------|-------------------|---------------------------------------------------------------|--------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `--seed`              | `-s`              | `int`                                                         | N/A                            | Specifies the global seed for both the game board and agent.                                                                                                                             |
| `--agent`             | `-a`              | <code>"simple" &#124; "set"</code>                            | `"set"`                        | Selects the agent used to play the game.                                                                                                                                                 |
| `--difficulty`        | `-d`              | <code>"beginner" &#124; "intermediate" &#124; "expert"</code> | `"expert"`                     | Selects the difficulty of the board using standard game defaults.                                                                                                                        |
| `--rows` `--height`   | `-r` `-H`         | `int`                                                         | `16`                           | Specifies the number of rows of the game board.                                                                                                                                          |
| `--columns` `--width` | `-c` `-W`         | `int`                                                         | `30`                           | Specifies the number of columns of the game board.                                                                                                                                       |
| `--mines`             | `-m`              | `int`                                                         | `99`                           | Specifies the total number of mines in the game board.                                                                                                                                                          |
| `--show-mines`        | N/A               | N/A                                                           | N/A                            | Shows all mines present in the game board in previews. Has no effect unless verbosity is set high enough.                                                                                |
