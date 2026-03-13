#include <stdio.h>

int main() {
    int a = 1, b = 1, c;
    
    printf("1 month: 1 pair\n");
    printf("2 months: 1 pair\n");
    
    for (int i = 3; i <= 20; i++) {
        c = a + b;
        printf("%d months: %d pair\n", i, c);
        a = b;
        b = c;
    }
    
    return 0;
}