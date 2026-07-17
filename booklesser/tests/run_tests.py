#!/usr/bin/env python3
"""
BookLessor 端到端自动化测试脚本

用法：
    cd tests
    python run_tests.py

脚本会尝试编译项目（GCC），退而求其次使用已有的 bin/booklesser.exe；
每次运行前都会重置 data/ 目录，运行结束后恢复原始数据。
"""

import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

PROJ_ROOT = Path(__file__).resolve().parent.parent
BIN_DIR = PROJ_ROOT / "bin"
DATA_DIR = PROJ_ROOT / "data"
SRC_FILES = [
    PROJ_ROOT / "src" / "ctrler" / "main.c",
    PROJ_ROOT / "src" / "ctrler" / "login_ctl.c",
    PROJ_ROOT / "src" / "ctrler" / "admin_ctl.c",
    PROJ_ROOT / "src" / "ctrler" / "stu_ctl.c",
    PROJ_ROOT / "src" / "ctrler" / "tch_ctl.c",
    PROJ_ROOT / "src" / "model" / "book_model.c",
    PROJ_ROOT / "src" / "model" / "order_model.c",
    PROJ_ROOT / "src" / "model" / "public_model.c",
    PROJ_ROOT / "src" / "model" / "usr_model.c",
    PROJ_ROOT / "src" / "viewer" / "admin_ui.c",
    PROJ_ROOT / "src" / "viewer" / "main_ui.c",
    PROJ_ROOT / "src" / "viewer" / "public_ui.c",
    PROJ_ROOT / "src" / "viewer" / "stu_ui.c",
    PROJ_ROOT / "src" / "viewer" / "tch_ui.c",
]
TEST_EXE = BIN_DIR / "booklesser_test.exe"
ORIGIN_EXE = BIN_DIR / "booklesser.exe"


# ---------------------------------------------------------------------------
# 工具函数
# ---------------------------------------------------------------------------

def compile_project():
    """尝试编译出一个测试用可执行文件。"""
    if shutil.which("gcc") is None:
        print("[INFO] 未找到 gcc，跳过编译。")
        return None

    cmd = ["gcc", "-I", str(PROJ_ROOT / "inc")]
    cmd += [str(f) for f in SRC_FILES]
    cmd += ["-o", str(TEST_EXE)]
    print("[INFO] 编译命令:", " ".join(cmd))
    try:
        subprocess.run(cmd, cwd=PROJ_ROOT, check=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, encoding="utf-8", errors="replace")
        print("[INFO] 编译成功:", TEST_EXE)
        return TEST_EXE
    except subprocess.CalledProcessError as e:
        print("[WARN] 编译失败:\n", e.stdout)
        return None


def locate_executable():
    """先尝试编译产物，否则用已有的可执行文件。"""
    exe = compile_project() or (ORIGIN_EXE if ORIGIN_EXE.exists() else None)
    if exe is None:
        print("[ERROR] 找不到可用的可执行文件，请检查 bin/ 目录或安装 gcc。")
        sys.exit(1)
    return exe


def backup_data():
    """备份 data/ 目录。"""
    backup = Path(tempfile.mkdtemp(prefix="booklesser_data_backup_"))
    for f in DATA_DIR.iterdir():
        if f.is_file():
            shutil.copy2(f, backup / f.name)
    return backup


def restore_data(backup):
    """恢复 data/ 目录。"""
    for f in DATA_DIR.iterdir():
        if f.is_file():
            f.unlink()
    for f in backup.iterdir():
        shutil.copy2(f, DATA_DIR / f.name)
    shutil.rmtree(backup)


def clean_data():
    """清空 data/ 目录，让程序在首次运行时自动生成默认管理员。"""
    for f in DATA_DIR.iterdir():
        if f.is_file():
            f.unlink()


