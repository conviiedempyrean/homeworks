#ifndef BOOK_MODEL_H
#define BOOK_MODEL_H
#include "bl_public.h"

//书名、册数、作者、版号、ISBN(长度为13、纯数字字符的字符串)、出版日期、库存数量、原价、租用价、租用次数
struct bookinfo
{
    char isbn[ISBN_BUF_LEN];
    char name[BOOK_NAME_LEN];
    int volume;
    int version;
    struct date pubdate;
    int stock;
    float sprice;
    float lprice;
    int lcnt;
};

struct bookslinklist
{
    struct node *pfirst;
    int cnt;
};

struct node *create_book_node(struct bookinfo *pdata);
int read_book_from_file(struct bookslinklist *pbsll,char *filename);
int save_book_to_file(struct bookslinklist *pbsll,char *filename);
struct node **sort_books_by_lcnt(struct bookslinklist *pbsll);
struct node *search_book_by_isbn(struct bookslinklist *pbsll,char *pisbn);
int search_book_by_name(struct bookslinklist *pbsll,char *pname,struct node**ppout,int maxcnt);//返回实际找到的项数，maxcnt期望最大的项数
int add_book(struct bookslinklist *pbsll,struct bookinfo *pdata);
int delete_book_by_isbn(struct bookslinklist *pbsll,char *pisbn);
int modify_book_stock(struct bookslinklist *pbsll,char *pisbn,int delta);

#endif