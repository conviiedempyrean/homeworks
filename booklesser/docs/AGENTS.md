# AGENTS.md

> 本文档面向 AI 编程助手。项目根目录此前没有 `AGENTS.md`（也没有任何 `README` 或文档文件），因此以下内容完全基于当前项目目录中的实际文件内容整理而成。

## 项目概述

- **项目名称**：`booklesser`（Book Lessor，图书租赁管理系统）
- **项目类型**：C 语言控制台应用程序
- **业务目标**：实现一个学生/老师可以租书、管理员可以管理图书/用户/订单的图书租赁系统
- **当前状态**：`src/model/` 模型层、`src/ctrler/` 控制器层及 `src/viewer/` 最小视图层均已实现，项目可完整编译并运行。缺少现成构建脚本、测试框架或 CI/CD 配置。

## 技术栈与运行时架构

- **编程语言**：C（ANSI C / C99 风格）
- **标准库**：`stdio.h`、`stdlib.h`、`string.h`
- **构建系统**：无（没有 `Makefile`、`CMakeLists.txt`、`.vscode`、批处理或 Shell 脚本）
- **数据持久化**：二进制文件，通过 `fread` / `fwrite` 直接读写结构体节点
- **数据结构**：
  - 通用单链表 `struct node`，节点内保存 `void *pdata` 和类型标记 `infotype`
  - 小型“哈希表”：`users[3]` 按用户类型分桶（0-学生、1-老师、2-管理员），`orders[2]` 按订单状态分桶（0-已关闭、1-未关闭）
- **架构风格**：尝试使用 MVC 分层
  - `viewer/`：视图/界面层（目前为空）
  - `ctrler/`：控制器层（入口与角色菜单调度）
  - `model/`：模型层（数据结构与业务操作）
- **运行方式**：终端交互式运行

## 目录结构与文件职责

```
booklesser/
├── bin/                  # 空的输出/可执行文件目录
├── data/                 # 数据文件目录，当前全部为空（0 字节）
│   ├── admin.usr         # 管理员数据
│   ├── book.dat          # 图书数据
│   ├── closed.dat        # 已关闭订单数据
│   ├── opened.dat        # 未关闭订单数据
│   ├── student.usr       # 学生用户数据
│   └── teacher.usr       # 老师用户数据
├── inc/                  # 头文件
│   ├── bl_public.h       # 公共宏、结构体（date、node）、链表操作、文件路径、硬编码 admin 账号
│   ├── book_leesor.h     # 总包含头文件，聚合其余所有头文件
│   ├── book_model.h      # 图书结构体 bookinfo 与图书相关 API
│   ├── ctrler.h          # 控制器函数声明
│   ├── order_model.h     # 订单结构体 orderinfo 与订单相关 API
│   ├── usr_model.h       # 用户结构体 userinfo 与用户相关 API
│   └── viewer.h          # 界面函数声明
└── src/
    ├── ctrler/             # 控制器层已实现
    │   ├── main.c          # 主入口：初始化、加载数据、主循环、保存数据、释放内存
    │   ├── login_ctl.c     # 共享登录与订单创建/关闭流程
    │   ├── admin_ctl.c     # 管理员登录与功能调度
    │   ├── stu_ctl.c       # 学生登录与功能调度
    │   └── tch_ctl.c       # 老师登录与功能调度
    ├── model/              # 模型层已实现
    │   ├── book_model.c    # 图书 CRUD、ISBN/书名查询、热度排序、库存修改
    │   ├── order_model.c   # 订单 CRUD、按用户查询、销售额统计、关闭订单
    │   ├── public_model.c  # 基础链表操作、文件存在性检查
    │   └── usr_model.c     # 用户 CRUD、登录验证、默认管理员初始化
    └── viewer/             # 视图层已提供最小实现
        ├── admin_ui.c
        ├── main_ui.c
        ├── public_ui.c
        ├── stu_ui.c
        └── tch_ui.c
```

## 构建与运行命令

项目**没有现成构建脚本**。如果希望手动尝试编译，可执行类似命令（以 GCC 为例）：

```bash
cd d:/sw_files/test/homeworks/booklesser
gcc -I inc \
    src/ctrler/main.c src/ctrler/login_ctl.c \
    src/ctrler/admin_ctl.c src/ctrler/stu_ctl.c src/ctrler/tch_ctl.c \
    src/model/book_model.c src/model/order_model.c src/model/public_model.c src/model/usr_model.c \
    src/viewer/admin_ui.c src/viewer/main_ui.c src/viewer/public_ui.c src/viewer/stu_ui.c src/viewer/tch_ui.c \
    -o bin/booklesser.exe
```

然后切换到 `bin/` 目录运行：

