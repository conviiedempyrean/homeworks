# `book_model.c` 图书模型层说明文档

`booklesser/src/model/book_model.c` 是 BookLessor 系统的**图书模型层**，负责维护图书在内存中的链表、与二进制文件 `../data/book.dat` 的读写，以及图书查询、排序、入库、删除、库存修改等核心业务操作。下面按数据结构和函数逐一说明。

---

## 1. 相关数据结构

这些数据结构定义在 `inc/book_model.h` 和 `inc/bl_public.h` 中。

### 1.1 `struct bookinfo`（图书数据本体）

```c
struct bookinfo
{
    char isbn[ISBN_BUF_LEN];     // ISBN，长度为 13 的纯数字字符串
    char name[BOOK_NAME_LEN];    // 书名
    int volume;                  // 册数
    int version;                 // 版号
    struct date pubdate;         // 出版日期
    int stock;                   // 库存数量
    float sprice;                // 原价
    float lprice;                // 租用价
    int lcnt;                    // 租用次数，即热度
};
```

### 1.2 `struct bookslinklist`（图书链表管理器）

```c
struct bookslinklist
{
    struct node *pfirst;   // 链表头指针
    int cnt;               // 当前图书节点数量
};
```

### 1.3 `struct node`（通用链表节点）

```c
struct node
{
    void *pdata;           // 指向实际数据（此处指向 struct bookinfo）
    int infotype;          // 数据类型标记，图书为 DT_BOOK
    struct node *pnext;    // 下一个节点
};
```

**整体结构**：图书数据在内存中表现为一条**无头节点的单向链表**，由 `bookslinklist.pfirst` 指向表头，`bookslinklist.cnt` 记录节点数量。每个节点的 `pdata` 指向一个 `bookinfo` 结构体。

---

## 2. 函数详解

### 2.1 `create_book_node`

```c
struct node *create_book_node(struct bookinfo *pdata)
```

- **作用**：根据传入的 `bookinfo` 创建一个新的图书链表节点。
- **数据结构变化**：
  - 分配 `struct node` 和 `struct bookinfo` 各一份内存。
  - 使用 `memcpy` 把 `pdata` 复制到新的 `bookinfo` 中。
  - 设置 `node->pdata = pbook`、`node->infotype = DT_BOOK`、`node->pnext = NULL`。
  - **不修改原链表**，只返回新节点指针。

---

### 2.2 `read_book_from_file`

```c
int read_book_from_file(struct bookslinklist *pbsll, char *filename)
```

- **作用**：从二进制文件 `book.dat` 读取所有图书，重建内存链表。
- **数据结构变化**：
  - 调用 `clear_linklist` 清空旧链表，并将 `pbsll->cnt` 置为 0。
  - 以 `rb` 模式打开文件，逐个 `fread` 读取 `bookinfo` 结构体。
  - 每读取一条记录，调用 `create_book_node` 创建节点，并 `insert_node_at_tail` 插入链表尾部。
  - 最后设置 `pbsll->cnt = cnt`。
- **返回值**：成功读取的记录数；文件不存在则返回 0。

---

### 2.3 `save_book_to_file`

```c
int save_book_to_file(struct bookslinklist *pbsll, char *filename)
```

- **作用**：将内存中的图书链表持久化到二进制文件。
- **数据结构变化**：
  - 以 `wb` 模式打开文件，覆盖原文件内容。
  - 遍历链表，将每个节点中的 `bookinfo` 原样写入文件。
  - **不修改内存链表**。

---

### 2.4 `sort_books_by_lcnt`

```c
struct node **sort_books_by_lcnt(struct bookslinklist *pbsll)
```

- **作用**：按租用次数 `lcnt` 从高到低排序，用于热度查询。
- **数据结构变化**：
  - 根据 `pbsll->cnt` 分配一个 `struct node*` 指针数组。
  - 遍历链表，把每个节点指针存入数组。
  - 使用 `qsort` 和比较函数 `cmp_book_by_lcnt` 对数组降序排序。
  - **排序只发生在返回的指针数组中，原链表顺序保持不变**。
  - 调用者需要自行 `free` 返回的数组。

