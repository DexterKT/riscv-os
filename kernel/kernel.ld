/*  它将内核代码和数据组织在物理地址 0x80000000  */

/* 指定目标架构为 RISC-V 64位 */
OUTPUT_ARCH("riscv")

/* 指定程序入口点为 _entry，这是你的汇编代码的起始标签 */
ENTRY(_entry)

SECTIONS
{
  /*
   * 这是 QEMU 加载内核的默认物理地址。
   */
  . = 0x80000000;

  /*
   * .text 段：存放所有可执行代码
   * *(.text .text.*) 匹配所有 .text 和 .text.* 的段
   */
  .text : {
    *(.text .text.*)
    /* 定义 etext 符号，标记代码段的结束 */
    PROVIDE(etext = .);
  }

  /*
   * .rodata 段：存放只读数据，如字符串字面量
   */
  .rodata : {
    . = ALIGN(16);
    *(.rodata .rodata.*)
  }

  /*
   * .data 段：存放已初始化的全局变量
   */
  .data : {
    . = ALIGN(16);
    *(.data .data.*)
    /* 定义 edata 符号，标记数据段的结束 */
    PROVIDE(edata = .);
  }

  /*
   * .bss 段：存放未初始化的全局变量
   * 这些变量的值在程序启动时被初始化为0
   */
  .bss : {
    . = ALIGN(16);
    /* 定义 sbss 符号，标记 BSS 段的开始 */
    PROVIDE(sbss = .);
    *(.sbss .sbss.*)
    *(.bss .bss.*)
    /* 定义 ebss 符号，标记 BSS 段的结束 */
    PROVIDE(ebss = .);
  }

  /*
   * 定义 end 符号，标记整个内核镜像的结束地址
   */
  PROVIDE(end = .);
}