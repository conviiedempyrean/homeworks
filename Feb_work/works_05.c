#include <stdio.h>

int main() {
    int m, n, a, b, temp;
    scanf("%d %d", &m, &n);
    a = m;
    b = n;
    
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    
    printf("GCD: %d\n", a);
    printf("LCM: %d\n", m * n / a);
    
    return 0;
}