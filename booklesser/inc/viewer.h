#ifndef VIEWER_H
#define VIEWER_H

#include "bl_public.h"
#include "book_model.h"
#include "order_model.h"
#include "usr_model.h"

// 公共界面
void show_welcome();            // 欢迎信息
void show_goodbye();            // 退出信息
void pause_screen();            // 暂停等待按键
void clear_screen();            // 清屏
int get_user_choice();          // 获取用户菜单选择

// 主界面
int main_ui();                  // 返回：1-学生登录 2-老师登录 3-管理员登录 0-退出

// 学生/老师界面（可合并为 unadmin_ui）
int stu_ui();                   // 学生功能菜单，返回操作码
int tea_ui();                   // 老师功能菜单，返回操作码

// 管理员界面
int admin_ui();                 // 管理员功能菜单，返回操作码

// 各功能子界面（输入/输出）
void ui_show_books(struct node **ppbooks, int cnt);     // 显示图书列表
void ui_show_orders(struct node **pporders, int cnt);   // 显示订单列表
void ui_show_users(struct node **ppusers, int cnt);       // 显示用户列表

// 输入界面
int ui_input_bookinfo(struct bookinfo *pdata);            // 图书入库输入
int ui_input_userinfo(struct userinfo *pdata);            // 新增用户输入
int ui_input_orderinfo(struct orderinfo *pdata);          // 新增订单输入
int ui_input_date(struct date *pd);                       // 日期输入
int ui_input_close_info(struct date *rdate, int *hway, float *price); // 关闭订单输入

// 查询界面
void ui_query_book_by_name();     // 按书名查询交互
void ui_query_book_by_heat();     // 按热度查询交互
void ui_query_sales();            // 销售额查询交互
void ui_query_heat();             // 热度查询交互

#endif