---

### 2.5 `search_book_by_isbn`

```c
struct node *search_book_by_isbn(struct bookslinklist *pbsll, char *pisbn)
```

- **作用**：按 ISBN 精确查找图书。
- **数据结构变化**：**只读**，不修改链表。
- 遍历链表，比较 `bookinfo.isbn` 与 `pisbn`，找到第一个匹配节点即返回；找不到返回 `NULL`。

---

### 2.6 `search_book_by_name`

```c
int search_book_by_name(struct bookslinklist *pbsll, char *pname,
                        struct node **ppout, int maxcnt)
```

- **作用**：按书名**模糊**查询，不区分大小写。
- 内部调用 `str_tolower` 将关键字和书名均转为小写，再用 `strstr` 做子串匹配。
- **数据结构变化**：
  - 不修改原链表。
  - 将匹配节点的指针依次写入调用者提供的 `ppout` 数组。
  - 返回实际匹配数量，最多不超过 `maxcnt`。

---

### 2.7 `add_book`

```c
int add_book(struct bookslinklist *pbsll, struct bookinfo *pdata)
```

- **作用**：图书入库。
- **数据结构变化**：
  - 调用 `search_book_by_isbn` 检查 ISBN 是否已存在。若存在则返回 -1，拒绝重复入库。
  - 创建新节点并插入链表尾部。
  - `pbsll->cnt++`。
  - 新节点的 `lcnt` 保持传入值（通常为 0）。

---

### 2.8 `delete_book_by_isbn`

```c
int delete_book_by_isbn(struct bookslinklist *pbsll, char *pisbn)
```

- **作用**：按 ISBN 删除图书。
- **数据结构变化**：
  - 找到对应节点后，用 `remove_node` 将其从链表中摘下。
  - 调用 `destroy_node` 释放节点及 `bookinfo` 内存。
  - `pbsll->cnt--`。

---

### 2.9 `modify_book_stock`

```c
int modify_book_stock(struct bookslinklist *pbsll, char *pisbn, int delta)
```

- **作用**：修改图书库存。`delta` 为正表示入库或归还，为负表示租借出库。
- **数据结构变化**：
  - 按 ISBN 找到对应节点。
  - 若 `delta < 0` 且 `stock + delta < 0`，则返回 -1，防止库存为负。
  - 否则执行 `pbook->stock += delta`。
- 租借成功时，上层 `create_order_flow` 还会将该图书的 `lcnt` 加 1，用于热度统计。

---

## 3. 函数对数据结构的改变总览

| 函数 | 是否修改 `bookslinklist` | 是否修改链表节点 | 是否涉及文件 |
| --- | --- | --- | --- |
| `create_book_node` | 否 | 否（仅创建新节点） | 否 |
| `read_book_from_file` | 是（重建链表、更新 `cnt`） | 是（新增节点） | 读 |
| `save_book_to_file` | 否 | 否 | 写 |
| `sort_books_by_lcnt` | 否 | 否 | 否 |
| `search_book_by_isbn` | 否 | 否 | 否 |
| `search_book_by_name` | 否 | 否 | 否 |
| `add_book` | 是（`cnt++`） | 是（尾部插入） | 否 |
| `delete_book_by_isbn` | 是（`cnt--`） | 是（删除并释放） | 否 |
| `modify_book_stock` | 否 | 是（修改 `stock`） | 否 |

---

## 4. 总结

`book_model.c` 把 `struct bookinfo` 组织成一条单向链表，并提供了一套完整的 **CRUD、查询、排序、持久化** 机制。它向上支撑着控制层与视图层完成图书入库、按书名/ISBN 查询、按热度排序、库存扣减与恢复、删除等全部图书相关业务。上层在租借/归还时通过对 `stock` 和 `lcnt` 的协同修改，进一步实现了库存管理与热度统计。
