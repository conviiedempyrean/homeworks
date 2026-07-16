#include "usr_model.h"

/* 创建用户节点 */
struct node *create_user_node(struct userinfo *pdata)
{
    struct node *pnode = NULL;
    struct userinfo *puser = NULL;

    if (pdata == NULL)
    {
        return NULL;
    }
    pnode = (struct node *)malloc(sizeof(struct node));
    if (pnode == NULL)
    {
        return NULL;
    }
    puser = (struct userinfo *)malloc(sizeof(struct userinfo));
    if (puser == NULL)
    {
        free(pnode);
        return NULL;
    }
    memcpy(puser, pdata, sizeof(struct userinfo));
    pnode->pdata = (void *)puser;
    pnode->infotype = DT_USER;
    pnode->pnext = NULL;
    return pnode;
}

/* 从二进制文件读取用户链表 */
int read_user_from_file(struct node **pphead, char *filename)
{
    FILE *pf = NULL;
    struct userinfo info;
    struct node *pnode = NULL;
    int cnt = 0;

    if (pphead == NULL || filename == NULL)
    {
        return -1;
    }
    /* 清空已有数据 */
    clear_linklist(pphead);

    pf = fopen(filename, "rb");
    if (pf == NULL)
    {
        return 0;
    }
    while (fread(&info, sizeof(struct userinfo), 1, pf) == 1)
    {
        pnode = create_user_node(&info);
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

/* 保存用户链表到二进制文件 */
int save_user_to_file(struct node *phead, char *filename)
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
            fwrite((struct userinfo *)p->pdata, sizeof(struct userinfo), 1, pf);
            cnt++;
        }
        p = p->pnext;
    }
    fclose(pf);
    return cnt;
}

/* 读取全部三类用户数据到哈希数组 */
int read_all_user(struct node **pphasharr, int cnt)
{
    int ret = 0;
    struct userinfo admin;
    struct node *pnode = NULL;

    if (pphasharr == NULL || cnt < 3)
    {
        return -1;
    }

    ret = read_user_from_file(&(pphasharr[UserHash(UT_STU)]), USER_STU_FILE);
    if (ret < 0)
    {
        return -1;
    }
    ret = read_user_from_file(&(pphasharr[UserHash(UT_TCH)]), USER_TCH_FILE);
    if (ret < 0)
    {
        return -1;
    }

    /* 管理员文件不存在或为空时，创建默认管理员 */
    if (!is_exist_file(USER_ADMIN_FILE))
    {
        memset(&admin, 0, sizeof(struct userinfo));
        strncpy(admin.username, ADMIN_NAME, USER_NAME_LEN - 1);
        strncpy(admin.password, ADMIN_PASS, PASS_LEN - 1);
        admin.type = UT_ADMIN;
        pnode = create_user_node(&admin);
        if (pnode == NULL)
        {
            return -1;
        }
        insert_node_at_tail(&(pphasharr[UserHash(UT_ADMIN)]), pnode);
        save_user_to_file(pphasharr[UserHash(UT_ADMIN)], USER_ADMIN_FILE);
    }
    else
    {
        ret = read_user_from_file(&(pphasharr[UserHash(UT_ADMIN)]), USER_ADMIN_FILE);
        if (ret < 0)
        {
            return -1;
        }
    }
    return 0;
}

/* 保存全部三类用户数据 */
int save_all_user(struct node **pphasharr, int cnt)
{
    if (pphasharr == NULL || cnt < 3)
    {
        return -1;
    }
    if (save_user_to_file(pphasharr[UserHash(UT_STU)], USER_STU_FILE) < 0)
    {
        return -1;
    }
    if (save_user_to_file(pphasharr[UserHash(UT_TCH)], USER_TCH_FILE) < 0)
    {
        return -1;
    }
    if (save_user_to_file(pphasharr[UserHash(UT_ADMIN)], USER_ADMIN_FILE) < 0)
    {
        return -1;
    }
    return 0;
}

/* 按用户名在指定类型链表中查找 */
struct node *search_user_by_name(struct node **pphasharr, int ut, char *name)
{
    struct node *p = NULL;
    struct userinfo *puser = NULL;

    if (pphasharr == NULL || name == NULL)
    {
        return NULL;
    }
    if (ut < UT_STU || ut > UT_ADMIN)
    {
        return NULL;
    }
    p = pphasharr[UserHash(ut)];
    while (p != NULL)
    {
        if (p->pdata != NULL)
        {
            puser = (struct userinfo *)p->pdata;
            if (strcmp(puser->username, name) == 0)
            {
                return p;
            }
        }
        p = p->pnext;
    }
    return NULL;
}

/* 登录验证：遍历所有类型用户，匹配用户名和密码 */
struct node *verify_user_login(struct node **pphasharr, char *name, char *pass, int *type)
{
    int ut = 0;
    struct node *p = NULL;
    struct userinfo *puser = NULL;

    if (pphasharr == NULL || name == NULL || pass == NULL || type == NULL)
    {
        return NULL;
    }
    for (ut = UT_STU; ut <= UT_ADMIN; ut++)
    {
        p = pphasharr[UserHash(ut)];
        while (p != NULL)
        {
            if (p->pdata != NULL)
            {
                puser = (struct userinfo *)p->pdata;
                if (strcmp(puser->username, name) == 0 && strcmp(puser->password, pass) == 0)
                {
                    *type = ut;
                    return p;
                }
            }
            p = p->pnext;
        }
    }
    return NULL;
}
