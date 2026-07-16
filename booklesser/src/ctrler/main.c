#include "book_leesor.h"

int main(int argc, char *argv[])
{
    struct node *users[3] = {NULL};     /* 0-stu, 1-tch, 2-admin */
    struct bookslinklist books = {NULL, 0};
    struct node *orders[2] = {NULL};    /* 0-closed, 1-opened */
    int cmd = 0;
    int exitflag = 0;

    show_welcome();

    /* 加载数据 */
    read_all_user(users, 3);
    read_book_from_file(&books, BOOK_DAT_FILE);
    read_all_order(orders, 2);

    /* 主循环 */
    while (1)
    {
        cmd = main_ui();
        switch (cmd)
        {
            case 1:
                stu_ctl(users, &books, orders);
                break;
            case 2:
                tea_ctl(users, &books, orders);
                break;
            case 3:
                admin_ctl(users, &books, orders);
                break;
            case 0:
                exitflag = 1;
                break;
            default:
                printf("无效选项，请重试。\n");
                break;
        }
        if (exitflag)
        {
            break;
        }
    }

    /* 退出前保存所有数据 */
    save_all_user(users, 3);
    save_book_to_file(&books, BOOK_DAT_FILE);
    save_all_order(orders, 2);

    /* 释放内存 */
    clear_linklist(&users[0]);
    clear_linklist(&users[1]);
    clear_linklist(&users[2]);
    clear_linklist(&books.pfirst);
    clear_linklist(&orders[0]);
    clear_linklist(&orders[1]);

    show_goodbye();
    return 0;
}
