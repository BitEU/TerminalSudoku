# Terminal Sudoku

A command-line Sudoku puzzle game written in C. This game generates random Sudoku puzzles with varying difficulty levels and provides an interactive terminal-based interface for solving them.

## Features

- **Random Puzzle Generation**: Generates unique Sudoku puzzles with guaranteed single solutions
- **Multiple Difficulty Levels**: Choose from Easy (40 removed cells), Medium (50 removed cells), or Hard (54 removed cells)
- **Interactive Gameplay**: Navigate the board with arrow keys or WASD controls
- **Cross-Platform Compatibility**: Works on Windows, Linux, and macOS
- **Visual Cursor**: Highlights your current position on the board
- **Input Validation**: Prevents modification of original puzzle numbers
- **Win Detection**: Automatically detects when you've successfully completed the puzzle

## How to Play

1. **Navigation**: 
   - Use arrow keys or WASD to move the cursor around the board
   - Current position is shown in brackets `[X]`

2. **Number Input**:
   - Press numbers 1-9 to place a number in the current cell
   - Press 0 to clear a cell
   - You can only modify empty cells (original puzzle numbers are protected)

3. **Game Controls**:
   - Press 'Q' or 'q' to quit the game at any time

4. **Winning**:
   - Fill all empty cells with valid numbers according to Sudoku rules
   - The game will automatically detect when you've won

## Sudoku Rules

- Each row must contain the digits 1-9 with no repetition
- Each column must contain the digits 1-9 with no repetition  
- Each of the nine 3×3 sub-grids must contain the digits 1-9 with no repetition

## Building and Running

### Prerequisites

- Microsoft Visual Studio Build Tools or Visual Studio (for Windows)
- C compiler (gcc, clang, or MSVC)

### Windows (Visual Studio Developer Command Prompt)

1. **Open VS Developer Command Prompt**:
   - Search for "Developer Command Prompt for VS" in the Start menu
   - Or open a regular command prompt and run `vcvars64.bat` (usually located in `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\`)

2. **Navigate to project folder**:
   ```cmd
   cd path\to\TerminalSudoku
   ```

3. **Compile the program**:
   ```cmd
   cl sudoku.c /Fe:sudoku.exe
   ```

4. **Run the game**:
   ```cmd
   sudoku.exe
   ```

## Game Interface

```
Sudoku Puzzle
Use arrow keys or WASD to move, 1-9 to place numbers, 0 to clear. 'q' to quit.
Current position: [1,1]

[5] .  . | .  .  . | .  8  . 
 .  2  . | .  .  . | .  .  6 
 .  .  . | 5  .  . | .  .  . 
-----------------------
 .  .  . | .  1  . | .  .  . 
 .  .  4 | .  .  . | 2  .  . 
 .  .  . | .  7  . | .  .  . 
-----------------------
 .  .  . | .  .  2 | .  .  . 
 1  .  . | .  .  . | .  4  . 
 .  6  . | .  .  . | .  .  7 
```

- Numbers from the original puzzle are displayed normally
- Empty cells show as dots (`.`)
- Your current position is highlighted with brackets `[X]`
- User-entered numbers are displayed clearly

## Technical Details

### Algorithm Features

- **Puzzle Generation**: Uses a backtracking algorithm to generate complete valid Sudoku solutions
- **Unique Solution Guarantee**: Ensures each generated puzzle has exactly one solution by systematically removing numbers and validating uniqueness
- **Optimized Solver**: Implements efficient backtracking with constraint checking
- **Cross-Platform Input**: Handles different keyboard input methods across operating systems

### Code Structure

- `isValid()`: Validates number placement according to Sudoku rules
- `solveSudoku()`: Backtracking solver for puzzle generation
- `countSolutions()`: Counts possible solutions to ensure puzzle uniqueness
- `generatePuzzle()`: Creates new puzzles with specified difficulty
- `gameLoop()`: Main interactive game loop
- `checkWin()`: Validates complete puzzle solution

## Difficulty Levels

- **Easy (1)**: 40 numbers removed from complete puzzle
- **Medium (2)**: 50 numbers removed from complete puzzle  
- **Hard (3)**: 54 numbers removed from complete puzzle

## Requirements

- C compiler with standard library support
- Terminal/Command prompt
- Windows: Uses `conio.h` for keyboard input
- Linux/macOS: Uses `termios.h` for cross-platform keyboard input

## License

This project is open source. Feel free to modify and distribute as needed.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.