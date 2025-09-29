// uart.c
void uart_putc(char c);
void uart_puts(char *s);
void console_putc(char c);
void console_puts(const char *s);
void clear_screen(void);
void goto_xy(int row, int col);
int printf(const char *fmt, ...);