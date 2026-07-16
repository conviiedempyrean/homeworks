#include "order_model.h"

/* 创建订单节点 */
struct node *create_order_node(struct orderinfo *pdata)
{
    struct node *pnode = NULL;
    struct orderinfo *porder = NULL;

    if (pdata == NULL)
    {
        return NULL;
    }
    pnode = (struct node *)malloc(sizeof(struct node));
    if (pnode == NULL)
    {
        return NULL;
    }
    porder = (struct orderinfo *)malloc(sizeof(struct orderinfo));
    if (porder == NULL)
    {
        free(pnode);
        return NULL;
    }
    memcpy(porder, pdata, sizeof(struct orderinfo));
    pnode->pdata = (void *)porder;
    pnode->infotype = DT_ORDER;
    pnode->pnext = NULL;
    return pnode;
}

/* 从二进制文件读取订单链表 */
int read_order_from_file(struct node **pphead, char *filename)
{
    FILE *pf = NULL;
    struct orderinfo info;
    struct node *pnode = NULL;
    int cnt = 0;

    if (pphead == NULL || filename == NULL)
    {
        return -1;
    }
    clear_linklist(pphead);

    pf = fopen(filename, "rb");
    if (pf == NULL)
    {
        return 0;
    }
    while (fread(&info, sizeof(struct orderinfo), 1, pf) == 1)
    {
        pnode = create_order_node(&info);
        if (pnode == NULL)
        {
            fclose(pf);
            return -1;
        }
        insert_node_at_tail(pphead, pnode);
        cnt++;
    }
    fclose(pf);
    return cnt;
}

/* 保存订单链表到二进制文件 */
int save_order_to_file(struct node *phead, char *filename)
{
    FILE *pf = NULL;
    struct node *p = phead;
    int cnt = 0;

    if (filename == NULL)
    {
        return -1;
    }
    pf = fopen(filename, "wb");
    if (pf == NULL)
    {
        return -1;
    }
    while (p != NULL)
    {
        if (p->pdata != NULL)
        {
            fwrite((struct orderinfo *)p->pdata, sizeof(struct orderinfo), 1, pf);
            cnt++;
        }
        p = p->pnext;
    }
    fclose(pf);
    return cnt;
}

/* 读取全部订单到哈希数组（0-已关闭，1-未关闭） */
int read_all_order(struct node **pphasharr, int cnt)
{
    if (pphasharr == NULL || cnt < 2)
    {
        return -1;
    }
    if (read_order_from_file(&(pphasharr[OrderHash(CLOSED)]), CLOSED_ORDER_FILE) < 0)
    {
        return -1;
    }
    if (read_order_from_file(&(pphasharr[OrderHash(OPENED)]), OPENED_ORDER_FILE) < 0)
    {
        return -1;
    }
    return 0;
}

/* 保存全部订单到文件 */
int save_all_order(struct node **pphasharr, int cnt)
{
    if (pphasharr == NULL || cnt < 2)
    {
        return -1;
    }
    if (save_order_to_file(pphasharr[OrderHash(CLOSED)], CLOSED_ORDER_FILE) < 0)
    {
        return -1;
    }
    if (save_order_to_file(pphasharr[OrderHash(OPENED)], OPENED_ORDER_FILE) < 0)
    {
        return -1;
    }
    return 0;
}

/* 统计指定用户名在未关闭订单中的数量 */
static int count_opened_order_by_name(struct node **pphasharr, char *pusername)
{
    struct node *p = NULL;
    struct orderinfo *porder = NULL;
    int cnt = 0;

    p = pphasharr[OrderHash(OPENED)];
    while (p != NULL)
    {
        if (p->pdata != NULL)
        {
            porder = (struct orderinfo *)p->pdata;
            if (strcmp(porder->username, pusername) == 0)
            {
                cnt++;
            }
        }
        p = p->pnext;
    }
    return cnt;
}

/* 按用户名查询未关闭订单，返回节点指针数组 */
struct node **search_opened_order_by_name(struct node **pphasharr, char *pusername, int *pcnt)
{
    struct node *p = NULL;
    struct orderinfo *porder = NULL;
    struct node **ppout = NULL;
    int total = 0;
    int i = 0;

    if (pphasharr == NULL || pusername == NULL || pcnt == NULL)
    {
        return NULL;
    }
    *pcnt = 0;
    total = count_opened_order_by_name(pphasharr, pusername);
    if (total <= 0)
    {
        return NULL;
    }
    ppout = (struct node **)malloc(sizeof(struct node *) * total);
    if (ppout == NULL)
    {
        return NULL;
    }
    p = pphasharr[OrderHash(OPENED)];
    while (p != NULL && i < total)
    {
        if (p->pdata != NULL)
        {
            porder = (struct orderinfo *)p->pdata;
            if (strcmp(porder->username, pusername) == 0)
            {
                ppout[i++] = p;
            }
        }
        p = p->pnext;
    }
    *pcnt = i;
    return ppout;
}

