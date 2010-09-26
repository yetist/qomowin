/* vi: set sw=4 ts=4: */
/*
 * utils.c: This file is part of ____
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

#include <shlwapi.h>
#include "qomowin.h"
#include "resource.h"
#include "utils.h"

static char* winpidgin_lcid_to_posix(LCID lcid);

static char* winpidgin_lcid_to_posix(LCID lcid) 
{
	char *posix = NULL;
	int lang_id = PRIMARYLANGID(lcid);
	int sub_id = SUBLANGID(lcid);

	switch (lang_id) {
		case LANG_AFRIKAANS: posix = "af"; break;
		case LANG_ARABIC: posix = "ar"; break;
		case LANG_AZERI: posix = "az"; break;
		case LANG_BENGALI: posix = "bn"; break;
		case LANG_BULGARIAN: posix = "bg"; break;
		case LANG_CATALAN: posix = "ca"; break;
		case LANG_CZECH: posix = "cs"; break;
		case LANG_DANISH: posix = "da"; break;
		case LANG_ESTONIAN: posix = "et"; break;
		case LANG_PERSIAN: posix = "fa"; break;
		case LANG_GERMAN: posix = "de"; break;
		case LANG_GREEK: posix = "el"; break;
		case LANG_ENGLISH:
			switch (sub_id) {
				case SUBLANG_ENGLISH_UK:
					posix = "en_GB"; break;
				case SUBLANG_ENGLISH_AUS:
					posix = "en_AU"; break;
				case SUBLANG_ENGLISH_CAN:
					posix = "en_CA"; break;
				default:
					posix = "en"; break;
			}
			break;
		case LANG_SPANISH: posix = "es"; break;
		case LANG_BASQUE: posix = "eu"; break;
		case LANG_FINNISH: posix = "fi"; break;
		case LANG_FRENCH: posix = "fr"; break;
		case LANG_GALICIAN: posix = "gl"; break;
		case LANG_GUJARATI: posix = "gu"; break;
		case LANG_HEBREW: posix = "he"; break;
		case LANG_HINDI: posix = "hi"; break;
		case LANG_HUNGARIAN: posix = "hu"; break;
		case LANG_ICELANDIC: break;
		case LANG_INDONESIAN: posix = "id"; break;
		case LANG_ITALIAN: posix = "it"; break;
		case LANG_JAPANESE: posix = "ja"; break;
		case LANG_GEORGIAN: posix = "ka"; break;
		case LANG_KANNADA: posix = "kn"; break;
		case LANG_KOREAN: posix = "ko"; break;
		case LANG_LITHUANIAN: posix = "lt"; break;
		case LANG_MACEDONIAN: posix = "mk"; break;
		case LANG_DUTCH: posix = "nl"; break;
		case LANG_NEPALI: posix = "ne"; break;
		case LANG_NORWEGIAN:
			switch (sub_id) {
				case SUBLANG_NORWEGIAN_BOKMAL:
					posix = "nb"; break;
				case SUBLANG_NORWEGIAN_NYNORSK:
					posix = "nn"; break;
			}
			break;
		case LANG_PUNJABI: posix = "pa"; break;
		case LANG_POLISH: posix = "pl"; break;
		case LANG_PASHTO: posix = "ps"; break;
		case LANG_PORTUGUESE:
			switch (sub_id) {
				case SUBLANG_PORTUGUESE_BRAZILIAN:
					posix = "pt_BR"; break;
				default:
				posix = "pt"; break;
			}
			break;
		case LANG_ROMANIAN: posix = "ro"; break;
		case LANG_RUSSIAN: posix = "ru"; break;
		case LANG_SLOVAK: posix = "sk"; break;
		case LANG_SLOVENIAN: posix = "sl"; break;
		case LANG_ALBANIAN: posix = "sq"; break;
		/* LANG_CROATIAN == LANG_SERBIAN == LANG_BOSNIAN */
		case LANG_SERBIAN:
			switch (sub_id) {
				case SUBLANG_SERBIAN_LATIN:
					posix = "sr@Latn"; break;
				case SUBLANG_SERBIAN_CYRILLIC:
					posix = "sr"; break;
				case SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_CYRILLIC:
				case SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_LATIN:
					posix = "bs"; break;
				case SUBLANG_CROATIAN_BOSNIA_HERZEGOVINA_LATIN:
					posix = "hr"; break;
			}
			break;
		case LANG_SWEDISH: posix = "sv"; break;
		case LANG_TAMIL: posix = "ta"; break;
		case LANG_TELUGU: posix = "te"; break;
		case LANG_THAI: posix = "th"; break;
		case LANG_TURKISH: posix = "tr"; break;
		case LANG_UKRAINIAN: posix = "uk"; break;
		case LANG_VIETNAMESE: posix = "vi"; break;
		case LANG_XHOSA: posix = "xh"; break;
		case LANG_CHINESE:
			switch (sub_id) {
				case SUBLANG_CHINESE_SIMPLIFIED:
					posix = "zh_CN"; break;
				case SUBLANG_CHINESE_TRADITIONAL:
					posix = "zh_TW"; break;
				default:
					posix = "zh"; break;
			}
			break;
		case LANG_URDU: break;
		case LANG_BELARUSIAN: break;
		case LANG_LATVIAN: break;
		case LANG_ARMENIAN: break;
		case LANG_FAEROESE: break;
		case LANG_MALAY: break;
		case LANG_KAZAK: break;
		case LANG_KYRGYZ: break;
		case LANG_SWAHILI: break;
		case LANG_UZBEK: break;
		case LANG_TATAR: break;
		case LANG_ORIYA: break;
		case LANG_MALAYALAM: break;
		case LANG_ASSAMESE: break;
		case LANG_MARATHI: break;
		case LANG_SANSKRIT: break;
		case LANG_MONGOLIAN: break;
		case LANG_KONKANI: break;
		case LANG_MANIPURI: break;
		case LANG_SINDHI: break;
		case LANG_SYRIAC: break;
		case LANG_KASHMIRI: break;
		case LANG_DIVEHI: break;
	}

	/* Deal with exceptions */
	if (posix == NULL) {
		switch (lcid) {
			case 0x0455: posix = "my_MM"; break; /* Myanmar (Burmese) */
			case 9999: posix = "ku"; break; /* Kurdish (from NSIS) */
		}
	}

	return posix;
}


