#include "viewer.h"
#include <stdio.h>

int admin_ui()
{
    int op = 0;
    printf("\n==== 管理员菜单 ====\n");
    printf("1. 新增用户\n");
    printf("2. 图书入库\n");
    printf("3. 新增订单\n");
    printf("4. 关闭订单\n");
    printf("5. 月销售额查询\n");
    printf("6. 年销售额查询\n");
    printf("7. 热度查询\n");
    printf("8. 修改密码\n");
    printf("0. 返回主菜单\n");
    printf("====================\n");
    op = get_user_choice();
    return op;
}
