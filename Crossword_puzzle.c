#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5
#define MAX_WORD 20

char grid[SIZE][SIZE];
char hints[5][MAX_WORD] = {"SUPER", "WORLD", "HELLO", "CROSS", "WORDS"};

typedef struct {
    int row, col;
    char direction;
    char word[MAX_WORD];
} Move;

Move stack[100];
int top = -1;

void initGrid() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            grid[i][j] = '.';
}

void displayGrid() {
    printf("\nCrossword Grid:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            printf(" %c", grid[i][j]);
        printf("\n");
    }
}

int canPlaceHorizontally(int row, int col, const char* word) {
    int len = strlen(word);
    if (col + len > SIZE) return 0;
    for (int i = 0; i < len; i++)
        if (grid[row][col + i] != '.' && grid[row][col + i] != word[i])
            return 0;
    return 1;
}

int canPlaceVertically(int row, int col, const char* word) {
    int len = strlen(word);
    if (row + len > SIZE) return 0;
    for (int i = 0; i < len; i++)
        if (grid[row + i][col] != '.' && grid[row + i][col] != word[i])
            return 0;
    return 1;
}

void placeWordHorizontally(int row, int col, const char* word) {
    for (int i = 0; i < strlen(word); i++)
        grid[row][col + i] = word[i];
}

void placeWordVertically(int row, int col, const char* word) {
    for (int i = 0; i < strlen(word); i++)
        grid[row + i][col] = word[i];
}

void removeWordHorizontally(int row, int col, const char* word) {
    for (int i = 0; i < strlen(word); i++)
        grid[row][col + i] = '.';
}

void removeWordVertically(int row, int col, const char* word) {
    for (int i = 0; i < strlen(word); i++)
        grid[row + i][col] = '.';
}

void push(Move move) {
    stack[++top] = move;
}

Move pop() {
    if (top < 0) {
        Move empty = {-1, -1, ' ', ""};
        return empty;
    }
    return stack[top--];
}

int hintIndex = 0;
void showHint() {
    if (hintIndex < 5) {
        printf("Hint: %s\n", hints[hintIndex]);
        hintIndex++;
    } else {
        printf("No more hints available!\n");
    }
}

int main() {
    initGrid();
    int choice;

    while (1) {
        printf("\n1. Display Grid\n");
        printf("2. Place Word\n");
        printf("3. Undo Last Move\n");
        printf("4. Show Hint\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            displayGrid();
        } else if (choice == 2) {
            char word[MAX_WORD], dir;
            int row, col;

            printf("Enter word (in uppercase): ");
            scanf("%s", word);
            printf("Enter row (0-%d): ", SIZE - 1);
            scanf("%d", &row);
            printf("Enter col (0-%d): ", SIZE - 1);
            scanf("%d", &col);
            getchar();
            printf("Enter direction (H for horizontal, V for vertical): ");
            scanf("%c", &dir);
            dir = toupper(dir);

            if (dir == 'H' && canPlaceHorizontally(row, col, word)) {
                placeWordHorizontally(row, col, word);
                Move newMove = {row, col, dir, ""};
                strcpy(newMove.word, word);
                push(newMove);
                printf("\nWord placed successfully!\n");
            } else if (dir == 'V' && canPlaceVertically(row, col, word)) {
                placeWordVertically(row, col, word);
                Move newMove = {row, col, dir, ""};
                strcpy(newMove.word, word);
                push(newMove);
                printf("\nWord placed successfully!\n");
            } else {
                printf("\nCannot place word at given position!\n");
            }
        } else if (choice == 3) {
            Move last = pop();
            if (last.row == -1) {
                printf("No moves to undo.\n");
            } else {
                if (last.direction == 'H')
                    removeWordHorizontally(last.row, last.col, last.word);
                else
                    removeWordVertically(last.row, last.col, last.word);
                printf("Last move undone!\n");
            }
        } else if (choice == 4) {
            showHint();
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
