#include "ctrler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TCH_MAX_OPEN 5
#define TCH_DISCOUNT 0.9f

int tea_ctl(struct node **users, struct bookslinklist *books, struct node **orders)
{
    struct node *puser_node = NULL;
    struct userinfo *puser = NULL;
    int cmd = 0;
    int backflag = 0;
    char name_buf[BOOK_NAME_LEN] = {0};
    struct node *out_buf[100] = {NULL};
    int cnt = 0;
    struct node **ppsorted = NULL;
    struct node **pporders = NULL;
    int order_cnt = 0;

    if (prompt_login(users, UT_TCH, &puser_node) != 0)
    {
        return -1;
    }
    puser = (struct userinfo *)puser_node->pdata;

    while (1)
    {
        cmd = tea_ui();
        switch (cmd)
        {
            case 1:
                printf("请输入要查询的书名关键字：");
                scanf("%s", name_buf);
                while (getchar() != '\n');
                cnt = search_book_by_name(books, name_buf, out_buf, 100);
                ui_show_books(out_buf, cnt);
                pause_screen();
                break;

            case 2:
                ppsorted = sort_books_by_lcnt(books);
                if (ppsorted != NULL)
                {
                    ui_show_books(ppsorted, books->cnt);
                    free(ppsorted);
                    ppsorted = NULL;
                }
                else
                {
                    printf("暂无图书数据。\n");
                }
                pause_screen();
                break;

            case 3:
                pporders = search_opened_order_by_name(orders, puser->username, &order_cnt);
                printf("\n未关闭订单：\n");
                ui_show_orders(pporders, order_cnt);
                if (pporders != NULL)
                {
                    free(pporders);
                    pporders = NULL;
                }
                pporders = search_closed_order_by_name(orders, puser->username, &order_cnt);
                printf("\n已关闭订单：\n");
                ui_show_orders(pporders, order_cnt);
                if (pporders != NULL)
                {
                    free(pporders);
                    pporders = NULL;
                }
                pause_screen();
                break;

            case 4:
                create_order_flow(users, books, orders, puser, TCH_DISCOUNT, TCH_MAX_OPEN);
                pause_screen();
                break;

            case 5:
                close_order_flow(users, books, orders, puser->username);
                pause_screen();
                break;

            case 6:
                printf("请输入新密码：");
                scanf("%s", puser->password);
                while (getchar() != '\n');
                printf("密码修改成功。\n");
                pause_screen();
                break;

            case 0:
                backflag = 1;
                break;

            default:
                printf("无效选项。\n");
                pause_screen();
                break;
        }
        if (backflag)
        {
            break;
        }
    }
    return 0;
}
