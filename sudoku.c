// compile with cl sudoku.c -o sudoku.exe

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // Used for memset

// For cross-platform compatibility, conio.h is Windows-specific.
// On Linux/macOS, you might need an alternative like ncurses.
#ifdef _WIN32
#include <conio.h>
#else
// A simple getch implementation for Linux/macOS
#include <termios.h>
#include <unistd.h>
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif


int board[9][9];
int original_puzzle[9][9];
int cursor_row = 0;
int cursor_col = 0;

// Function Prototypes
void printBoard();
int isValid(int row, int col, int num);
int solveSudoku();
int countSolutions(int* solution_count);
void generatePuzzle(int difficulty);
void gameLoop();
int checkWin();

/**
 * Checks if placing a number in a given cell is valid according to Sudoku rules.
 */
int isValid(int row, int col, int num) {
    // Check row and column
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return 0;
        }
    }

    // Check 3x3 subgrid
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) {
                return 0;
            }
        }
    }
    return 1;
}

/**
 * Prints the Sudoku board to the console with the cursor position highlighted.
 */
void printBoard() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("Sudoku Puzzle\n");
    printf("Use arrow keys or WASD to move, 1-9 to place numbers, 0 to clear. 'q' to quit.\n");
    printf("Current position: [%d,%d]\n\n", cursor_row + 1, cursor_col + 1);

    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0 && i != 0) {
            printf("-----------------------\n");
        }
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0 && j != 0) {
                printf("| ");
            }
            if (i == cursor_row && j == cursor_col) {
                printf("[%c]", board[i][j] == 0 ? '.' : board[i][j] + '0');
            } else {
                // Use a different character for preset numbers vs user input for clarity
                if (original_puzzle[i][j] != 0) {
                     printf(" %d ", board[i][j]);
                } else {
                     printf(" %c ", board[i][j] == 0 ? '.' : board[i][j] + '0');
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}


/**
 * A backtracking algorithm to find a solution for the Sudoku board.
 * Returns 1 if a solution is found, 0 otherwise.
 */
int solveSudoku() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                // Shuffle numbers for randomness in generation
                for (int k = 0; k < 9; k++) {
                    int l = rand() % 9;
                    int temp = nums[k];
                    nums[k] = nums[l];
                    nums[l] = temp;
                }
                for (int n = 0; n < 9; n++) {
                    if (isValid(i, j, nums[n])) {
                        board[i][j] = nums[n];
                        if (solveSudoku()) {
                            return 1;
                        }
                        board[i][j] = 0; // Backtrack
                    }
                }
                return 0; // No valid number found
            }
        }
    }
    return 1; // Board is full
}

/**
 * [FIXED] Correctly counts the number of possible solutions for the current board
 * using backtracking. It stops once it finds 2 solutions, as we only need to
 * know if the solution is unique or not.
 */
int countSolutions(int* solution_count) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                for (int k = 1; k <= 9; k++) {
                    if (isValid(i, j, k)) {
                        board[i][j] = k;
                        countSolutions(solution_count);
                        board[i][j] = 0; // Backtrack
                        if (*solution_count >= 2) return 0; // Optimization
                    }
                }
                return 0;
            }
        }
    }
    (*solution_count)++;
    return 0;
}


/**
 * [REWRITTEN] Generates a new puzzle. This function no longer hangs.
 * It systematically tries to remove cells from a full solution while
 * ensuring the puzzle remains unique.
 */
