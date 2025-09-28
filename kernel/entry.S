# kernel/entry.S
# QEMU 默认将内核加载到 0x80000000 地址并从这里开始执行
# 链接脚本 (kernel.ld) 确保 _entry 标签在这个地址

.section .text
.global _entry

_entry:
    # 设置栈指针
    # la加载stack0数组起始地址（lowest
    # add后sp指向栈顶
    la  sp,stack0
    li  t0,4096
    add sp,sp,t0

    # 清空.bss段
    la  t0,sbss
    la  t1,ebss
bss_loop:
# 使用 sd 一次写入8个字节的0
    la  a0,0
    sd  a0,0(t0)
    addi t0,t0,8
    blt t0,t1,bss_loop

    # 跳转到C语言的main函数
    call main

    # 如果main函数返回，进入无限循环
spin:
    j spin