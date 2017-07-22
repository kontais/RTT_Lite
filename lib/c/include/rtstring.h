/*
 * File      : rtstring.h
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

#ifndef __OS_STRING_H__
#define __OS_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif

void *os_memset(void *s, int c, uint32_t count);
void *os_memcpy(void *dst, const void *src, uint32_t count);
void *os_memmove(void *dest, const void *src, uint32_t n);

int32_t os_strcmp(const char *cs, const char *ct);
int32_t os_strncmp(const char *cs, const char *ct, uint32_t count);
size_t os_strlen(const char *s);

char *os_strstr(const char *s1, const char *s2);
char *os_strncpy(char *dst, const char *src, uint32_t n);

int32_t  os_memcmp(const void *cs, const void *ct, uint32_t count);
uint32_t os_strcasecmp(const char *a, const char *b);

#ifdef __cplusplus
}
#endif

#endif /* __OS_STRING_H__ */
