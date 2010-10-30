/* vi: set sw=4 ts=4: */
/*
 * debug.c: This file is part of ____
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
#include <stdarg.h>
#include "qomowin.h"
#include "qw-debug.h"
#include "resource.h"

#define def_msgfunc(fname, msgid, btnid, iconid) \
	int msg_##fname (HWND hwnd, const char* fmt, ...) { \
		va_list ap;	\
		char msg[BUFSIZ];	\
		char szTitle[BUFSIZ];	\
		LoadString(hLangDll, IDS_MSG_##msgid, szTitle, sizeof(szTitle));	\
		va_start(ap, fmt);	\
		vsnprintf(msg, sizeof(msg), fmt, ap);	\
		va_end(ap);	\
		return qw_log(hwnd, MB_##btnid | MB_##iconid, szTitle, "%s", msg);	\
	}

static int qw_log(HWND hwnd, UINT type, const char* title, const char *fmt, ...);

static int qw_log(HWND hwnd, UINT type, const char* title, const char *fmt, ...)
{
	va_list ap;
	char msg[BUFSIZ];

	va_start(ap, fmt);
	vsnprintf(msg, sizeof(msg), fmt, ap);
	va_end(ap);
	return MessageBox(hwnd, msg, title, type);
}

int qw_msg(HWND hwnd, const char* file, int line, const char* func, const char *fmt, ...)
{
	va_list ap;
	char msg[BUFSIZ];

	va_start(ap, fmt);
	vsnprintf(msg, sizeof(msg), fmt, ap);
	va_end(ap);

	return msg_info(hwnd, "%s:%d %s() %s\n", file, line, func, msg);
}

def_msgfunc(info, INFO, OK, ICONINFORMATION);
def_msgfunc(warn, WARNING, OK, ICONWARNING);
def_msgfunc(error, ERROR, OK, ICONERROR);
def_msgfunc(yesno, QUESTION, YESNO, ICONQUESTION);

