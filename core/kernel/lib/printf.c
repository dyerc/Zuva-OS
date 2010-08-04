/* 
 * printf.c - Parses a va_list set of commands
 * 
 * 
 * This file is part of the Zuva Operating System. 
 * 
 * (C) Copyright Chris Dyer & Zuvium 2009 - 2010. Do Not Re-distribute. 
 * For internal viewing only.
 * 
 */

#include <console.h>
#include <utils.h>
#include <types.h>


void vprintf(const char* args, va_list ap)
{
    char buffer[32]; // Larger is not needed at the moment

    for (; *args; ++args)
    {
        switch (*args)
        {
            case '%':
                switch (*(++args))
                {
                    case 'u':
                        itoa(va_arg(ap, uint32_t), buffer);
                        console_puts(buffer);
                        break;
                    case 'f':
                        ftoa(va_arg(ap, double), buffer);
                        console_puts(buffer);
                        break;
                    case 'i': case 'd':
                        itoa(va_arg(ap, int32_t), buffer);
                        console_puts(buffer);
                        break;
                    case 'X': /// TODO: make it standardized
			console_puts("0x");
                        i2hex(va_arg(ap, int32_t), buffer, 8);
                        console_puts(buffer);
                        break;
                    case 'x':
                        i2hex(va_arg(ap, int32_t), buffer, 4);
                        console_puts(buffer);
                        break;
                    case 'y':
                        i2hex(va_arg(ap, int32_t), buffer, 2);
                        console_puts(buffer);
                        break;
                    case 's':
                        console_puts(va_arg (ap, char*));
                        break;
                    case 'c':
                        console_putchar((int8_t)va_arg(ap, int32_t));
                        break;
                    case '%':
                        console_putchar('%');
                        break;
                    default:
                        --args;
                        break;
                }
                break;
            default:
                console_putchar(*args);
                break;
        }
    }
}

void kprintf(const char *msg, ...)
{
    va_list ap;
    va_start (ap, msg);
    vprintf(msg, ap);;
}