def run_session(exe, inputs):
    """运行一次程序会话，返回 stdout 文本。"""
    r = subprocess.run(
        [str(exe)],
        cwd=BIN_DIR,
        input=inputs.encode("utf-8"),
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    return r.stdout.decode("utf-8", errors="replace")


# ---------------------------------------------------------------------------
# 输入脚本构建器
# ---------------------------------------------------------------------------

class ScriptBuilder:
    """构造通过 stdin 驱动 BookLessor 的输入脚本。"""

    def __init__(self):
        self._lines = []

    def line(self, value):
        """发送一行输入。"""
        self._lines.append(str(value))
        return self

    def choice(self, n):
        """菜单选择项。"""
        return self.line(n)

    def pause(self):
        """对应 pause_screen() 的“按 Enter 键继续”。"""
        return self.line("")

    def build(self):
        return "\n".join(self._lines) + "\n"


# ---------------------------------------------------------------------------
# 测试断言
# ---------------------------------------------------------------------------

class AssertFail(Exception):
    pass


def assert_contains(output, expected, msg=None):
    if expected not in output:
        raise AssertFail(msg or f"输出中未找到期望文本：{expected!r}")


def assert_not_contains(output, unexpected, msg=None):
    if unexpected in output:
        raise AssertFail(msg or f"输出中不应出现文本：{unexpected!r}")


# ---------------------------------------------------------------------------
# 主测试场景
# ---------------------------------------------------------------------------

def build_full_scenario():
    """构造覆盖大部分需求的完整操作脚本。"""
    s = ScriptBuilder()

    # 1. 管理员登录
    s.choice(3).line("admin").line("123456")

    # 2. 新增学生用户
    s.choice(1)
    s.line("stu1").line("spass").line(0)
    s.line("13800000001").line("2021001001").line("张三")
    s.line("2000 1 1").pause()

    # 3. 新增老师用户
    s.choice(1)
    s.line("tch1").line("tpass").line(1)
    s.line("13900000002").line("T1988001").line("李四")
    s.line("1980 5 10").pause()

    # 4. 入库图书 1：C程序设计，库存 5，租价 10
    s.choice(2)
    s.line("9780000000001").line("C程序设计")
    s.line(1).line(3)
    s.line("2020 6 1")
    s.line(5).line("50.00").line("10.00")
    s.pause()

    # 5. 入库图书 2：数据结构，库存 2，租价 15
    s.choice(2)
    s.line("9780000000002").line("数据结构")
    s.line(1).line(2)
    s.line("2019 3 15")
    s.line(2).line("80.00").line("15.00")
    s.pause()

    # 6. 入库图书 3：操作系统，库存 3，租价 12
    s.choice(2)
    s.line("9780000000003").line("操作系统")
    s.line(1).line(1)
    s.line("2021 8 20")
    s.line(3).line("60.00").line("12.00")
    s.pause()

    # 7. 管理员返回主菜单
    s.choice(0)

    # 8. 学生登录
    s.choice(1).line("stu1").line("spass")

    # 9. 按书名查询（关键字“C”）
    s.choice(1).line("C").pause()

    # 10. 按热度查询
    s.choice(2).pause()

    # 11. 学生租书 1：10 * 0.8 = 8.00
    s.choice(4)
    s.line("9780000000001")
    s.line("2024 3 1")
    s.pause()

    # 12. 学生租书 2：15 * 0.8 = 12.00
    s.choice(4)
    s.line("9780000000002")
    s.line("2024 3 2")
    s.pause()

    # 13. 学生租书 3：12 * 0.8 = 9.60
    s.choice(4)
    s.line("9780000000003")
    s.line("2024 3 3")
    s.pause()

    # 14. 学生第 4 次租书：达到上限 3，应失败
    s.choice(4)
    s.line("9780000000001")
    s.line("2024 3 4")
    s.pause()

    # 15. 查看我的订单（未关闭 + 已关闭）
    s.choice(3).pause()

    # 16. 修改学生密码
    s.choice(6).line("newspass").pause()

    # 17. 学生返回主菜单
    s.choice(0)

    # 18. 用新密码重新登录验证
    s.choice(1).line("stu1").line("newspass")

    # 19. 学生返回主菜单
    s.choice(0)

    # 20. 老师登录
    s.choice(2).line("tch1").line("tpass")

    # 21. 老师租书 1：10 * 0.9 = 9.00
    s.choice(4)
    s.line("9780000000001")
    s.line("2024 3 5")
    s.pause()

    # 22. 老师查看我的订单
    s.choice(3).pause()

    # 23. 老师返回主菜单
    s.choice(0)

    # 24. 管理员再次登录
    s.choice(3).line("admin").line("123456")

    # 25. 关闭学生订单 1（正常归还）：结算价 8.00
    s.choice(4)
    s.line("9780000000001").line("stu1")
    s.line("2024 3 10").line(1)
    s.pause()

    # 26. 关闭学生订单 2（损毁赔偿）：赔偿 25.00
    s.choice(4)
    s.line("9780000000002").line("stu1")
    s.line("2024 3 12").line(2)
    s.line("25.00").pause()

    # 27. 关闭老师订单（正常归还）：结算价 9.00
    s.choice(4)
    s.line("9780000000001").line("tch1")
    s.line("2024 3 15").line(1)
    s.pause()

    # 28. 月销售额查询（2024-03）：8 + 25 + 9 = 42.00
    s.choice(5).line("2024 3 1").pause()

    # 29. 年销售额查询（2024）：42.00
    s.choice(6).line(2024).pause()

    # 30. 热度查询
    s.choice(7).pause()

    # 31. 管理员返回主菜单
    s.choice(0)

    # 32. 退出系统
    s.choice(0)

    return s.build()


def verify_full_scenario(output):
    """检查关键业务输出。"""
    # 登录与新增
    assert_contains(output, "登录成功")
    assert_contains(output, "用户新增成功")
    assert_contains(output, "图书入库成功")

    # 学生折扣（80%）
    assert_contains(output, "租书成功，结算价：8.00")
    assert_contains(output, "租书成功，结算价：12.00")
    assert_contains(output, "租书成功，结算价：9.60")

    # 未关闭订单上限
    assert_contains(output, "达到上限")
    assert_contains(output, "3")

    # 修改密码后登录成功
    assert_contains(output, "密码修改成功")

    # 老师折扣（90%）
    assert_contains(output, "租书成功，结算价：9.00")

    # 关闭订单
    assert_contains(output, "订单关闭成功，结算金额：8.00")
    assert_contains(output, "订单关闭成功，结算金额：25.00")
    assert_contains(output, "订单关闭成功，结算金额：9.00")

    # 销售额（月/年）
    assert_contains(output, "月销售额：42.00")
    assert_contains(output, "年销售额：42.00")

    # 热度查询中应出现三本书
    assert_contains(output, "C程序设计")
    assert_contains(output, "数据结构")
    assert_contains(output, "操作系统")

    # 正常退出
    assert_contains(output, "感谢使用，再见！")


# ---------------------------------------------------------------------------
# 边界/回归测试：库存不足 & 未关闭订单数
# ---------------------------------------------------------------------------

def build_edge_scenario():
    """在全新数据上测试库存不足和订单上限。"""
    s = ScriptBuilder()

    # 管理员初始化
    s.choice(3).line("admin").line("123456")

    # 添加学生
    s.choice(1)
    s.line("s1").line("p1").line(0)
    s.line("123").line("2021001").line("学生甲")
    s.line("2000 1 1").pause()

    # 添加库存为 1 的图书
    s.choice(2)
    s.line("1110000000001").line("唯一书")
    s.line(1).line(1)
    s.line("2020 1 1")
    s.line(1).line("10.00").line("5.00")
    s.pause()

    s.choice(0)

    # 学生租走唯一的一本
    s.choice(1).line("s1").line("p1")
    s.choice(4).line("1110000000001").line("2024 1 1").pause()

    # 再次租借同一本书，应提示库存不足
    s.choice(4).line("1110000000001").line("2024 1 2").pause()

    s.choice(0)

    # 退出
    s.choice(0)
    return s.build()


def verify_edge_scenario(output):
    assert_contains(output, "租书成功")
    assert_contains(output, "图书库存不足")


# ---------------------------------------------------------------------------
# 入口
# ---------------------------------------------------------------------------

def main():
    backup = backup_data()
    try:
        clean_data()
        exe = locate_executable()

        print("\n" + "=" * 60)
        print("运行主场景测试")
        print("=" * 60)
        out1 = run_session(exe, build_full_scenario())
        verify_full_scenario(out1)
        print("[PASS] 主场景测试通过")

        # 边界测试需要重新清空数据
        print("\n" + "=" * 60)
        print("运行边界场景测试")
        print("=" * 60)
        clean_data()
        out2 = run_session(exe, build_edge_scenario())
        verify_edge_scenario(out2)
        print("[PASS] 边界场景测试通过")

        print("\n" + "=" * 60)
        print("所有测试通过")
        print("=" * 60)

    except AssertFail as e:
        print("\n[FAIL] 测试失败：", e)
        sys.exit(1)
    finally:
        restore_data(backup)
        print("[INFO] 数据目录已恢复")


if __name__ == "__main__":
    main()
