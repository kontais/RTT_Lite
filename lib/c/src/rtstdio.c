/*
 * File      : rtstdio.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2012, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-16     Bernard      the first version
 */
#include <os.h>

/* use precision */
#define PRINTF_PRECISION

/**
 * @addtogroup KernelService
 */

/* private function */
#define isdigit(c)  ((unsigned)((c) - '0') < 10)

STATIC_INLINE int32_t divide(int32_t *n, int32_t base)
{
    int32_t res;

    /* optimized for processor which does not support divide instructions. */
    if (base == 10) {
        res = ((uint32_t)*n) % 10U;
        *n = ((uint32_t)*n) / 10U;
    } else {
        res = ((uint32_t)*n) % 16U;
        *n = ((uint32_t)*n) / 16U;
    }

    return res;
}

STATIC_INLINE int skip_atoi(const char **s)
{
    register int i=0;
    while (isdigit(**s))
        i = i * 10 + *((*s)++) - '0';

    return i;
}

#define ZEROPAD     (1 << 0)    /* pad with zero */
#define SIGN        (1 << 1)    /* unsigned/signed int */
#define PLUS        (1 << 2)    /* show plus */
#define SPACE       (1 << 3)    /* space if plus */
#define LEFT        (1 << 4)    /* left justified */
#define SPECIAL     (1 << 5)    /* 0x */
#define LARGE       (1 << 6)    /* use 'ABCDEF' instead of 'abcdef' */

#ifdef PRINTF_PRECISION
static char *print_number(char *buf,
                          char *end,
                          int   num,
                          int   base,
                          int   s,
                          int   precision,
                          int   type)
#else
static char *print_number(char *buf,
                          char *end,
                          int   num,
                          int   base,
                          int   s,
                          int   type)
#endif
{
    char c, sign;
#ifdef PRINTF_LONGLONG
    char tmp[32];
#else
    char tmp[16];
#endif
    const char *digits;
    static const char small_digits[] = "0123456789abcdef";
    static const char large_digits[] = "0123456789ABCDEF";
    register int i;
    register int size;

    size = s;

    digits = (type & LARGE) ? large_digits : small_digits;
    if (type & LEFT)
        type &= ~ZEROPAD;

    c = (type & ZEROPAD) ? '0' : ' ';

    /* get sign */
    sign = 0;
    if (type & SIGN) {
        if (num < 0) {
            sign = '-';
            num = -num;
        } else if (type & PLUS)
            sign = '+';
        else if (type & SPACE)
            sign = ' ';
    }

#ifdef PRINTF_SPECIAL
    if (type & SPECIAL) {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }
#endif

    i = 0;
    if (num == 0)
        tmp[i++]='0';
    else
    {
        while (num != 0)
            tmp[i++] = digits[divide(&num, base)];
    }

#ifdef PRINTF_PRECISION
    if (i > precision)
        precision = i;
    size -= precision;
#else
    size -= i;
#endif

    if (!(type&(ZEROPAD | LEFT))) {
        if ((sign)&&(size>0))
            size--;

        while (size-->0) {
            if (buf <= end)
                *buf = ' ';
           buf++;
        }
    }

    if (sign) {
        if (buf <= end) {
            *buf = sign;
            size--;
        }
       buf++;
    }

#ifdef PRINTF_SPECIAL
    if (type & SPECIAL) {
        if (base==8) {
            if (buf <= end)
                *buf = '0';
           buf++;
        } else if (base == 16) {
            if (buf <= end)
                *buf = '0';
           buf++;
            if (buf <= end) {
                *buf = type & LARGE? 'X' : 'x';
            }
           buf++;
        }
    }
#endif

    /* no align to the left */
    if (!(type & LEFT)) {
        while (size-- > 0) {
            if (buf <= end)
                *buf = c;
           buf++;
        }
    }

#ifdef PRINTF_PRECISION
    while (i < precision--) {
        if (buf <= end)
            *buf = '0';
       buf++;
    }
#endif

    /* put number in the temporary buffer */
    while (i-- > 0) {
        if (buf <= end)
            *buf = tmp[i];
       buf++;
    }

    while (size-- > 0) {
        if (buf <= end)
            *buf = ' ';
       buf++;
    }

    return buf;
}

