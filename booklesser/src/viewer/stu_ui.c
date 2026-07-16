#include "viewer.h"
#include <stdio.h>

int stu_ui()
{
    int op = 0;
    printf("\n==== 学生菜单 ====\n");
    printf("1. 按书名查询图书\n");
    printf("2. 按热度查询图书\n");
    printf("3. 查看我的订单\n");
    printf("4. 租书\n");
    printf("5. 还书\n");
    printf("6. 修改密码\n");
    printf("0. 返回主菜单\n");
    printf("==================\n");
    op = get_user_choice();
    return op;
}
