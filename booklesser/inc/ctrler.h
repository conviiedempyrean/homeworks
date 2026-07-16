#ifndef CTRLER_H
#define CTRLER_H

#include "book_model.h"
#include "order_model.h"
#include "usr_model.h"
#include "viewer.h"

/* 控制器入口函数 */
int admin_ctl(struct node **users, struct bookslinklist *books, struct node **orders);
int stu_ctl(struct node **users, struct bookslinklist *books, struct node **orders);
int tea_ctl(struct node **users, struct bookslinklist *books, struct node **orders);

/* 共享登录辅助函数：循环提示输入用户名/密码，直到登录成功或用户放弃 */
int prompt_login(struct node **users, int expected_type, struct node **out_user);

/* 共享订单创建流程：租书 */
int create_order_flow(struct node **users,
                      struct bookslinklist *books,
                      struct node **orders,
                      struct userinfo *current_user,
                      float discount,
                      int max_open);

/* 共享订单关闭流程：还书/赔偿 */
int close_order_flow(struct node **users,
                     struct bookslinklist *books,
                     struct node **orders,
                     const char *restrict_username);

#endif
