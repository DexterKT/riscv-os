// kernel/printf.c
// Formatted console output -- printf, panic.

#include <stdarg.h>

#include "../include/types.h"
#include "../include/defs.h"

// Assume these functions are declared in defs.h
void console_putc(char c);
void console_puts(const char *s);

static char digits[] = "0123456789abcdef";

/**
 * @brief Converts and prints a number to the console.
 * @param num The number to print.
 * @param base The numeric base (e.g., 10 for decimal, 16 for hex).
 * @param sign 1 for signed, 0 for unsigned.
 */
static void print_number(long long num, int base, int sign) {
    char buf[64];
    int i = 0;
    unsigned long long x;

    // Handle the special case of INT_MIN to prevent overflow
    if (sign && num == -2147483648LL) {
        x = -(num / 10);
        buf[i++] = digits[-(num % 10)];
    } else if (sign && num < 0) {
        x = -num;
    } else {
        x = num;
    }

    // Convert number to string in reverse order
    do {
        buf[i++] = digits[x % base];
    } while ((x /= base) != 0);

    // Add the negative sign if needed
    if (sign && num < 0) {
        buf[i++] = '-';
    }

    // Print the string in the correct order
    while (--i >= 0) {
        console_putc(buf[i]);
    }
}

/**
 * @brief Prints formatted output to the console.
 * @param fmt The format string.
 * @param ... Variable arguments based on the format string.
 */
int printf(const char *fmt, ...) {
    va_list ap;
    const char *s;
    char c;

    // Start processing variable arguments
    va_start(ap, fmt);

    // Iterate through the format string
    for (int i = 0; (c = fmt[i]) != 0; i++) {
        // If it's a normal character, print it directly
        if (c != '%') {
            console_putc(c);
            continue;
        }

        // It's a '%' character, so handle the format specifier
        c = fmt[++i];
        if (c == 0) {
            break;
        }

        switch (c) {
            case 'd':
                print_number(va_arg(ap, int), 10, 1);
                break;
            case 'x':
                print_number(va_arg(ap, unsigned int), 16, 0);
                break;
            case 'p':
                console_puts("0x");
                print_number(va_arg(ap, unsigned long), 16, 0);
                break;
            case 's':
                s = va_arg(ap, const char *);
                if (s == 0) {
                    s = "(null)";
                }
                console_puts(s);
                break;
            case 'c':
                console_putc(va_arg(ap, int));
                break;
            case '%':
                console_putc('%');
                break;
            default:
                // Print the unknown % sequence
                console_putc('%');
                console_putc(c);
                break;
        }
    }

    // Clean up variable arguments
    va_end(ap);
    return 0;
}