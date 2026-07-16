#include "ctrler.h"
#include <stdio.h>
#include <string.h>

/* 循环提示登录，直到成功或用户输入空用户名放弃 */
int prompt_login(struct node **users, int expected_type, struct node **out_user)
{
    char username[USER_NAME_LEN] = {0};
    char password[PASS_LEN] = {0};
    int type = 0;
    struct node *puser = NULL;

    if (users == NULL || out_user == NULL)
    {
        return -1;
    }
    *out_user = NULL;

    while (1)
    {
        printf("请输入用户名（直接回车放弃）：");
        if (scanf("%19s", username) != 1)
        {
            while (getchar() != '\n');
            return -1;
        }
        printf("请输入密码：");
        scanf("%19s", password);
        while (getchar() != '\n');

        puser = verify_user_login(users, username, password, &type);
        if (puser != NULL && type == expected_type)
        {
            *out_user = puser;
            printf("登录成功！\n");
            return 0;
        }
        printf("用户名或密码错误，请重试。\n");
    }
}

/* 租书流程：输入 ISBN，检查库存与未关闭订单上限，创建订单 */
int create_order_flow(struct node **users,
                      struct bookslinklist *books,
                      struct node **orders,
                      struct userinfo *current_user,
                      float discount,
                      int max_open)
{
    char isbn[ISBN_BUF_LEN] = {0};
    struct node *pbook_node = NULL;
    struct bookinfo *pbook = NULL;
    struct orderinfo oinfo;
    struct node *porder = NULL;
    struct node **ppopened = NULL;
    int opened_cnt = 0;
    int cnt = 0;

    if (users == NULL || books == NULL || orders == NULL || current_user == NULL)
    {
        return -1;
    }

    printf("请输入要租借的图书 ISBN：");
    scanf("%s", isbn);
    while (getchar() != '\n');

    pbook_node = search_book_by_isbn(books, isbn);
    if (pbook_node == NULL || pbook_node->pdata == NULL)
    {
        printf("图书不存在。\n");
        return -1;
    }
    pbook = (struct bookinfo *)pbook_node->pdata;
    if (pbook->stock <= 0)
    {
        printf("图书库存不足。\n");
        return -1;
    }

    /* 检查当前用户未关闭订单数量 */
    ppopened = search_opened_order_by_name(orders, current_user->username, &opened_cnt);
    if (ppopened != NULL)
    {
        cnt = opened_cnt;
        free(ppopened);
        ppopened = NULL;
    }
    if (cnt >= max_open)
    {
        printf("您当前已有 %d 个未关闭订单，达到上限 %d，无法继续租书。\n", cnt, max_open);
        return -1;
    }

    memset(&oinfo, 0, sizeof(oinfo));
    printf("请输入租用日期（年 月 日）：");
    scanf("%d %d %d", &oinfo.ldate.y, &oinfo.ldate.m, &oinfo.ldate.d);
    while (getchar() != '\n');
    strncpy(oinfo.isbn, isbn, ISBN_BUF_LEN - 1);
    strncpy(oinfo.username, current_user->username, USER_NAME_LEN - 1);
    oinfo.status = OPENED;
    oinfo.hway = UNHANDLED;
    oinfo.lastprice = pbook->lprice * discount;

    if (modify_book_stock(books, isbn, -1) != 0)
    {
        printf("库存扣减失败。\n");
        return -1;
    }
    pbook->lcnt++;

    porder = create_order_node(&oinfo);
    if (porder == NULL)
    {
        /* 回滚库存 */
        modify_book_stock(books, isbn, 1);
        pbook->lcnt--;
        return -1;
    }
    insert_node_at_tail(&orders[OrderHash(OPENED)], porder);
    printf("租书成功，结算价：%.2f\n", oinfo.lastprice);
    return 0;
}

/* 还书/赔偿流程 */
int close_order_flow(struct node **users,
                     struct bookslinklist *books,
                     struct node **orders,
                     const char *restrict_username)
{
    char isbn[ISBN_BUF_LEN] = {0};
    char username[USER_NAME_LEN] = {0};
    struct node **ppopened = NULL;
    int opened_cnt = 0;
    struct node *ptarget = NULL;
    struct orderinfo *poinfo = NULL;
    struct date rdate;
    int hway = 0;
    float price = 0.0f;
    int i = 0;

    if (orders == NULL)
    {
        return -1;
    }

    printf("请输入要关闭订单的 ISBN：");
    scanf("%s", isbn);
    printf("请输入订单所属用户名：");
    scanf("%s", username);
    while (getchar() != '\n');

    if (restrict_username != NULL && strcmp(username, restrict_username) != 0)
    {
        printf("您只能关闭自己的订单。\n");
        return -1;
    }

    ppopened = search_opened_order_by_name(orders, username, &opened_cnt);
    if (ppopened == NULL || opened_cnt <= 0)
    {
        printf("未找到该用户的未关闭订单。\n");
        if (ppopened != NULL) free(ppopened);
        return -1;
    }

    for (i = 0; i < opened_cnt; i++)
    {
        if (ppopened[i] != NULL && ppopened[i]->pdata != NULL)
        {
            poinfo = (struct orderinfo *)ppopened[i]->pdata;
            if (strcmp(poinfo->isbn, isbn) == 0)
            {
                ptarget = ppopened[i];
                break;
            }
        }
    }
    free(ppopened);
    ppopened = NULL;

    if (ptarget == NULL)
    {
        printf("未找到指定 ISBN 的未关闭订单。\n");
        return -1;
    }

    if (ui_input_close_info(&rdate, &hway, &price) != 0)
    {
        return -1;
    }

    poinfo = (struct orderinfo *)ptarget->pdata;
    if (hway == NORMAL)
    {
        /* 正常归还，结算价为租价 */
        price = poinfo->lastprice;
        if (modify_book_stock(books, isbn, 1) != 0)
        {
            printf("库存恢复失败。\n");
            return -1;
        }
    }

    if (close_order(ptarget, &rdate, hway, price) != 0)
    {
        return -1;
    }

    /* 从 opened 链表移到 closed 链表 */
    remove_node(&orders[OrderHash(OPENED)], ptarget);
    insert_node_at_tail(&orders[OrderHash(CLOSED)], ptarget);
    printf("订单关闭成功，结算金额：%.2f\n", price);
    return 0;
}
