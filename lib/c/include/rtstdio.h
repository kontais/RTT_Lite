/*
 * File      : rtstdio.h
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

#ifndef __OS_STDIO_H__
#define __OS_STDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

int32_t os_vsprintf(char *dest, const char *format, va_list arg_ptr);
int32_t os_vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int32_t os_sprintf(char *buf ,const char *format, ...);
int32_t os_snprintf(char *buf, size_t size, const char *format, ...);

void printk(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __OS_STDIO_H__ */
