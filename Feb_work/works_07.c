#include <stdio.h>

int main() {
    int num, arr[20], i = 0, j, flag = 1;
    scanf("%d", &num);
    
    while (num > 0) {
        arr[i++] = num % 10;
        num /= 10;
    }
    arr[i] = -1;
    
    for (j = 0; j < i / 2; j++) {
        if (arr[j] != arr[i - 1 - j]) {
            flag = 0;
            break;
        }
    }
    
    printf("%s\n", flag ? "Yes" : "No");
    return 0;
}