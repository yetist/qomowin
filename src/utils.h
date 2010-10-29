/* vi: set sw=4 ts=4: */
/*
 * utils.h: This file is part of ____
 *
 * Copyright (C) 2010 yetist <yetist@gmail.com>
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

#ifndef __UTILS_H__ 
#define __UTILS_H__  1
#include <windows.h>
#include <stdio.h>

HMODULE		LoadLangDll(const char* locale);
const char * winpidgin_get_locale(void);
BOOL GetPrivileges(void);
BOOL getExeDir(char* path);
int ReBoot(int reboot);
void PrintError(HWND hwnd, TCHAR* msg);
BOOL CheckNtLdr(HWND hwnd);
BOOL ExtractISO(HWND hwnd);
BOOL UpdateGrubCfg(HWND hwnd);
BOOL CheckBootMgr(HWND hwnd);
BOOL FormatDriver(HWND hwnd, const char* driver, const char* label);
BOOL ISO2USB(HWND hwnd, const char* driver, const char* label);

#endif /* __UTILS_H__ */

/*
vi:ts=4:wrap:ai:
*/
