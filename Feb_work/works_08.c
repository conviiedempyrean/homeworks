#include <stdio.h>

int main() {
    char str[] = "aca";
    char ch = 'a';
    char result[100];
    int j = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ch) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    
    printf("%s\n", result);
    return 0;
}