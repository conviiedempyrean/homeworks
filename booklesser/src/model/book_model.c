#include "book_model.h"
#include <ctype.h>

/* 创建图书节点 */
struct node *create_book_node(struct bookinfo *pdata)
{
    struct node *pnode = NULL;
    struct bookinfo *pbook = NULL;

    if (pdata == NULL)
    {
        return NULL;
    }
    pnode = (struct node *)malloc(sizeof(struct node));
    if (pnode == NULL)
    {
        return NULL;
    }
    pbook = (struct bookinfo *)malloc(sizeof(struct bookinfo));
    if (pbook == NULL)
    {
        free(pnode);
        return NULL;
    }
    memcpy(pbook, pdata, sizeof(struct bookinfo));
    pnode->pdata = (void *)pbook;
    pnode->infotype = DT_BOOK;
    pnode->pnext = NULL;
    return pnode;
}

/* 从二进制文件读取图书链表 */
int read_book_from_file(struct bookslinklist *pbsll, char *filename)
{
    FILE *pf = NULL;
    struct bookinfo info;
    struct node *pnode = NULL;
    int cnt = 0;

    if (pbsll == NULL || filename == NULL)
    {
        return -1;
    }
    /* 清空已有数据 */
    clear_linklist(&(pbsll->pfirst));
    pbsll->cnt = 0;

    pf = fopen(filename, "rb");
    if (pf == NULL)
    {
        return 0;
    }
    while (fread(&info, sizeof(struct bookinfo), 1, pf) == 1)
    {
        pnode = create_book_node(&info);
        if (pnode == NULL)
        {
            fclose(pf);
            return -1;
        }
        insert_node_at_tail(&(pbsll->pfirst), pnode);
        cnt++;
    }
    fclose(pf);
    pbsll->cnt = cnt;
    return cnt;
}

/* 保存图书链表到二进制文件 */
int save_book_to_file(struct bookslinklist *pbsll, char *filename)
{
    FILE *pf = NULL;
    struct node *p = NULL;
    int cnt = 0;

    if (pbsll == NULL || filename == NULL)
    {
        return -1;
    }
    pf = fopen(filename, "wb");
    if (pf == NULL)
    {
        return -1;
    }
    p = pbsll->pfirst;
    while (p != NULL)
    {
        if (p->pdata != NULL)
        {
            fwrite((struct bookinfo *)p->pdata, sizeof(struct bookinfo), 1, pf);
            cnt++;
        }
        p = p->pnext;
    }
    fclose(pf);
    return cnt;
}

/* qsort比较函数：按租用次数降序排列 */
static int cmp_book_by_lcnt(const void *a, const void *b)
{
    struct node *pa = *(struct node **)a;
    struct node *pb = *(struct node **)b;
    struct bookinfo *pba = NULL;
    struct bookinfo *pbb = NULL;

    if (pa == NULL || pb == NULL || pa->pdata == NULL || pb->pdata == NULL)
    {
        return 0;
    }
    pba = (struct bookinfo *)pa->pdata;
    pbb = (struct bookinfo *)pb->pdata;
    return (pbb->lcnt - pba->lcnt);
}

/* 按热度排序，返回排序后的节点指针数组（需由调用者释放） */
struct node **sort_books_by_lcnt(struct bookslinklist *pbsll)
{
    struct node **pparr = NULL;
    struct node *p = NULL;
    int i = 0;

    if (pbsll == NULL || pbsll->cnt <= 0)
    {
        return NULL;
    }
    pparr = (struct node **)malloc(sizeof(struct node *) * pbsll->cnt);
    if (pparr == NULL)
    {
        return NULL;
    }
    p = pbsll->pfirst;
    for (i = 0; i < pbsll->cnt && p != NULL; i++)
    {
        pparr[i] = p;
        p = p->pnext;
    }
    qsort(pparr, pbsll->cnt, sizeof(struct node *), cmp_book_by_lcnt);
    return pparr;
}

/* 按ISBN精确查找图书 */
struct node *search_book_by_isbn(struct bookslinklist *pbsll, char *pisbn)
{
    struct node *p = NULL;
    struct bookinfo *pbook = NULL;

    if (pbsll == NULL || pisbn == NULL)
    {
        return NULL;
    }
    p = pbsll->pfirst;
    while (p != NULL)
    {
        if (p->pdata != NULL)
        {
            pbook = (struct bookinfo *)p->pdata;
            if (strcmp(pbook->isbn, pisbn) == 0)
            {
                return p;
            }
        }
        p = p->pnext;
    }
    return NULL;
}

/* 辅助：将字符串转换为小写 */
static void str_tolower(char *dst, const char *src, int len)
{
    int i = 0;
    for (i = 0; i < len - 1 && src[i] != '\0'; i++)
    {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

/* 按书名模糊查找图书，结果写入ppout数组，返回实际找到数量 */
int search_book_by_name(struct bookslinklist *pbsll, char *pname, struct node **ppout, int maxcnt)
{
    struct node *p = NULL;
    struct bookinfo *pbook = NULL;
    char name_lower[BOOK_NAME_LEN] = {0};
    char book_name_lower[BOOK_NAME_LEN] = {0};
    int cnt = 0;

    if (pbsll == NULL || pname == NULL || ppout == NULL || maxcnt <= 0)
    {
        return 0;
    }
    str_tolower(name_lower, pname, BOOK_NAME_LEN);
    p = pbsll->pfirst;
    while (p != NULL && cnt < maxcnt)
    {
        if (p->pdata != NULL)
        {
            pbook = (struct bookinfo *)p->pdata;
            str_tolower(book_name_lower, pbook->name, BOOK_NAME_LEN);
            if (strstr(book_name_lower, name_lower) != NULL)
            {
                ppout[cnt++] = p;
            }
        }
        p = p->pnext;
    }
    return cnt;
}

/* 图书入库：ISBN查重，尾部插入 */
int add_book(struct bookslinklist *pbsll, struct bookinfo *pdata)
{
    struct node *pnode = NULL;

    if (pbsll == NULL || pdata == NULL)
    {
        return -1;
    }
    if (search_book_by_isbn(pbsll, pdata->isbn) != NULL)
    {
        return -1;
    }
    pnode = create_book_node(pdata);
    if (pnode == NULL)
    {
        return -1;
    }
    insert_node_at_tail(&(pbsll->pfirst), pnode);
    pbsll->cnt++;
    return 0;
}

/* 按ISBN删除图书 */
int delete_book_by_isbn(struct bookslinklist *pbsll, char *pisbn)
{
    struct node *pnode = NULL;

    if (pbsll == NULL || pisbn == NULL)
    {
        return -1;
    }
    pnode = search_book_by_isbn(pbsll, pisbn);
    if (pnode == NULL)
    {
        return -1;
    }
    if (remove_node(&(pbsll->pfirst), pnode) != 0)
    {
        return -1;
    }
    destroy_node(pnode);
    pbsll->cnt--;
    return 0;
}

/* 修改图书库存：delta为正表示入库补充，为负表示租借出库 */
int modify_book_stock(struct bookslinklist *pbsll, char *pisbn, int delta)
{
    struct node *pnode = NULL;
    struct bookinfo *pbook = NULL;

    if (pbsll == NULL || pisbn == NULL)
    {
        return -1;
    }
    pnode = search_book_by_isbn(pbsll, pisbn);
    if (pnode == NULL || pnode->pdata == NULL)
    {
        return -1;
    }
    pbook = (struct bookinfo *)pnode->pdata;
    if (delta < 0 && pbook->stock + delta < 0)
    {
        return -1;
    }
    pbook->stock += delta;
    return 0;
}
