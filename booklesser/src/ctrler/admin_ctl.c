#include "ctrler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 管理员新增订单时，按用户类型决定折扣与上限 */
static int get_user_discount_and_limit(struct userinfo *puser, float *discount, int *max_open)
{
    if (puser == NULL || discount == NULL || max_open == NULL)
    {
        return -1;
    }
    if (puser->type == UT_STU)
    {
        *discount = 0.8f;
        *max_open = 3;
    }
    else if (puser->type == UT_TCH)
    {
        *discount = 0.9f;
        *max_open = 5;
    }
    else
    {
        return -1;
    }
    return 0;
}

int admin_ctl(struct node **users, struct bookslinklist *books, struct node **orders)
{
    struct node *puser_node = NULL;
    struct userinfo *padmin = NULL;
    int cmd = 0;
    int backflag = 0;
    struct userinfo uinfo;
    struct bookinfo binfo;
    struct node *pnew = NULL;
    char name_buf[BOOK_NAME_LEN] = {0};
    struct node *out_buf[100] = {NULL};
    int cnt = 0;
    struct node **ppsorted = NULL;
    struct date qdate;
    int year = 0;
    float sales = 0.0f;
    char target_user[USER_NAME_LEN] = {0};
    struct node *ptarget_user = NULL;
    struct userinfo *ptu = NULL;
    float discount = 1.0f;
    int max_open = 0;

    if (prompt_login(users, UT_ADMIN, &puser_node) != 0)
    {
        return -1;
    }
    padmin = (struct userinfo *)puser_node->pdata;

    while (1)
    {
        cmd = admin_ui();
        switch (cmd)
        {
            case 1:
                /* 新增用户 */
                if (ui_input_userinfo(&uinfo) == 0)
                {
                    if (uinfo.type < UT_STU || uinfo.type > UT_ADMIN)
                    {
                        printf("无效的用户类型。\n");
                        pause_screen();
                        break;
                    }
                    if (search_user_by_name(users, uinfo.type, uinfo.username) != NULL)
                    {
                        printf("该用户名已存在。\n");
                        pause_screen();
                        break;
                    }
                    pnew = create_user_node(&uinfo);
                    if (pnew != NULL)
                    {
                        insert_node_at_tail(&users[UserHash(uinfo.type)], pnew);
                        printf("用户新增成功。\n");
                    }
                }
                pause_screen();
                break;

            case 2:
                /* 图书入库 */
                if (ui_input_bookinfo(&binfo) == 0)
                {
                    if (add_book(books, &binfo) == 0)
                    {
                        printf("图书入库成功。\n");
                    }
                    else
                    {
                        printf("ISBN 已存在，入库失败。\n");
                    }
                }
                pause_screen();
                break;

            case 3:
                /* 新增订单：管理员代某用户租书 */
                printf("请输入要租借的用户名：");
                scanf("%s", target_user);
                while (getchar() != '\n');
                ptarget_user = NULL;
                for (int ut = UT_STU; ut <= UT_TCH; ut++)
                {
                    ptarget_user = search_user_by_name(users, ut, target_user);
                    if (ptarget_user != NULL) break;
                }
                if (ptarget_user == NULL || ptarget_user->pdata == NULL)
                {
                    printf("未找到该用户（仅支持学生或老师）。\n");
                    pause_screen();
                    break;
                }
                ptu = (struct userinfo *)ptarget_user->pdata;
                if (get_user_discount_and_limit(ptu, &discount, &max_open) != 0)
                {
                    printf("该用户类型不支持租书。\n");
                    pause_screen();
                    break;
                }
                create_order_flow(users, books, orders, ptu, discount, max_open);
                pause_screen();
                break;

            case 4:
                /* 关闭订单 */
                close_order_flow(users, books, orders, NULL);
                pause_screen();
                break;

            case 5:
                /* 月销售额查询 */
                if (ui_input_date(&qdate) == 0)
                {
                    sales = count_month_sales(orders, qdate.y, qdate.m);
                    printf("%d 年 %d 月销售额：%.2f\n", qdate.y, qdate.m, sales);
                }
                pause_screen();
                break;

            case 6:
                /* 年销售额查询 */
                printf("请输入年份：");
                scanf("%d", &year);
                while (getchar() != '\n');
                sales = count_year_sales(orders, year);
                printf("%d 年销售额：%.2f\n", year, sales);
                pause_screen();
                break;

            case 7:
                /* 热度查询 */
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

            case 8:
                /* 修改管理员密码 */
                printf("请输入新密码：");
                scanf("%s", padmin->password);
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