void generatePuzzle(int difficulty) {
    // 1. Clear board and generate a full, valid solution
    memset(board, 0, sizeof(board));
    solveSudoku();

    // 2. Define removal counts by difficulty
    int cells_to_remove = 0;
    if (difficulty == 1) cells_to_remove = 40; // Easy
    if (difficulty == 2) cells_to_remove = 50; // Medium
    if (difficulty == 3) cells_to_remove = 54; // Hard

    // 3. Create a shuffled list of all cell positions
    int positions[81];
    for(int i=0; i<81; i++) positions[i] = i;
    for(int i=0; i<81; i++) {
        int j = rand() % 81;
        int temp = positions[i];
        positions[i] = positions[j];
        positions[j] = temp;
    }

    // 4. Try to remove cells one by one from the shuffled list
    int removed_count = 0;
    for(int i=0; i<81 && removed_count < cells_to_remove; i++) {
        int row = positions[i] / 9;
        int col = positions[i] % 9;
        
        if (board[row][col] != 0) {
            int temp = board[row][col];
            board[row][col] = 0;

            int solution_count = 0;
            countSolutions(&solution_count);

            if (solution_count != 1) {
                // If not unique, restore the number
                board[row][col] = temp;
            } else {
                removed_count++;
            }
        }
    }

    // 5. Copy the final puzzle to original_puzzle to track fixed cells
    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            original_puzzle[i][j] = board[i][j];
        }
    }
}


/**
 * [FIXED] Checks if the user has correctly solved the puzzle.
 * The original implementation had a flaw where it would check a number against itself.
 */
int checkWin() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) return 0; // Board is not full

            int num = board[i][j];
            board[i][j] = 0; // Temporarily empty the cell to validate against others
            if (!isValid(i, j, num)) {
                board[i][j] = num; // Restore number
                return 0; // Invalid placement found
            }
            board[i][j] = num; // Restore number
        }
    }
    return 1; // All cells are valid
}


void gameLoop() {
    char ch;
    printBoard();
    while (1) {
        ch = getch();
        if (ch == 'q' || ch == 'Q') break;

        // Enhanced arrow key handling for Windows
        if (ch == 224 || ch == 0 || ch == -32) { // Different possible prefixes for special keys
            ch = getch(); // Get the actual arrow key code
            switch(ch) {
                case 72: // Up arrow (or 'H' on some systems)
                    cursor_row = (cursor_row - 1 + 9) % 9;
                    break;
                case 80: // Down arrow (or 'P' on some systems)
                    cursor_row = (cursor_row + 1) % 9;
                    break;
                case 75: // Left arrow (or 'K' on some systems)
                    cursor_col = (cursor_col - 1 + 9) % 9;
                    break;
                case 77: // Right arrow (or 'M' on some systems)
                    cursor_col = (cursor_col + 1) % 9;
                    break;
            }
        } else if (ch == 27) { // ANSI escape codes for Linux/macOS
            if (getch() == '[') {
                 ch = getch();
                 if (ch == 'A') cursor_row = (cursor_row - 1 + 9) % 9; // Up
                 if (ch == 'B') cursor_row = (cursor_row + 1) % 9;   // Down
                 if (ch == 'D') cursor_col = (cursor_col - 1 + 9) % 9; // Left
                 if (ch == 'C') cursor_col = (cursor_col + 1) % 9;   // Right
            }
        }
        // WASD controls as alternative
        else if (ch == 'w' || ch == 'W') cursor_row = (cursor_row - 1 + 9) % 9; // Up
        else if (ch == 's' || ch == 'S') cursor_row = (cursor_row + 1) % 9;   // Down
        else if (ch == 'a' || ch == 'A') cursor_col = (cursor_col - 1 + 9) % 9; // Left
        else if (ch == 'd' || ch == 'D') cursor_col = (cursor_col + 1) % 9;   // Right
        else if (ch >= '0' && ch <= '9') {
            // Only allow changing non-original numbers
            if (original_puzzle[cursor_row][cursor_col] == 0) {
                board[cursor_row][cursor_col] = ch - '0';
            }
        }
        
        printBoard();

        if (checkWin()) {
            printf("Congratulations! You solved the puzzle!\n");
            break;
        }
    }
}

int main() {
    srand(time(0));
    int difficulty = 0;
    
    printf("Generating random sudoku puzzle...\n");
    while(difficulty < 1 || difficulty > 3) {
        printf("Select difficulty (1-Easy, 2-Medium, 3-Hard): ");
        scanf("%d", &difficulty);
    }
    
    printf("Generating puzzle, please wait...\n");
    generatePuzzle(difficulty);
    gameLoop();

    return 0;
}