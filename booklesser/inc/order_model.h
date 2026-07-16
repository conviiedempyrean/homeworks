#ifndef ORDER_MODEL_H

#define ORDER_MODEL_H

#include "bl_public.h"

enum ORDER_STATUS
{
    CLOSED = 0,
    OPENED,
};
enum HANDLER_WAY
{
    UNHANDLED,
    NORMAL,
    REDRESS,
};

//租用日期、ISBN、用户名、订单状态（关闭和未关闭）、归还日期、处理方式（正常归还、损毁赔偿）
struct orderinfo
{
    struct date ldate;
    char isbn[ISBN_BUF_LEN];
    char username[USER_NAME_LEN];
    int status;
    struct date rdate;
    int hway;
    float lastprice;
};

#define OrderHash(s) ((s)%2)

struct node *create_order_node(struct orderinfo *pdata);
int read_order_from_file(struct node **pphead,char *filename);
int save_order_to_file(struct node *phead,char *filename);
int read_all_order(struct node **pphasharr,int cnt);
int save_all_order(struct node **pphasharr,int cnt);
struct node **search_opened_order_by_name(struct node **pphasharr,char
*pusername,int *pcnt);
struct node **search_closed_order_by_name(struct node **pphasharr,char
*pusername,int *pcnt);
struct node *search_node_by_isbn(struct node **ppopened,int cnt,char *pisbn);
float count_month_sales(struct node **pphasharr,int year,int month);
float count_year_sales(struct node **pphasharr,int year);
int close_order(struct node *porder,struct date *rdate,int hway,float price);

#endif