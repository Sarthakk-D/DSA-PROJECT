#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5  // Grid size
#define MAX_WORDS 10
#define MAX_WORD_LEN 10

// Crossword Grid
char grid[SIZE][SIZE];

// Stack for Undo Feature
typedef struct {
    char word[MAX_WORD_LEN];
    int row, col;
    char direction;
} Move;

Move stack[MAX_WORDS];
int top = -1;

// Queue for Hint System
char wordQueue[MAX_WORDS][MAX_WORD_LEN];
int front = -1, rear = -1;

// Trie Node for Word Storage
typedef struct TrieNode {
    struct TrieNode* children[26];
    int isEndOfWord;
} TrieNode;

// Graph for Word Connections (Adjacency Matrix)
int graph[MAX_WORDS][MAX_WORDS];

// Function to create a new Trie node
TrieNode* createTrieNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    for (int i = 0; i < 26; i++)
        node->children[i] = NULL;
    node->isEndOfWord = 0;
    return node;
}

// Insert word into Trie
void insertWord(TrieNode* root, char* word) {
    TrieNode* temp = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'A';
        if (temp->children[index] == NULL)
            temp->children[index] = createTrieNode();
        temp = temp->children[index];
    }
    temp->isEndOfWord = 1;
}

// Search for word in Trie
int searchWord(TrieNode* root, char* word) {
    TrieNode* temp = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'A';
        if (temp->children[index] == NULL)
            return 0;
        temp = temp->children[index];
    }
    return temp->isEndOfWord;
}

// Initialize Grid
void initializeGrid() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            grid[i][j] = '.';
}

// Display Grid
void displayGrid() {
    printf("\nCrossword Grid:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            printf(" %c ", grid[i][j]);
        printf("\n");
    }
}

// Check if word can be placed
int canPlaceWord(char word[], int row, int col, char direction) {
    int len = strlen(word);
    if (direction == 'H' && (col + len > SIZE)) return 0;
    if (direction == 'V' && (row + len > SIZE)) return 0;

    for (int i = 0; i < len; i++) {
        if (direction == 'H' && grid[row][col + i] != '.' && grid[row][col + i] != word[i])
            return 0;
        if (direction == 'V' && grid[row + i][col] != '.' && grid[row + i][col] != word[i])
            return 0;
    }
    return 1;
}

// Backtracking function for automatic placement
int solveCrossword(char words[MAX_WORDS][MAX_WORD_LEN], int index, int totalWords) {
    if (index == totalWords) return 1; // All words placed

    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            for (char d = 'H'; d <= 'V'; d++) {
                if (canPlaceWord(words[index], r, c, d)) {
                    placeWord(words[index], r, c, d);
                    if (solveCrossword(words, index + 1, totalWords))
                        return 1; // Solution found
                    undoLastWord(); // Backtrack
                }
            }
        }
    }
    return 0;
}

// Place word in grid & Push to Undo Stack
void placeWord(char word[], int row, int col, char direction) {
    int len = strlen(word);

    // Store move in stack for undo
    top++;
    strcpy(stack[top].word, word);
    stack[top].row = row;
    stack[top].col = col;
    stack[top].direction = direction;

    // Place word in grid
    for (int i = 0; i < len; i++) {
        if (direction == 'H')
            grid[row][col + i] = word[i];
        else
            grid[row + i][col] = word[i];
    }
}

// Undo Last Word
void undoLastWord() {
    if (top == -1) {
        printf("\nNo moves to undo!\n");
        return;
    }

    Move lastMove = stack[top];
    int len = strlen(lastMove.word);

    // Remove word from grid
    for (int i = 0; i < len; i++) {
        if (lastMove.direction == 'H')
            grid[lastMove.row][lastMove.col + i] = '.';
        else
            grid[lastMove.row + i][lastMove.col] = '.';
    }

    top--;  // Pop from stack
    printf("\nLast move undone!\n");
}

// Enqueue a word into the Hint Queue
void addHint(char word[]) {
    if (rear == MAX_WORDS - 1) return; // Queue full
    if (front == -1) front = 0; // First word

    rear++;
    strcpy(wordQueue[rear], word);
}

// Show a Hint
void showHint() {
    if (front == -1 || front > rear) {
        printf("\nNo hints available!\n");
        return;
    }

    printf("\nHint: Try placing the word '%s'\n", wordQueue[front]);
    front++;  // Dequeue
}

// Main function
int main() {
    TrieNode* dictionary = createTrieNode();
    char words[MAX_WORDS][MAX_WORD_LEN];
    int totalWords;

    initializeGrid();

    printf("Enter number of words: ");
    scanf("%d", &totalWords);

    for (int i = 0; i < totalWords; i++) {
        printf("Enter word %d (uppercase only): ", i + 1);
        scanf("%s", words[i]);
        insertWord(dictionary, words[i]); // Insert into Trie
        addHint(words[i]); // Store word in hint system
    }

    if (solveCrossword(words, 0, totalWords)) {
        printf("\nCrossword Solved Successfully!\n");
    } else {
        printf("\nCould not solve the crossword. Try different words.\n");
    }

    displayGrid();

    while (1) {
        int choice;
        printf("\n1. Display Grid\n2. Undo Last Move\n3. Show Hint\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayGrid();
                break;
            case 2:
                undoLastWord();
                break;
            case 3:
                showHint();
                break;
            case 4:
                printf("\nExiting...\n");
                return 0;
            default:
                printf("\nInvalid choice. Try again.\n");
        }
    }
}
