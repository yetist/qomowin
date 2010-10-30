/* vi: set sw=4 ts=4: */
/*
 * debug.h: This file is part of ____
 *
 * Copyright (C) 2010 yetist <wuxiaotian@redflag-linux.com>
 *
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */

#ifndef __DEBUG_H__ 
#define __DEBUG_H__  1

#include <windows.h>
#include <stdio.h>

int qw_msg(HWND hwnd, const char* file, int line, const char* func, const char *fmt, ...);

int msg_info(HWND hwnd, const char* fmt, ...);
int msg_warn(HWND hwnd, const char* fmt, ...);
int msg_error(HWND hwnd, const char* fmt, ...);
int msg_yesno(HWND hwnd, const char* fmt, ...);

#ifdef _DEBUG
#	define debug_msg(handle, arg...) qw_msg(handle, __FILE__, __LINE__, __FUNCTION__, ##arg);
#else
#	define debug_msg(handle, arg...)
#endif

#endif /* __DEBUG_H__ */

/*
vi:ts=4:wrap:ai:
*/
