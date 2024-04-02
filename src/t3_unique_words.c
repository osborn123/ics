#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORS 100
#define MAX_WORD_LENGTH 30


void unique_words(char input_text[]) {
    
    char words[MAX_WORS][MAX_WORD_LENGTH];
    int unique_word_count = 0;

    int i = 0, j = 0, k;
    while (input_text[i] != '\0') {
        // Collect the characters of a word
        while (input_text[i] != ' ' && input_text[i] != '\0') {
            words[unique_word_count][j] = input_text[i];
            j++;
            i++;
        }
        words[unique_word_count][j] = '\0';  // Null terminate the collected word

        // Check if the word is unique
        int is_unique = 1;
        for (k = 0; k < unique_word_count; k++) {
            if (strcmp(words[k], words[unique_word_count]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            printf("%d %s\n", unique_word_count + 1, words[unique_word_count]);
            unique_word_count++;
        }

        // Reset for the next word
        j = 0;
        if (input_text[i] != '\0') {
            i++;  // Skip the space character
        }
    }
    // TODO: Implement me!
}

// Driver code. You should not need to modify this.
int main() {
    FILE *file_pointer = fopen("task3.txt", "r");
    char *input = malloc(sizeof(char) * 1000);

    fgets(input, 1000, file_pointer);

    fclose(file_pointer);

    unique_words(input);
    free(input);
    return 0;
}
