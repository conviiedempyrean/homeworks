#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    char surname[100];
    char givenname[100];
    char input[256];

    printf("请输入姓：");
    if (scanf("%99s", surname) != 1) {
        return 1;
    }

    printf("请输入名：");
    if (scanf("%99s", givenname) != 1) {
        return 1;
    }

    printf("中文名组合结果：%s %s\n", surname, givenname);
}
