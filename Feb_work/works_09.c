#include <stdio.h>

void input(int a[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            scanf("%d", &a[i][j]);
}

int main() {
    int a[3][3], sum = 0;
    input(a);
    
    for (int i = 0; i < 3; i++)
        sum += a[i][i] + a[i][2 - i];
    
    printf("%d\n", sum - a[1][1]);
    return 0;
}