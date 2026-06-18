#include <stdio.h>

/**
 * 递归函数：计算第 day 天早上的桃子个数
 * @param day: 天数 (1~10)
 * @return: 第 day 天早上的桃子个数
 */
int peach(int day) {
    // 递归终止条件：第10天早上只剩下1个桃子
    if (day == 10) {
        return 1;
    }
    // 递归关系：第day天的桃子数 = 2 * (第day+1天的桃子数 + 1)
    return 2 * (peach(day + 1) + 1);
}

int main() {
    int day = 1;
    int result = peach(day);
    
    printf("猴子第1天共摘了 %d 个桃子。\n", result);
    
    // 验证输出：打印每天的桃子数量
    printf("\n验证每天的桃子数量：\n");
    for (int i = 1; i <= 10; i++) {
        printf("第 %2d 天早上：%4d 个桃子\n", i, peach(i));
    }
    
    return 0;
}