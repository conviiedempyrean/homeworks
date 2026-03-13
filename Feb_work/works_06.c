#include <stdio.h>

int main() {
    char str[] = "Hello World 123!@#";
    int letters = 0, spaces = 0, digits = 0, others = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
            letters++;
        else if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
            spaces++;
        else if (str[i] >= '0' && str[i] <= '9')
            digits++;
        else
            others++;
    }
    
    printf("Letters: %d\n", letters);
    printf("Spaces: %d\n", spaces);
    printf("Digits: %d\n", digits);
    printf("Others: %d\n", others);
    
    return 0;
}