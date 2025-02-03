#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

void generateKeyMatrix(char key[], char keyMatrix[SIZE][SIZE]) {
    int alphabet[26] = {0};
    int x = 0, y = 0;

    for (int i = 0; key[i] != '\0'; i++) {
        char ch = tolower(key[i]);
        if (ch == 'j') ch = 'i'; // Treat 'i' and 'j' as the same
        if (isalpha(ch) && !alphabet[ch - 'a']) {
            keyMatrix[x][y++] = ch;
            alphabet[ch - 'a'] = 1;
            if (y == SIZE) {
                y = 0;
                x++;
            }
        }
    }

    for (char ch = 'a'; ch <= 'z'; ch++) {
        if (ch == 'j') continue;
        if (!alphabet[ch - 'a']) {
            keyMatrix[x][y++] = ch;
            alphabet[ch - 'a'] = 1;
            if (y == SIZE) {
                y = 0;
                x++;
            }
        }
    }
}

void formatText(char text[]) {
    int len = strlen(text), index = 0;
    char formatted[100] = {0};
    
    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            formatted[index++] = tolower(text[i] == 'j' ? 'i' : text[i]);
        }
    }

    for (int i = 0; i < index; i += 2) {
        if (i + 1 < index && formatted[i] == formatted[i + 1]) {
            memmove(&formatted[i + 1], &formatted[i], index - i);
            formatted[i + 1] = 'x';
            index++;
        }
    }

    if (index % 2 != 0) {
        formatted[index++] = 'x';
    }

    formatted[index] = '\0';
    strcpy(text, formatted);
}

void findPosition(char keyMatrix[SIZE][SIZE], char ch, int *row, int *col) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (keyMatrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

void playfairCipher(char text[], char keyMatrix[SIZE][SIZE], int encrypt) {
    for (int i = 0; text[i] != '\0'; i += 2) {
        int r1, c1, r2, c2;
        findPosition(keyMatrix, text[i], &r1, &c1);
        findPosition(keyMatrix, text[i + 1], &r2, &c2);

        if (r1 == r2) {
            text[i] = keyMatrix[r1][(c1 + encrypt + SIZE) % SIZE];
            text[i + 1] = keyMatrix[r2][(c2 + encrypt + SIZE) % SIZE];
        } else if (c1 == c2) {
            text[i] = keyMatrix[(r1 + encrypt + SIZE) % SIZE][c1];
            text[i + 1] = keyMatrix[(r2 + encrypt + SIZE) % SIZE][c2];
        } else {
            text[i] = keyMatrix[r1][c2];
            text[i + 1] = keyMatrix[r2][c1];
        }
    }
}

int main() {
    char key[100], text[100], keyMatrix[SIZE][SIZE];

    printf("Enter key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';

    generateKeyMatrix(key, keyMatrix);
    formatText(text);

    printf("Choose an option:\n1. Encrypt\n2. Decrypt\n");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        playfairCipher(text, keyMatrix, 1);
        printf("Encrypted text: %s\n", text);
    } else if (choice == 2) {
        playfairCipher(text, keyMatrix, -1);
        printf("Decrypted text: %s\n", text);
    } else {
        printf("Invalid choice\n");
    }

    return 0;
}
