#include <stdio.h>
#include <string.h>

typedef void (*ActionFunc)(void);

typedef struct {
    char *command;
    ActionFunc action;
} Instruction;

void start_action(void)
{
    printf("执行操作：启动设备\n");
}

void stop_action(void)
{
    printf("执行操作：停止设备\n");
}

void reset_action(void)
{
    printf("执行操作：重置系统\n");
}

void status_action(void)
{
    printf("执行操作：查询状态\n");
}

int main(void)
{
    Instruction instructions[] = {
        {"start", start_action},
        {"stop", stop_action},
        {"reset", reset_action},
        {"status", status_action}
    };
    const int count = sizeof(instructions) / sizeof(instructions[0]);
    char input[64];

    printf("请输入操作指令 (start stop reset status): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }

    input[strcspn(input, "\r\n")] = '\0';

    for (int i = 0; i < count; i++) {
        if (strcmp(input, instructions[i].command) == 0) {
            instructions[i].action();
            return 0;
        }
    }

    printf("未知指令：%s\n", input);
    return 0;
}
