#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void count_words(char input_text[]) {
    int wordcount = 0;
    int i = 0;

    while (input_text[i] != '\0') {
        

            if (input_text[i + 1] == ' ' || input_text[i + 1] == '\n' || input_text[i + 1] == '\0') {
                wordcount++;
            }

        i++;
        
    }
    printf("%d\n",wordcount-1);




    
    // TODO: Implement me!
}

// Driver code. You should not need to modify this.
int main() {
    FILE *file_pointer = fopen("task1.txt", "r");
    char *input = malloc(sizeof(char) * 1000);

    fgets(input, 1000, file_pointer);

    fclose(file_pointer);

    count_words(input);
    free(input);
    return 0;
}
