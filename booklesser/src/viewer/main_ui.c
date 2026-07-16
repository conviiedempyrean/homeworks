#include "viewer.h"
#include <stdio.h>

int main_ui()
{
    int op = 0;
    printf("\n==== 主菜单 ====\n");
    printf("1. 学生登录\n");
    printf("2. 老师登录\n");
    printf("3. 管理员登录\n");
    printf("0. 退出\n");
    printf("================\n");
    op = get_user_choice();
    return op;
}
