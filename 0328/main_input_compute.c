#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 4) {
        printf("用法: %s 操作数1 运算符 操作数2\n", argv[0]);
        printf("例如: %s 21 + 34\n", argv[0]);
        return 1;
    }

    char *endptr;
    long a = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0') {
        printf("错误: 操作数1 不是有效整数\n");
        return 1;
    }

    char *op = argv[2];
    long b = strtol(argv[3], &endptr, 10);
    if (*endptr != '\0') {
        printf("错误: 操作数2 不是有效整数\n");
        return 1;
    }

    long result;
    if (strcmp(op, "+") == 0) {
        result = a + b;
    } else if (strcmp(op, "-") == 0) {
        result = a - b;
    } else if (strcmp(op, "*") == 0 || strcmp(op, "x") == 0 || strcmp(op, "X") == 0) {
        result = a * b;
    } else if (strcmp(op, "/") == 0) {
        if (b == 0) {
            printf("错误: 除数不能为零\n");
            return 1;
        }
        result = a / b;
    } else if (strcmp(op, "%%") == 0) {
        if (b == 0) {
            printf("错误: 取模除数不能为零\n");
            return 1;
        }
        result = a % b;
    } else {
        printf("错误: 不支持的运算符 '%s'\n", op);
        return 1;
    }

    printf("%ld %s %ld = %ld\n", a, op, b, result);
    return 0;
}
