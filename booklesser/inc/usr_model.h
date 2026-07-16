#ifndef USR_MODEL_H
#define USR_MODEL_H
#include "bl_public.h"
enum usertype
{
    UT_STU,
    UT_TCH,
    UT_ADMIN
};

//用户名、密码、类型、联系方式（字符串）、证件号码(学生证编号或身份证号码字符串)、证件姓名、出生日期
struct userinfo
{
char username[USER_NAME_LEN];
char password[PASS_LEN];
int type;
char phone[PHONE_BUF_LEN];
char paper[CARD_BUF_LEN];
char cardname[USER_NAME_LEN];
struct date birth;
};

#define UserHash(t) ((t)%3)

struct node *create_user_node(struct userinfo *pdata);
int read_user_from_file(struct node **pphead,char *filename);
int save_user_to_file(struct node *phead,char *filename);
int read_all_user(struct node **pphasharr,int cnt);
int save_all_user(struct node **pphasharr,int cnt);
struct node *search_user_by_name(struct node **pphasharr,int ut,char *name);
struct node *verify_user_login(struct node **pphasharr,char *name,char *pass,int *type);

#endif