// kernel/uart.c
// 这是一个极简的串口驱动，用于通过轮询向QEMU输出。

#include "../include/types.h"
#include "../include/memlayout.h"

// UART 设备的基地址，由QEMU模拟器指定
#define UART0 0x10000000

// UART 寄存器偏移量
#define THR 0 // Transmit Holding Register
#define LSR 5 // Line Status Register

// LSR 寄存器的 THRE (Transmit Holding Register Empty) 位
#define LSR_THRE (1 << 5)

// 将地址转换为volatile指针
#define Reg(reg) ((volatile unsigned char *)(UART0 + reg))

// 读取寄存器
#define ReadReg(reg) (*(Reg(reg)))
// 写入寄存器
#define WriteReg(reg, v) (*(Reg(reg)) = (v))


// 输出单个字符到UART
void uart_putc(char c) {
    // 轮询等待，直到THR寄存器空闲
    while ((ReadReg(LSR) & LSR_THRE) == 0) {
        // 忙等待
    }
    // 当THR空闲时，写入字符
    WriteReg(THR, c);
}

// 输出字符串到UART
void uart_puts(char *s) {
    while (*s) {
        uart_putc(*s);
        s++;
    }
}