/* Determine and set Pidgin locale as follows (in order of priority):
   - Check PIDGINLANG env var
   - Check NSIS Installer Language reg value
   - Use default user locale
*/
const char *winpidgin_get_locale(void) 
{
	const char *locale = NULL;
	LCID lcid;

	lcid = GetUserDefaultLCID();
	if ((locale = winpidgin_lcid_to_posix(lcid)))
		return locale;

	return "en";
}

HMODULE	LoadLangDll(const char* locale)
{
	char 		DllPath[MAX_PATH];
	HMODULE		hDLL;

	if( GetCurrentDirectory(MAX_PATH, DllPath))
	{
		strcat(DllPath, "\\locale\\");
		strcat(DllPath, locale);
		strcat(DllPath, ".dll");
		hDLL = LoadLibrary(DllPath);
		if( hDLL )
			return hDLL;
		else {   // try the primary language ID
			return NULL;
		}
	}
	else
		return NULL;

}

/**
 * GetPrivileges:
 *
 * 提升权限
 *
 * Return value: 
 **/
BOOL GetPrivileges(void)
{
    // 取得当前进程的[Token](标识)句柄
    HANDLE hToken;
    TOKEN_PRIVILEGES stTokenPrivilege;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return FALSE;

    // 取得关闭系统的[LUID](本地唯一的标识符)值
    if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &stTokenPrivilege.Privileges[0].Luid))
        return FALSE;

    // 设置特权数组的元素个数
    stTokenPrivilege.PrivilegeCount = 1;

    // 设置[LUID]的属性值
    stTokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // 为当前进程取得DEBUG权限
    if (!AdjustTokenPrivileges(hToken, FALSE, &stTokenPrivilege, 0, NULL, NULL))
        return FALSE;

    return TRUE;
}

int Boot(int reboot)
{

	if(reboot) {
		if(ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0))
			return 0;
	}
	else {
		if(ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0))
			return 0;
	}

	return 1;
}

void PrintError(HWND hwnd, TCHAR* msg)
{
  DWORD eNum;
  TCHAR sysMsg[BUFSIZ];
  TCHAR showmsg[BUFSIZ];
  TCHAR* p;
  TCHAR szTitle[BUFSIZ];

  LoadString(hLangDll, IDS_MSG_ERROR, szTitle, BUFSIZ);

  eNum = GetLastError();
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         sysMsg, 256, NULL );

  /* 去掉末尾的换行符号 */
  p = sysMsg;
  while(p && (*p))
	  ++p;

  do {
	  *p-- = '\0'; 
  } while(( p >= sysMsg ) && (( *p == '\r' ) || (*p == '\n')));

  // Display the message
  sprintf(showmsg, "%s\n[ErrorID=%ld]%s", msg, eNum, sysMsg);
  MessageBox(hwnd, showmsg, szTitle, MB_OK | MB_ICONWARNING);
}

