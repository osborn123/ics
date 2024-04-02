#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void print_words(char input_text[]) {

int word_start = 0; 
    int length = strlen(input_text);

    for (int i = 0; i < length; i++) {
        if (input_text[i] == ' ' || i == length - 1) {
            
            for (int j = word_start; j <= i; j++) {
                if (input_text[j] != ' ') {
                    printf("%c", input_text[j]);
                }
            }
            if (i != length - 1) {
                printf("\n");
            }
            word_start = i + 1; 
        }
    }
  
    

    // for ( int i = 0;input_text[i] != '\0'; i++) {
    //     if (input_text[i] == ' ')
    //     {
    //         printf("\n");
    //     } 
    //     else{
    //         printf("%c ", input_text[i]);
    //     }
       
    // }

    

}
    
    
    // TODO: Implement me!

 
// Driver code. You should not need to modify this.
int main() {
    FILE *file_pointer = fopen("task2.txt", "r");
    char *input = malloc(sizeof(char) * 1000);

    fgets(input, 1000, file_pointer);

    fclose(file_pointer);

    print_words(input);
    free(input);
    return 0;
}
