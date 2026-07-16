#include "bl_public.h"

/* 获取链表长度 */
int get_len_of_linklist(struct node *phead)
{
    int len = 0;
    struct node *p = phead;
    while (p != NULL)
    {
        len++;
        p = p->pnext;
    }
    return len;
}

/* 释放节点及其数据 */
int destroy_node(struct node *pnode)
{
    if (pnode == NULL)
    {
        return -1;
    }
    if (pnode->pdata != NULL)
    {
        free(pnode->pdata);
        pnode->pdata = NULL;
    }
    free(pnode);
    return 0;
}

/* 在链表尾部插入节点 */
int insert_node_at_tail(struct node **pphead, struct node *pnew)
{
    if (pphead == NULL || pnew == NULL)
    {
        return -1;
    }
    pnew->pnext = NULL;
    if (*pphead == NULL)
    {
        *pphead = pnew;
        return 0;
    }
    struct node *p = *pphead;
    while (p->pnext != NULL)
    {
        p = p->pnext;
    }
    p->pnext = pnew;
    return 0;
}

/* 在链表头部插入节点 */
int insert_node_at_head(struct node **pphead, struct node *pnew)
{
    if (pphead == NULL || pnew == NULL)
    {
        return -1;
    }
    pnew->pnext = *pphead;
    *pphead = pnew;
    return 0;
}

/* 从链表中移除指定节点（不释放内存） */
int remove_node(struct node **pphead, struct node *pnode)
{
    if (pphead == NULL || *pphead == NULL || pnode == NULL)
    {
        return -1;
    }
    struct node *p = *pphead;
    if (p == pnode)
    {
        *pphead = p->pnext;
        p->pnext = NULL;
        return 0;
    }
    while (p->pnext != NULL)
    {
        if (p->pnext == pnode)
        {
            p->pnext = pnode->pnext;
            pnode->pnext = NULL;
            return 0;
        }
        p = p->pnext;
    }
    return -1;
}

/* 清空整个链表并释放所有节点 */
int clear_linklist(struct node **pphead)
{
    if (pphead == NULL)
    {
        return -1;
    }
    struct node *p = *pphead;
    struct node *ptmp = NULL;
    while (p != NULL)
    {
        ptmp = p;
        p = p->pnext;
        destroy_node(ptmp);
    }
    *pphead = NULL;
    return 0;
}

/* 检查文件是否存在且非空 */
int is_exist_file(char *filename)
{
    FILE *pf = NULL;
    int len = 0;

    if (filename == NULL)
    {
        return 0;
    }
    pf = fopen(filename, "rb");
    if (pf == NULL)
    {
        return 0;
    }
    fseek(pf, 0, SEEK_END);
    len = ftell(pf);
    fclose(pf);
    pf = NULL;
    return (len > 0) ? 1 : 0;
}
