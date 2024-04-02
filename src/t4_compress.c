#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORS 100
#define MAX_WORD_LENGTH 30


void compress(char input_text[]) {
    // TODO: Implement me!
}

// Driver code. You should not need to modify this.
int main() {
    FILE *file_pointer = fopen("task4.txt", "r");
    char *input = malloc(sizeof(char) * 1000);

    fgets(input, 1000, file_pointer);

    fclose(file_pointer);

    compress(input);
    free(input);
    return 0;
}
