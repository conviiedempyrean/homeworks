#ifndef BOOK_LESSOR_PUBLIC_H
#define BOOK_LESSOR_PUBLIC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ADMIN_NAME "admin"
#define ADMIN_PASS "123456"
#define ISBN_LEN 13
#define BOOK_NAME_LEN 40
#define USER_NAME_LEN 20
#define PASS_LEN 20
#define PHONE_LEN 14
#define ID_CARD_LEN 18
#define STU_CARD_LEN 16
// +3 for Byte Alignment
#define ISBN_BUF_LEN (ISBN_LEN + 3)
// +2 for Byte Alignment
#define PHONE_BUF_LEN (PHONE_LEN + 2)
#define CARD_BUF_LEN 20
#define BOOK_DAT_FILE "../data/book.dat"
#define USER_STU_FILE "../data/student.usr"
#define USER_TCH_FILE "../data/teacher.usr"
#define USER_ADMIN_FILE "../data/admin.usr"
#define CLOSED_ORDER_FILE "../data/closed.dat"
#define OPENED_ORDER_FILE "../data/opened.dat"
struct date
{
    int y;
    int m;
    int d;
};
enum infotype
{
    DT_USER = 1,
    DT_BOOK,
    DT_ORDER
};
struct node
{
    void *pdata;
    int infotype;//enum infotype
    struct node *pnext;
};
//无头节点单向链表操作函数
int get_len_of_linklist(struct node *phead);
int destroy_node(struct node *pnode);
int insert_node_at_tail(struct node **phead,struct node *pnew);
int insert_node_at_head(struct node **phead,struct node *pnew);
int remove_node(struct node **phead,struct node *pnode);
int clear_linklist(struct node **phead);
int is_exist_file(char *filename);

#endif
