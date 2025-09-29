
#include "../include/defs.h"

// 声明汇编代码中的符号
extern char sbss[];
extern char ebss[];

// 定义内核栈空间
// __attribute__ ((aligned (16))) 确保栈数组按16字节对齐

#define STACK_SIZE 4096
__attribute__ ((aligned (16))) char stack0[STACK_SIZE];
char *stack_top = stack0 + STACK_SIZE;

// C语言入口函数
int main() {
    // 清零BSS段
    char *p;
    for (p = sbss; p < ebss; p++) {
        *p = 0;
    }

    // 输出 "Hello OS"
    uart_puts("Hello OS\n");

    printf("Hello OS by printf! \n");

    // 进入无限循环
    for(;;) {}
}