int32_t os_vsnprintf(char       *buf,
                        size_t   size,
                        const char *fmt,
                        va_list     args)
{
#ifdef PRINTF_LONGLONG
    unsigned int64_t num;
#else
    uint32_t num;
#endif
    int i, len;
    char *str, *end, c;
    const char *s;

    uint8_t base;            /* the base of number */
    uint8_t flags;           /* flags to print number */
    uint8_t qualifier;       /* 'h', 'l', or 'L' for integer fields */
    int32_t field_width;     /* width of output field */

#ifdef PRINTF_PRECISION
    int precision;      /* min. # of digits for integers and max for a string */
#endif

    str = buf;
    end = buf + size - 1;

    /* Make sure end is always >= buf */
    if (end < buf) {
        end  = ((char *)-1);
        size = end - buf;
    }

    for (; *fmt ; fmt++) {
        if (*fmt != '%') {
            if (str <= end)
                *str = *fmt;
           str++;
            continue;
        }

        /* process flags */
        flags = 0;

        while (1) {
            /* skips the first '%' also */
           fmt++;
            if (*fmt == '-') flags |= LEFT;
            else if (*fmt == '+') flags |= PLUS;
            else if (*fmt == ' ') flags |= SPACE;
            else if (*fmt == '#') flags |= SPECIAL;
            else if (*fmt == '0') flags |= ZEROPAD;
            else break;
        }

        /* get field width */
        field_width = -1;
        if (isdigit(*fmt)) field_width = skip_atoi(&fmt);
        else if (*fmt == '*') {
           fmt++;
            /* it's the next argument */
            field_width = va_arg(args, int);
            if (field_width < 0) {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

#ifdef PRINTF_PRECISION
        /* get the precision */
        precision = -1;
        if (*fmt == '.') {
           fmt++;
            if (isdigit(*fmt)) precision = skip_atoi(&fmt);
            else if (*fmt == '*') {
               fmt++;
                /* it's the next argument */
                precision = va_arg(args, int);
            }
            if (precision < 0) precision = 0;
        }
#endif
        /* get the conversion qualifier */
        qualifier = 0;
#ifdef PRINTF_LONGLONG
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
#else
        if (*fmt == 'h' || *fmt == 'l')
#endif
        {
            qualifier = *fmt;
           fmt++;
#ifdef PRINTF_LONGLONG
            if (qualifier == 'l' && *fmt == 'l') {
                qualifier = 'L';
               fmt++;
            }
#endif
        }

        /* the default base */
        base = 10;

        switch (*fmt) {
        case 'c':
            if (!(flags & LEFT)) {
                while (--field_width > 0) {
                    if (str <= end) *str = ' ';
                   str++;
                }
            }

            /* get character */
            c = (uint8_t)va_arg(args, int);
            if (str <= end) *str = c;
           str++;

            /* put width */
            while (--field_width > 0) {
                if (str <= end) *str = ' ';
               str++;
            }
            continue;

        case 's':
            s = va_arg(args, char *);
            if (!s) s = "(NULL)";

            len = os_strlen(s);
#ifdef PRINTF_PRECISION
            if (precision > 0 && len > precision) len = precision;
#endif

            if (!(flags & LEFT)) {
                while (len < field_width--) {
                    if (str <= end) *str = ' ';
                   str++;
                }
            }

            for (i = 0; i < len; i++) {
                if (str <= end) *str = *s;
               str++;
               ++s;
            }

            while (len < field_width--) {
                if (str <= end) *str = ' ';
               str++;
            }
            continue;

        case 'p':
            if (field_width == -1) {
                field_width = sizeof(void *) << 1;
                flags |= ZEROPAD;
            }
#ifdef PRINTF_PRECISION
            str = print_number(str, end,
                               (int)va_arg(args, void *),
                               16, field_width, precision, flags);
#else
            str = print_number(str, end,
                               (int)va_arg(args, void *),
                               16, field_width, flags);
#endif
            continue;

        case '%':
            if (str <= end) *str = '%';
           str++;
            continue;

            /* integer number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;

        case 'X':
            flags |= LARGE;
        case 'x':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;

        default:
            if (str <= end) *str = '%';
           str++;

            if (*fmt) {
                if (str <= end) *str = *fmt;
            	str++;
            } else {
                fmt--;
            }
            continue;
        }

#ifdef PRINTF_LONGLONG
        if (qualifier == 'L') num = va_arg(args, int64_t);
        else if (qualifier == 'l')
#else
        if (qualifier == 'l')
#endif
        {
            num = va_arg(args, uint32_t);
            if (flags & SIGN) num = (int32_t)num;
        } else if (qualifier == 'h') {
            num = (uint16_t)va_arg(args, int32_t);
            if (flags & SIGN) num = (int16_t)num;
        } else {
            num = va_arg(args, uint32_t);
            if (flags & SIGN) num = (int32_t)num;
        }
#ifdef PRINTF_PRECISION
        str = print_number(str, end, num, base, field_width, precision, flags);
#else
        str = print_number(str, end, num, base, field_width, flags);
#endif
    }

    if (str <= end) *str = '\0';
    else *end = '\0';

    /* the trailing null byte doesn't count towards the total
    *str++;
    */
    return str - buf;
}

/**
 * This function will fill a formatted string to buffer
 *
 * @param buf the buffer to save formatted string
 * @param size the size of buffer
 * @param fmt the format
 */
int32_t os_snprintf(char *buf, size_t size, const char *fmt, ...)
{
    int32_t n;
    va_list args;

    va_start(args, fmt);
    n = os_vsnprintf(buf, size, fmt, args);
    va_end(args);

    return n;
}

/**
 * This function will fill a formatted string to buffer
 *
 * @param buf the buffer to save formatted string
 * @param arg_ptr the arg_ptr
 * @param format the format
 */
int32_t os_vsprintf(char *buf, const char *format, va_list arg_ptr)
{
    return os_vsnprintf(buf, (size_t) -1, format, arg_ptr);
}

/**
 * This function will fill a formatted string to buffer
 *
 * @param buf the buffer to save formatted string
 * @param format the format
 */
int32_t os_sprintf(char *buf, const char *format, ...)
{
    int32_t n;
    va_list arg_ptr;

    va_start(arg_ptr, format);
    n = os_vsprintf(buf ,format, arg_ptr);
    va_end(arg_ptr);

    return n;
}

/**
 * This function will print a formatted string on system console
 *
 * @param fmt the format
 */
void printk(const char *fmt, ...)
{
    va_list args;
    size_t length;
    static char os_log_buf[OS_CONSOLE_BUF_SIZE];

    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the os_log_buf, we have to adjust the output
     * length. */
    length = os_vsnprintf(os_log_buf, sizeof(os_log_buf) - 1, fmt, args);
    if (length > OS_CONSOLE_BUF_SIZE - 1) {
        length = OS_CONSOLE_BUF_SIZE - 1;
    }

    console_output(os_log_buf);

    va_end(args);
}