/* 统计指定用户名在已关闭订单中的数量 */
static int count_closed_order_by_name(struct node **pphasharr, char *pusername)
{
    struct node *p = NULL;
    struct orderinfo *porder = NULL;
    int cnt = 0;

    p = pphasharr[OrderHash(CLOSED)];
    while (p != NULL)
    {
        if (p->pdata != NULL)
        {
            porder = (struct orderinfo *)p->pdata;
            if (strcmp(porder->username, pusername) == 0)
            {
                cnt++;
            }
        }
        p = p->pnext;
    }
    return cnt;
}

/* 按用户名查询已关闭订单，返回节点指针数组 */
struct node **search_closed_order_by_name(struct node **pphasharr, char *pusername, int *pcnt)
{
    struct node *p = NULL;
    struct orderinfo *porder = NULL;
    struct node **ppout = NULL;
    int total = 0;
    int i = 0;

    if (pphasharr == NULL || pusername == NULL || pcnt == NULL)
    {
        return NULL;
    }
    *pcnt = 0;
    total = count_closed_order_by_name(pphasharr, pusername);
    if (total <= 0)
    {
        return NULL;
    }
    ppout = (struct node **)malloc(sizeof(struct node *) * total);
    if (ppout == NULL)
    {
        return NULL;
    }
    p = pphasharr[OrderHash(CLOSED)];
    while (p != NULL && i < total)
    {
        if (p->pdata != NULL)
        {
            porder = (struct orderinfo *)p->pdata;
            if (strcmp(porder->username, pusername) == 0)
            {
                ppout[i++] = p;
            }
        }
        p = p->pnext;
    }
    *pcnt = i;
    return ppout;
}

/* 在指定订单链表中按ISBN查找节点 */
struct node *search_node_by_isbn(struct node **ppopened, int cnt, char *pisbn)
{
    int i = 0;
    struct node *p = NULL;
    struct orderinfo *porder = NULL;

    if (ppopened == NULL || cnt <= 0 || pisbn == NULL)
    {
        return NULL;
    }
    for (i = 0; i < cnt; i++)
    {
        p = ppopened[i];
        while (p != NULL)
        {
            if (p->pdata != NULL)
            {
                porder = (struct orderinfo *)p->pdata;
                if (strcmp(porder->isbn, pisbn) == 0)
                {
                    return p;
                }
            }
            p = p->pnext;
        }
    }
    return NULL;
}

/* 统计指定月份销售额 */
float count_month_sales(struct node **pphasharr, int year, int month)
{
    struct node *p = NULL;
    struct orderinfo *porder = NULL;
    float total = 0.0f;

    if (pphasharr == NULL)
    {
        return 0.0f;
    }
    p = pphasharr[OrderHash(CLOSED)];
    while (p != NULL)
    {
        if (p->pdata != NULL)
        {
            porder = (struct orderinfo *)p->pdata;
            if (porder->rdate.y == year && porder->rdate.m == month)
            {
                total += porder->lastprice;
            }
        }
        p = p->pnext;
    }
    return total;
}

/* 统计指定年份销售额 */
float count_year_sales(struct node **pphasharr, int year)
{
    struct node *p = NULL;
    struct orderinfo *porder = NULL;
    float total = 0.0f;

    if (pphasharr == NULL)
    {
        return 0.0f;
    }
    p = pphasharr[OrderHash(CLOSED)];
    while (p != NULL)
    {
        if (p->pdata != NULL)
        {
            porder = (struct orderinfo *)p->pdata;
            if (porder->rdate.y == year)
            {
                total += porder->lastprice;
            }
        }
        p = p->pnext;
    }
    return total;
}

/* 关闭订单：设置归还日期、处理方式、结算金额，状态改为已关闭 */
int close_order(struct node *porder, struct date *rdate, int hway, float price)
{
    struct orderinfo *poinfo = NULL;

    if (porder == NULL || rdate == NULL)
    {
        return -1;
    }
    if (porder->infotype != DT_ORDER || porder->pdata == NULL)
    {
        return -1;
    }
    poinfo = (struct orderinfo *)porder->pdata;
    poinfo->status = CLOSED;
    poinfo->rdate = *rdate;
    poinfo->hway = hway;
    poinfo->lastprice = price;
    return 0;
}
