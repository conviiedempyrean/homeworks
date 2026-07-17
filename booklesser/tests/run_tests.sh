#!/usr/bin/env bash
#
# BookLessor 端到端集成测试脚本（Bash / Git Bash 环境）
#
# 用法：
#     cd tests
#     bash run_tests.sh
#
# 脚本会尝试编译项目，失败则使用已有的 bin/booklesser.exe；
# 测试前备份 data/，测试后自动恢复。

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BIN_DIR="$PROJECT_ROOT/bin"
DATA_DIR="$PROJECT_ROOT/data"
TEST_EXE="$BIN_DIR/booklesser_test.exe"
ORIGIN_EXE="$BIN_DIR/booklesser.exe"

SRCS=(
    "$PROJECT_ROOT/src/ctrler/main.c"
    "$PROJECT_ROOT/src/ctrler/login_ctl.c"
    "$PROJECT_ROOT/src/ctrler/admin_ctl.c"
    "$PROJECT_ROOT/src/ctrler/stu_ctl.c"
    "$PROJECT_ROOT/src/ctrler/tch_ctl.c"
    "$PROJECT_ROOT/src/model/book_model.c"
    "$PROJECT_ROOT/src/model/order_model.c"
    "$PROJECT_ROOT/src/model/public_model.c"
    "$PROJECT_ROOT/src/model/usr_model.c"
    "$PROJECT_ROOT/src/viewer/admin_ui.c"
    "$PROJECT_ROOT/src/viewer/main_ui.c"
    "$PROJECT_ROOT/src/viewer/public_ui.c"
    "$PROJECT_ROOT/src/viewer/stu_ui.c"
    "$PROJECT_ROOT/src/viewer/tch_ui.c"
)

# ----------------------------------------------------------------------------
# 工具函数
# ----------------------------------------------------------------------------

