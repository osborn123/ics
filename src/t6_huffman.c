#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 100
#define MAX_SYMBOLS 26


void huffman(char input_text[]) {
}

// Driver code. You should not need to modify this.
int main() {
    FILE *file_pointer = fopen("task6.txt", "r");
    char *input = malloc(sizeof(char) * 1000);

    fgets(input, 1000, file_pointer);

    fclose(file_pointer);

    huffman(input);
    free(input);
    return 0;
}