```bash
cd bin
./booklesser.exe
```

> ⚠️ 注意：数据文件路径使用相对路径 `../data/...`，因此必须从 `bin/` 目录运行，否则无法找到数据文件。

## 代码组织与主要模块

| 模块 | 目录 | 说明 |
|------|------|------|
| 视图 (Viewer) | `src/viewer/` | 负责菜单显示、输入输出，已提供最小可用实现 |
| 控制器 (Controller) | `src/ctrler/` | 负责登录验证、菜单循环、调用模型/视图，已实现 |
| 模型 (Model) | `src/model/` | 负责数据结构与业务逻辑（链表、文件读写、查询统计），已实现 |
| 公共定义 | `inc/bl_public.h` | 链表节点、日期结构、常量、文件路径、默认管理员账号 |

### 关键数据结构

- `struct bookinfo`：ISBN、书名、册数、版号、出版日期、库存、原价、租用价、租用次数
- `struct userinfo`：用户名、密码、类型、联系方式、证件号、证件姓名、出生日期
- `struct orderinfo`：租用日期、ISBN、用户名、订单状态、归还日期、处理方式、结算金额
- `struct node`：通用链表节点，含 `void *pdata` 和 `infotype` 类型标记

### 角色与业务规则（从注释中整理）

- **管理员**：默认账号 `admin` / `123456`，可新增用户、图书入库、新增订单、关闭订单、销售额/热度查询、修改密码
- **学生**：最多同时存在 3 个未关闭订单，租价为 `book.lprice * 0.8`
- **老师**：最多同时存在 5 个未关闭订单，租价为 `book.lprice * 0.9`
- **损毁赔偿**：关闭订单时可选处理方式，赔偿金额覆盖租价

## 开发约定与代码风格

- **注释与界面文本语言**：简体中文
- **标识符风格**：
  - 函数、变量、结构体名：小写蛇形（`snake_case`），如 `search_book_by_isbn`、`struct bookinfo`
  - 宏：全大写，如 `ISBN_LEN`、`BOOK_DAT_FILE`
- **缩进**：空格/Tab 混用，不统一
- **错误处理/边界检查**：模型层已实现基本的空指针、文件打开等检查，但输入长度与类型校验仍不完整
- **“哈希”实现**：只是固定长度数组加分桶宏
  - `UserHash(t) ((t) % 3)`
  - `OrderHash(s) ((s) % 2)`
- **内存管理**：`public_model.c` 已实现 `destroy_node` 与 `clear_linklist`，各 `read_*_from_file` 在加载前会先清空旧链表

## 测试策略

- 已通过脚本化输入完成端到端集成测试：管理员新增用户/图书/订单、学生查询图书与还书、管理员查询销售额等流程均正常，退出后数据正确持久化。
- 建议的后续测试方式：
  1. 使用单元测试框架（如 Unity）对 `model/` 中链表、文件读写、查询统计函数进行系统测试；
  2. 使用临时 `data/` 目录进行更多边界场景测试（库存不足、订单上限、重复 ISBN、损毁赔偿等）；
  3. 补充自动化构建脚本（Makefile）与回归测试。

## 安全注意事项

- **硬编码管理员凭据**：`inc/bl_public.h` 中定义了 `ADMIN_NAME "admin"` 和 `ADMIN_PASS "123456"`。
- **明文存储密码**：用户密码以明文形式保存在二进制数据文件中。
- **结构体二进制序列化风险**：数据文件直接依赖当前编译器/平台下的结构体内存布局，跨平台或结构体变更后会导致数据损坏或不兼容。
- **硬编码相对路径**：所有数据文件路径都是相对路径（如 `../data/book.dat`），可执行文件必须从正确目录运行，否则找不到数据。
- **缺少输入校验**：当前代码没有体现长度检查、类型校验、缓冲区溢出防护等机制。
- 测试后 `data/` 文件已重置为空，但运行程序后会生成二进制数据文件，其中包含明文密码，仅用于本地测试。

## 当前已知问题清单

1. `src/viewer/*.c` 目前为最小桩实现，菜单与表格格式较为简陋，可后续美化。
2. 缺少输入长度与类型校验，存在缓冲区溢出风险（如 `scanf("%s", ...)` 未限制长度）。
3. 没有构建脚本（Makefile）、测试脚本或 CI/CD 配置。
4. 相对路径依赖导致可执行文件必须从 `bin/` 目录运行。

---

**总结**：BookLessor 项目已完成 MVC 三层核心实现：模型层负责数据结构与持久化，控制器层负责登录验证与业务流程，视图层提供最小可用交互界面。项目可编译运行，支持管理员、学生、老师三类角色的完整租书/还书/查询流程。