backup_data() {
    BACKUP_DIR="$(mktemp -d)"
    cp "$DATA_DIR"/* "$BACKUP_DIR" 2>/dev/null || true
}

restore_data() {
    if [[ -d "${BACKUP_DIR:-}" ]]; then
        rm -f "$DATA_DIR"/*.dat "$DATA_DIR"/*.usr 2>/dev/null || true
        cp "$BACKUP_DIR"/* "$DATA_DIR" 2>/dev/null || true
        rm -rf "$BACKUP_DIR"
    fi
}

clean_data() {
    rm -f "$DATA_DIR"/*.dat "$DATA_DIR"/*.usr 2>/dev/null || true
}

compile_project() {
    if command -v gcc &>/dev/null; then
        gcc -I "$PROJECT_ROOT/inc" "${SRCS[@]}" -o "$TEST_EXE" 2>/dev/null && {
            echo "[INFO] 编译成功：$TEST_EXE"
            return 0
        }
        echo "[WARN] 编译失败，尝试使用已有二进制文件。"
    else
        echo "[WARN] 未找到 gcc，尝试使用已有二进制文件。"
    fi
    return 1
}

locate_executable() {
    if compile_project; then
        echo "$TEST_EXE"
    elif [[ -f "$ORIGIN_EXE" ]]; then
        echo "$ORIGIN_EXE"
    else
        echo "[ERROR] 找不到可用的可执行文件。" >&2
        exit 1
    fi
}

run_session() {
    local input_file="$1"
    local output_file="$2"
    "$EXE" < "$input_file" > "$output_file" 2>&1 || true
}

assert_contains() {
    local file="$1"
    local pattern="$2"
    local msg="${3:-$pattern}"
    if ! grep -q "$pattern" "$file"; then
        echo "[FAIL] 未找到期望输出：$msg" >&2
        return 1
    fi
}

assert_not_contains() {
    local file="$1"
    local pattern="$2"
    local msg="${3:-$pattern}"
    if grep -q "$pattern" "$file"; then
        echo "[FAIL] 不应出现该输出：$msg" >&2
        return 1
    fi
}

# ----------------------------------------------------------------------------
# 主测试场景：覆盖管理员、学生、老师、图书、订单、折扣、销售额等需求
# ----------------------------------------------------------------------------

build_full_scenario() {
    cat <<'EOF'
3
admin
123456
1
stu1
spass
0
13800000001
2021001001
张三
2000 1 1

1
tch1
tpass
1
13900000002
T1988001
李四
1980 5 10

2
9780000000001
C程序设计
1
3
2020 6 1
5
50.00
10.00

2
9780000000002
数据结构
1
2
2019 3 15
2
80.00
15.00

2
9780000000003
操作系统
1
1
2021 8 20
3
60.00
12.00

0
1
stu1
spass
1
C

2

4
9780000000001
2024 3 1

4
9780000000002
2024 3 2

4
9780000000003
2024 3 3

4
9780000000001
2024 3 4

3

6
newspass

0
1
stu1
newspass

0
2
tch1
tpass
4
9780000000001
2024 3 5

3

0
3
admin
123456
4
9780000000001
stu1
2024 3 10
1

4
9780000000002
stu1
2024 3 12
2
25.00

4
9780000000001
tch1
2024 3 15
1

5
2024 3 1

6
2024

7

0
0
EOF
}

verify_full_scenario() {
    local log="$1"
    assert_contains "$log" "登录成功"
    assert_contains "$log" "用户新增成功"
    assert_contains "$log" "图书入库成功"
    assert_contains "$log" "租书成功，结算价：8.00" "学生租书 80% 折扣应为 8.00"
    assert_contains "$log" "租书成功，结算价：12.00" "学生租书 80% 折扣应为 12.00"
    assert_contains "$log" "租书成功，结算价：9.60" "学生租书 80% 折扣应为 9.60"
    assert_contains "$log" "达到上限" "学生未关闭订单应受上限 3 限制"
    assert_contains "$log" "密码修改成功"
    assert_contains "$log" "租书成功，结算价：9.00" "老师租书 90% 折扣应为 9.00"
    assert_contains "$log" "订单关闭成功，结算金额：8.00" "正常归还结算价"
    assert_contains "$log" "订单关闭成功，结算金额：25.00" "损毁赔偿金额"
    assert_contains "$log" "订单关闭成功，结算金额：9.00" "老师订单结算价"
    assert_contains "$log" "月销售额：42.00" "2024-03 月销售额 = 8 + 25 + 9"
    assert_contains "$log" "年销售额：42.00" "2024 年销售额 = 8 + 25 + 9"
    assert_contains "$log" "C程序设计"
    assert_contains "$log" "数据结构"
    assert_contains "$log" "操作系统"
    assert_contains "$log" "感谢使用，再见！"
}

# ----------------------------------------------------------------------------
# 边界场景：库存不足
# ----------------------------------------------------------------------------

build_edge_scenario() {
    cat <<'EOF'
3
admin
123456
1
s1
p1
0
123
2021001
学生甲
2000 1 1

2
1110000000001
唯一书
1
1
2020 1 1
1
10.00
5.00

0
1
s1
p1
4
1110000000001
2024 1 1

4
1110000000001
2024 1 2

0
0
EOF
}

verify_edge_scenario() {
    local log="$1"
    assert_contains "$log" "租书成功" "第一次租借应成功"
    assert_contains "$log" "图书库存不足" "库存为 1 时第二次租借应失败"
}

# ----------------------------------------------------------------------------
# 主入口
# ----------------------------------------------------------------------------

main() {
    cd "$PROJECT_ROOT"
    backup_data
    trap restore_data EXIT

    clean_data
    EXE="$(locate_executable)"

    echo "======================================"
    echo "运行主场景测试"
    echo "======================================"
    local main_input="$(mktemp)"
    local main_log="$(mktemp)"
    build_full_scenario > "$main_input"
    run_session "$main_input" "$main_log"
    verify_full_scenario "$main_log"
    echo "[PASS] 主场景测试通过"

    echo ""
    echo "======================================"
    echo "运行边界场景测试"
    echo "======================================"
    clean_data
    local edge_input="$(mktemp)"
    local edge_log="$(mktemp)"
    build_edge_scenario > "$edge_input"
    run_session "$edge_input" "$edge_log"
    verify_edge_scenario "$edge_log"
    echo "[PASS] 边界场景测试通过"

    echo ""
    echo "======================================"
    echo "所有测试通过"
    echo "======================================"

    rm -f "$main_input" "$main_log" "$edge_input" "$edge_log"
}

main "$@"
