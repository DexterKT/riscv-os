// kernel/console.c
// 这是一个控制台抽象层，用于处理字符输出和终端控制，并新增输入缓冲区功能。

#include "../include/types.h"
#include "../include/defs.h"

// 假设 uart_putc 在 defs.h 中声明
void uart_putc(char c);

// --- ANSI Escape Codes ---
#define ANSI_CLEAR_SCREEN "\x1b[2J"
#define ANSI_CURSOR_HOME  "\x1b[H"
#define ANSI_CLEAR_LINE   "\x1b[K"

void console_putc(char c) {
    if (c == '\n') {
        uart_putc('\r');
    }
    uart_putc(c);
}


void console_puts(const char *s) {
    while (*s) {
        console_putc(*s);
        s++;
    }
}


void clear_screen(void) {
    console_puts(ANSI_CLEAR_SCREEN);
    console_puts(ANSI_CURSOR_HOME);
}

/* 清除从光标位置到行尾的所有内容 */

void clear_line(void) {
    console_puts(ANSI_CLEAR_LINE);
}

/*
 * 将光标移动到指定位置
 * row 行号（从1开始）
 * col 列号（从1开始）
 */

void goto_xy(int row, int col) {
    printf("\x1b[%d;%dH", row, col);
}

/*处理输入*/

/**
 * 处理来自 UART 的输入字符，并存入缓冲区
 * 从 UART 接收到的字符
 */

#define CONSOLE_BUF_SIZE 128
static char input_buf[CONSOLE_BUF_SIZE];
static int input_head = 0;
static int input_tail = 0;
static int input_ready = 0;

void console_intr(char c) {
    // 处理退格键
    if (c == '\b' || c == 0x7F) {
        if (input_head > input_tail) {
            input_head--;
            console_puts("\b \b"); // 回显退格
        }
        return;
    }

    // 检查缓冲区是否已满
    if (input_head - input_tail >= CONSOLE_BUF_SIZE) {
        return;
    }

    // 回显字符并存入缓冲区
    uart_putc(c);
    input_buf[input_head % CONSOLE_BUF_SIZE] = c;
    input_head++;
    
    // 如果是回车或缓冲区满了，标记为可读
    if (c == '\r' || (input_head - input_tail) == CONSOLE_BUF_SIZE) {
        input_ready = 1;
    }
}

char console_getc(void) {
    // 轮询等待，直到缓冲区中有可读的完整行
    while (!input_ready) {
        // 忙等待或进入睡眠
    }
    
    // 从缓冲区中取出字符并返回
    char c = input_buf[input_tail % CONSOLE_BUF_SIZE];
    input_tail++;
    
    // 如果一行已读完，重置状态
    if (input_tail == input_head) {
        input_ready = 0;
        input_tail = 0;
        input_head = 0;
    }
    
    return c;
}