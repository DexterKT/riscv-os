# Makefile

# 指定 RISC-V 交叉编译工具链的前缀
TOOLPREFIX = riscv64-unknown-elf-

# 定义编译器、链接器等
CC = $(TOOLPREFIX)gcc
LD = $(TOOLPREFIX)ld
AS = $(TOOLPREFIX)as

# 定义编译和链接的参数
CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb -MD -mcmodel=medany
CFLAGS += -fno-builtin
CFLAGS += -Iinclude
LDFLAGS = -T kernel/kernel.ld

# 定义需要编译的源文件
OBJS = \
  kernel/entry.o \
  kernel/main.o \
  kernel/uart.o \

# 最终的目标是生成内核文件 kernel.elf
all: kernel.elf

# 链接规则：如何从 .o 文件生成 .elf 文件
kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.elf $(OBJS)

# 编译规则：如何从 .c 或 .S 文件生成 .o 文件
# $< 是自动变量，代表第一个依赖文件
# $@ 是自动变量，代表目标文
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

# 启动 QEMU 的规则
qemu: kernel.elf
	qemu-system-riscv64 \
	  -machine virt \
	  -m 128M \
	  -nographic \
	  -bios none \
	  -kernel kernel.elf


# 用于调试的 qemu-gdb 规则
qemu-gdb: kernel.elf
	qemu-system-riscv64 \
	  -machine virt \
	  -m 128M \
	  -nographic \
	  -bios none \
	  -kernel kernel.elf \
	  -s -S


# 清理所有生成的文件，包括所有子目录
clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	rm -f kernel.elf