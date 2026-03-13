#include <stdio.h>

int main() {
    int n, bin[32], i = 0;
    printf("Enter a decimal number: ");
    scanf("%d", &n);
    
    if (n == 0) {
        printf("Binary: 0\n");
        return 0;
    }
    
    while (n > 0) {
        bin[i++] = n % 2;
        n /= 2;
    }
    
    printf("Binary: ");
    for (int j = i - 1; j >= 0; j--)
        printf("%d", bin[j]);
    printf("\n");
    
    return 0;
}