BOOL CopyMbrFiles(HWND hwnd, const char* sysDriver)
{
	/* sysDriver = "X:\" */
	char *p, srcFileName[MAX_PATH], dstFileName[MAX_PATH];
	DWORD dwAttrs;

	HINSTANCE hInstance = GetModuleHandle(NULL);
	GetModuleFileName(hInstance, srcFileName, MAX_PATH);

	p = strrchr(srcFileName, '\\');
	*p = '\0';

	/* Copy qomoldr.mbr file */
	strcat(srcFileName, "\\winboot\\qomoldr");
	snprintf(dstFileName, MAX_PATH, "%sqomoldr", sysDriver);
	dwAttrs = GetFileAttributes(dstFileName); 
	if (dwAttrs==INVALID_FILE_ATTRIBUTES) dwAttrs = FILE_ATTRIBUTE_READONLY;
	if (dwAttrs & FILE_ATTRIBUTE_READONLY || dwAttrs & FILE_ATTRIBUTE_HIDDEN)
	{ 
		SetFileAttributes(dstFileName, dwAttrs & ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_HIDDEN); 
	}
	if (!CopyFile(srcFileName, dstFileName, FALSE))
	{
		PrintError(hwnd, "Copyfile qomoldr error!");
		return FALSE;
	}
	if (!SetFileAttributes(dstFileName, dwAttrs | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN))
		PrintError(hwnd, "SetFileAttributes Failed");

	/* Copy qomoldr.mbr file */
	strcat(srcFileName, ".mbr");
	strcat(dstFileName, ".mbr");
	dwAttrs = GetFileAttributes(dstFileName);
	if (dwAttrs==INVALID_FILE_ATTRIBUTES) dwAttrs = FILE_ATTRIBUTE_READONLY;
	if (dwAttrs & FILE_ATTRIBUTE_READONLY || dwAttrs & FILE_ATTRIBUTE_HIDDEN)
	{ 
		SetFileAttributes(dstFileName, dwAttrs & ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_HIDDEN); 
	}
	if (!CopyFile(srcFileName, dstFileName, FALSE))
	{
		PrintError(hwnd, "Copyfile qomoldr.mbr error!");
		return FALSE;
	}
	if (!SetFileAttributes(dstFileName, dwAttrs | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN))
		PrintError(hwnd, "SetFileAttributes Failed");
	return TRUE;
}

BOOL CheckNtLdr(HWND hwnd)
{
	char bootIni[BUFSIZ], *p;
	int isInstalled = 0;

	/* 得到系统盘根目录的 X:/boot.ini 文件 */
	if(!GetSystemDirectory(bootIni, BUFSIZ))
	{
		PrintError(hwnd, "GetSystemDirectory Failed!"); 
		return FALSE;
	}
	p = bootIni + 3;
	memset(p, '\0', BUFSIZ - 3);
	strncpy(p, "boot.ini", 8);

	if (PathFileExists(bootIni))  /* 检查 X:/boot.ini 文件 */
	{
		unsigned int readCount;
		char  keys[BUFSIZ];
		char *p;

		readCount= GetPrivateProfileSection("operating systems", keys, BUFSIZ, bootIni);
		p = keys;
		while (p < (keys + readCount))
		{
			printf("->[%s]\n", p);
			if (strstr(p, "\\qomoldr.mbr") != NULL)
			{
				isInstalled = 1;
				break;
			}
			p = p + strlen(p) + 1;
		}

		if (!isInstalled) /* 在 X:/boot.ini 文件中未发现qomoldr.mbr引导项 */
		{
			char key[20];
			DWORD dwAttrs;
			memset(key, '\0', sizeof(key));
			strncpy(key, bootIni, 3);		/* 这里key="X:\" */
			if (! CopyMbrFiles(hwnd, key))
				return FALSE;
			strcat(key, "qomoldr.mbr");		/* 这里 key = "X:\qomoldr.mbr" */

			dwAttrs = GetFileAttributes(bootIni); 
			if (dwAttrs==INVALID_FILE_ATTRIBUTES)
				return FALSE;

			if (dwAttrs & FILE_ATTRIBUTE_READONLY)
			{ 
				SetFileAttributes(bootIni, dwAttrs & ~FILE_ATTRIBUTE_READONLY); 
			} 
			if (!WritePrivateProfileString("operating systems", key, "Qomo Linux", bootIni)) 
			{
				PrintError(hwnd, "Modify boot.ini file failed!");
				return FALSE;
			} else {
				SetFileAttributes(bootIni, dwAttrs); 
				return TRUE;
			}
		}
		else
		{
			/* qomoldr.mbr引导项已经安装在 X:/boot.ini 文件中 */
			return TRUE;
		}
	}
	else 
	{
		/* 在2000、XP、2003系统上看起来丢失了/boot.ini文件，这通常不太可能，如果真的发生了，系统应该已经无法启动了，
		 * 所以，这种情况下已经完全没有必要去增加QomoWin的引导项了，直接返回FALSE */
		MessageBox(hwnd, "Your OS looks like has some errors, lost the boot.ini file.", "qomoldr.mbr", MB_OK|MB_ICONWARNING);
		return FALSE;
	}
}

/*
vi:ts=4:wrap:ai:
*/
