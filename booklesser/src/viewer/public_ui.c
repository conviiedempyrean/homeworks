#include "viewer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_welcome()
{
    printf("====================================\n");
    printf("  欢迎使用 BookLessor 图书租赁系统  \n");
    printf("====================================\n\n");
}

void show_goodbye()
{
    printf("\n感谢使用，再见！\n");
}

void pause_screen()
{
    printf("\n按 Enter 键继续...");
    while (getchar() != '\n');
}

void clear_screen()
{
    /* 简单清屏：输出多个换行 */
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

int get_user_choice()
{
    int op = 0;
    printf("请输入选项：");
    scanf("%d", &op);
    while (getchar() != '\n');
    return op;
}

void ui_show_books(struct node **ppbooks, int cnt)
{
    int i = 0;
    struct bookinfo *pb = NULL;

    if (cnt <= 0 || ppbooks == NULL)
    {
        printf("暂无图书信息。\n");
        return;
    }
    printf("%-16s %-20s %-8s %-8s %-12s %-8s %-8s %-8s\n",
           "ISBN", "书名", "册数", "版号", "出版日期", "库存", "原价", "租价");
    for (i = 0; i < cnt; i++)
    {
        if (ppbooks[i] != NULL && ppbooks[i]->pdata != NULL)
        {
            pb = (struct bookinfo *)ppbooks[i]->pdata;
            printf("%-16s %-20s %-8d %-8d %04d-%02d-%02d   %-8d %-8.2f %-8.2f\n",
                   pb->isbn, pb->name, pb->volume, pb->version,
                   pb->pubdate.y, pb->pubdate.m, pb->pubdate.d,
                   pb->stock, pb->sprice, pb->lprice);
        }
    }
}

void ui_show_orders(struct node **pporders, int cnt)
{
    int i = 0;
    struct orderinfo *po = NULL;
    const char *status_str = NULL;
    const char *hway_str = NULL;

    if (cnt <= 0 || pporders == NULL)
    {
        printf("暂无订单信息。\n");
        return;
    }
    printf("%-12s %-16s %-12s %-10s %-12s %-10s %-10s\n",
           "租用日期", "ISBN", "用户名", "状态", "归还日期", "处理方式", "结算金额");
    for (i = 0; i < cnt; i++)
    {
        if (pporders[i] != NULL && pporders[i]->pdata != NULL)
        {
            po = (struct orderinfo *)pporders[i]->pdata;
            status_str = (po->status == OPENED) ? "未关闭" : "已关闭";
            switch (po->hway)
            {
                case NORMAL: hway_str = "正常归还"; break;
                case REDRESS: hway_str = "损毁赔偿"; break;
                default: hway_str = "未处理"; break;
            }
            printf("%04d-%02d-%02d   %-16s %-12s %-10s %04d-%02d-%02d   %-10s %-10.2f\n",
                   po->ldate.y, po->ldate.m, po->ldate.d,
                   po->isbn, po->username, status_str,
                   po->rdate.y, po->rdate.m, po->rdate.d,
                   hway_str, po->lastprice);
        }
    }
}

void ui_show_users(struct node **ppusers, int cnt)
{
    int i = 0;
    struct userinfo *pu = NULL;
    const char *type_str = NULL;

    if (cnt <= 0 || ppusers == NULL)
    {
        printf("暂无用户信息。\n");
        return;
    }
    printf("%-12s %-12s %-10s %-14s %-18s %-12s %-12s\n",
           "用户名", "密码", "类型", "联系方式", "证件号码", "证件姓名", "出生日期");
    for (i = 0; i < cnt; i++)
    {
        if (ppusers[i] != NULL && ppusers[i]->pdata != NULL)
        {
            pu = (struct userinfo *)ppusers[i]->pdata;
            switch (pu->type)
            {
                case UT_STU: type_str = "学生"; break;
                case UT_TCH: type_str = "老师"; break;
                case UT_ADMIN: type_str = "管理员"; break;
                default: type_str = "未知"; break;
            }
            printf("%-12s %-12s %-10s %-14s %-18s %-12s %04d-%02d-%02d\n",
                   pu->username, pu->password, type_str,
                   pu->phone, pu->paper, pu->cardname,
                   pu->birth.y, pu->birth.m, pu->birth.d);
        }
    }
}

int ui_input_bookinfo(struct bookinfo *pdata)
{
    if (pdata == NULL)
    {
        return -1;
    }
    memset(pdata, 0, sizeof(struct bookinfo));
    printf("请输入 ISBN（13 位数字）：");
    scanf("%s", pdata->isbn);
    printf("请输入书名：");
    scanf("%s", pdata->name);
    printf("请输入册数：");
    scanf("%d", &pdata->volume);
    printf("请输入版号：");
    scanf("%d", &pdata->version);
    printf("请输入出版日期（年 月 日）：");
    scanf("%d %d %d", &pdata->pubdate.y, &pdata->pubdate.m, &pdata->pubdate.d);
    printf("请输入库存数量：");
    scanf("%d", &pdata->stock);
    printf("请输入原价：");
    scanf("%f", &pdata->sprice);
    printf("请输入租用价：");
    scanf("%f", &pdata->lprice);
    pdata->lcnt = 0;
    while (getchar() != '\n');
    return 0;
}

int ui_input_userinfo(struct userinfo *pdata)
{
    if (pdata == NULL)
    {
        return -1;
    }
    memset(pdata, 0, sizeof(struct userinfo));
    printf("请输入用户名：");
    scanf("%s", pdata->username);
    printf("请输入密码：");
    scanf("%s", pdata->password);
    printf("请输入用户类型（0-学生 1-老师 2-管理员）：");
    scanf("%d", &pdata->type);
    printf("请输入联系方式：");
    scanf("%s", pdata->phone);
    printf("请输入证件号码：");
    scanf("%s", pdata->paper);
    printf("请输入证件姓名：");
    scanf("%s", pdata->cardname);
    printf("请输入出生日期（年 月 日）：");
    scanf("%d %d %d", &pdata->birth.y, &pdata->birth.m, &pdata->birth.d);
    while (getchar() != '\n');
    return 0;
}

int ui_input_orderinfo(struct orderinfo *pdata)
{
    if (pdata == NULL)
    {
        return -1;
    }
    memset(pdata, 0, sizeof(struct orderinfo));
    printf("请输入租用日期（年 月 日）：");
    scanf("%d %d %d", &pdata->ldate.y, &pdata->ldate.m, &pdata->ldate.d);
    printf("请输入 ISBN：");
    scanf("%s", pdata->isbn);
    printf("请输入用户名：");
    scanf("%s", pdata->username);
    pdata->status = OPENED;
    pdata->hway = UNHANDLED;
    pdata->lastprice = 0.0f;
    while (getchar() != '\n');
    return 0;
}

int ui_input_date(struct date *pd)
{
    if (pd == NULL)
    {
        return -1;
    }
    printf("请输入日期（年 月 日）：");
    scanf("%d %d %d", &pd->y, &pd->m, &pd->d);
    while (getchar() != '\n');
    return 0;
}

int ui_input_close_info(struct date *rdate, int *hway, float *price)
{
    if (rdate == NULL || hway == NULL || price == NULL)
    {
        return -1;
    }
    printf("请输入归还日期（年 月 日）：");
    scanf("%d %d %d", &rdate->y, &rdate->m, &rdate->d);
    printf("请选择处理方式（1-正常归还 2-损毁赔偿）：");
    scanf("%d", hway);
    if (*hway == REDRESS)
    {
        printf("请输入赔偿金额：");
        scanf("%f", price);
    }
    else
    {
        *hway = NORMAL;
        *price = 0.0f;
    }
    while (getchar() != '\n');
    return 0;
}

void ui_query_book_by_name()
{
    /* 控制器直接调用 search_book_by_name，本函数仅作为桩 */
}

void ui_query_book_by_heat()
{
    /* 控制器直接调用 sort_books_by_lcnt，本函数仅作为桩 */
}

void ui_query_sales()
{
    /* 控制器直接调用 count_month_sales / count_year_sales，本函数仅作为桩 */
}

void ui_query_heat()
{
    /* 控制器直接调用 sort_books_by_lcnt，本函数仅作为桩 */
}
