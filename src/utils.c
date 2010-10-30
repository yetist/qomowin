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
#include <winioctl.h>
#include <unistd.h>
#include <ctype.h>

#include "qomowin.h"
#include "resource.h"
#include "utils.h"
#include "qw-debug.h"

static char* winpidgin_lcid_to_posix(LCID lcid);
static BOOL CopyMbrFiles(HWND hwnd, const char* sysDriver);
static BOOL getIsoPath(HWND hwnd, char* isopath);
static BOOL getIsoDev(HWND hwnd, char* isodev);
static BOOL GetDriveInfo(const char* Driver, int* nDisk, int* nPart);
static BOOL ExecCmd(HWND hwnd, const char* cmd);
static BOOL ExecCmdOut(HWND hwnd, const char* cmd, char* buf);
static BOOL getRegKey(const char* key, char* value, size_t size);
static BOOL writeRegKey(HWND hwnd, const char* key, const char* value);

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
	char 		DllPath[MAX_PATH] = {0};
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

int ReBoot(int reboot)
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
	TCHAR sysMsg[BUFSIZ] = {0};
	TCHAR showmsg[BUFSIZ] = {0};
	TCHAR* p;

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
	snprintf(showmsg, sizeof(showmsg), "%s\n[ErrorID=%ld]%s", msg, eNum, sysMsg);
	SendMessage(GetDlgItem(hwnd, IDC_MAIN_STATUS), SB_SETTEXT, 0, showmsg);
	msg_error(hwnd, showmsg);
}

/**
 * getExeDir:
 * @path: 将被填充为路径
 *
 * 得到可执行文件Qomowin.exe的绝对路径
 *
 * */
BOOL getExeDir(char* path)
{
	char *p, modFileName[MAX_PATH] = {0};

	HINSTANCE hInstance = GetModuleHandle(NULL);
	if (hInstance == NULL)
		return FALSE;
	if (GetModuleFileName(hInstance, modFileName, MAX_PATH) != 0)
	{
		p = strrchr(modFileName, '\\');
		*p = '\0';
		strcpy(path, modFileName);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static BOOL CopyMbrFiles(HWND hwnd, const char* sysDriver)
{
	/* sysDriver = "X:\" */
	char srcFileName[MAX_PATH] = {0}, dstFileName[MAX_PATH] = {0};
	DWORD dwAttrs;

	if (!getExeDir(srcFileName))
	{
		PrintError(hwnd, "get Current dir error!");
		return FALSE;
	}

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

static BOOL ExecCmd(HWND hwnd, const char* cmd)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
	int ret;
	DWORD exitcode;

	ZeroMemory(&si,sizeof(si));
	ZeroMemory(&pi,sizeof(pi));
    si.cb = sizeof(STARTUPINFO);
    GetStartupInfo(&si);
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

    if (!CreateProcess(NULL, (LPSTR)cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) 
	{
		MessageBox(hwnd, cmd, "here", MB_OK|MB_ICONWARNING);
        return FALSE;
    }

	do
	{
		// 等待结束
		WaitForSingleObject(pi.hProcess,0);
		ret = GetExitCodeProcess (pi.hProcess, &exitcode);
	} while (ret && exitcode == STILL_ACTIVE);
	TerminateProcess(pi.hProcess, 0);
	return TRUE;
}

BOOL ExtractKernel(HWND hwnd)
{
	char szFile[MAX_PATH] = {0};
	char cwd[MAX_PATH] = {0};
	char cmd[BUFSIZ] = {0};

	/* 7z 提取命令：
	 * 7z e -o/tmp/ ~/Qomo1.iso isolinux/initrd0.img
	 * 将~/Qomo1.iso中的文件isolinux/initrd0.img解压到/tmp目录中
	 * */

	if (getExeDir(cwd))
	{
		char arg[MAX_PATH] = {0};

		SendMessage(GetDlgItem(hwnd, IDC_FILE_PATH), WM_GETTEXT, MAX_PATH, (LPARAM)szFile);

		snprintf(cmd, sizeof(cmd), "%s\\bin\\7z.exe e -y \"-o%s\\boot\" \"%s\" ", cwd, cwd, szFile);
		strcpy(cwd, cmd); //save cmd line into cwd;

		snprintf(arg, sizeof(arg), "%s\\qomowin.ini", cwd);
		GetPrivateProfileString("iso9660", "kernel", "isolinux/vmlinuz0", szFile, MAX_PATH, arg);
		strcat(cmd, szFile);  //cmd="command.com /c .\bin\7z.exe e -o boot ~/Qomo.iso isolinux/initrd0.img"

		if (!ExecCmd(hwnd, cmd))
		{
			PrintError(hwnd, "extract kernel from iso failed.");
			return FALSE;
		}

		GetPrivateProfileString("iso9660", "initrd", "isolinux/initrd0.img", szFile, MAX_PATH, arg);
		strcpy(cmd, cwd); //restore cmd line from cwd.
		strcat(cmd, szFile);  //cmd="command.com /c .\bin\7z.exe e -o boot ~/Qomo.iso isolinux/initrd0.img"

		if (!ExecCmd(hwnd, cmd))
		{
			PrintError(hwnd, "extract initrd from iso failed.");
			return FALSE;
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL CheckNtLdr(HWND hwnd)
{
	char bootIni[BUFSIZ] = {0}, *p;
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
		char  keys[BUFSIZ] = {0};
		char *p;

		readCount= GetPrivateProfileSection("operating systems", keys, BUFSIZ, bootIni);
		p = keys; 	/* keys="name1=value1\0name2=value2\0name3=value3\0...\0\0" */
		while (p < (keys + readCount))
		{
			if (strstr(p, "\\qomoldr.mbr") != NULL)
			{
				isInstalled = 1;
				break;
			}
			p = p + strlen(p) + 1;
		}

		if (!isInstalled) /* 在 X:/boot.ini 文件中未发现qomoldr.mbr引导项 */
		{
			char key[20] = {0};
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

//   Driver- 盘符，比如'C '
//   nDisk - 返回哪个硬盘，序号从1开始
//   nPart - 返回哪个分区，序号从1开始
static BOOL   GetDriveInfo(const char* Driver, int* nDisk, int* nPart)
{
	BOOL   bRet;
	char   buf[33] = {0};
	DWORD   dwIOCode;
	DWORD   dwReturn;
	HANDLE   hDevice;
	VOLUME_DISK_EXTENTS   DiskExts;
	PARTITION_INFORMATION   ParInfo;

	wsprintf(buf,   "\\\\.\\%s ",   Driver);
	hDevice = CreateFile(buf,GENERIC_READ,
			FILE_SHARE_WRITE, 0, OPEN_EXISTING,
			0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
		return FALSE;
	dwIOCode = IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS;
	bRet = DeviceIoControl(hDevice,dwIOCode,NULL,0,
			&DiskExts,sizeof(DiskExts),&dwReturn,NULL);
	if(!bRet || DiskExts.NumberOfDiskExtents==0)
	{
		CloseHandle(hDevice);
		return FALSE;
	}

	*nDisk = DiskExts.Extents[0].DiskNumber + 1;
	dwIOCode = IOCTL_DISK_GET_PARTITION_INFO;
	bRet = DeviceIoControl(hDevice,dwIOCode,NULL,0,
			&ParInfo,sizeof(ParInfo),&dwReturn,NULL);
	if(bRet)
		*nPart = ParInfo.PartitionNumber;
	CloseHandle(hDevice);
   return bRet;
} 

static BOOL getIsoDev(HWND hwnd, char* isodev)
{
	char szFile[MAX_PATH] = {0};
	int ndisk, npart;

	SendMessage(GetDlgItem(hwnd, IDC_FILE_PATH), WM_GETTEXT, MAX_PATH, (LPARAM)szFile);
	szFile[2] = '\0';
	if (GetDriveInfo(szFile, &ndisk, &npart) == TRUE)
	{
		sprintf(szFile, "/dev/sd%c%d", 'a' + ndisk -1, npart);
		strcpy(isodev, szFile);
		return TRUE;
	}
	return FALSE;
}

static BOOL getIsoPath(HWND hwnd, char* isopath)
{
	char szFile[MAX_PATH] = {0};
	char *p, *p1;
	SendMessage(GetDlgItem(hwnd, IDC_FILE_PATH), WM_GETTEXT, MAX_PATH, (LPARAM)szFile);
	memmove(szFile, szFile + 2, strlen(szFile) - 2); /* 去掉开头的盘符"X:" */

	/* 去掉末尾的文件名，只保留路径部分 */
	p = strrchr(szFile, '\\');
	*++p = '\0';

	/* 转换Windows路径为Unix路径 */
	p = szFile;
	p1 = isopath;

	while(p && (*p))
	{
		if (*p == '\\')
			*p = '/';
		if (*p == ' ')
			*p1++ = '\\';
		*p1++ = *p++;
	}
	return TRUE;
}

BOOL UpdateGrubCfg(HWND hwnd)
{
	FILE *fp, *fp2;
	char cwd[MAX_PATH] = {0};
	char sztmp[MAX_PATH] = {0};
	char arg[MAX_PATH] = {0};
	char txt[BUFSIZ] = {0};
	const char *locale = NULL;
	LCID lcid;
	size_t nread;

	if (!getExeDir(cwd))
		return FALSE;
	snprintf(sztmp, sizeof(sztmp), "%s\\qomowin.ini", cwd);
	GetPrivateProfileString("default", "title", "Qomo Linux LiveCD Installer", arg, MAX_PATH, sztmp);
	strcat(txt, "set title=\""); strcat(txt, arg); strcat(txt, "\"\n");

	GetPrivateProfileString("default", "kernel", "/boot/vmlinuz0", arg, MAX_PATH, sztmp);
	strcat(txt, "set kernel=\""); strcat(txt, arg); strcat(txt, "\"\n");

	GetPrivateProfileString("default", "initrd", "/boot/initrd0", arg, MAX_PATH, sztmp);
	strcat(txt, "set initrd=\""); strcat(txt, arg); strcat(txt, "\"\n");

	GetPrivateProfileString("default", "rootflags", "liveimg nodmraid rdblacklist=b44 rdblacklist=b43 rdblacklist=ssb", arg, MAX_PATH, sztmp);
	strcat(txt, "set rootflags=\""); strcat(txt, arg); strcat(txt, "\"\n");

	if (getIsoDev(hwnd, arg) != TRUE)
		return FALSE;
	strcat(txt, "set iso_dev=\""); strcat(txt, arg); strcat(txt, "\"\n");
	memset(arg, '\0', sizeof(arg));
	if (getIsoPath(hwnd, arg) != TRUE)
		return FALSE;
	strcat(txt, "set iso_path=\""); strcat(txt, arg); strcat(txt, "\"\n");

	lcid = GetUserDefaultLCID();
	locale = winpidgin_lcid_to_posix(lcid);
	strcat(txt, "set grublang=\""); strcat(txt, locale); strcat(txt, "\"\n");

	snprintf(sztmp, sizeof(sztmp), "%s\\boot\\grub\\grub.cfg", cwd); 
	if ((fp = fopen(sztmp, "w+")) == NULL)
	{
		return FALSE;
	}
	fwrite(txt, strlen(txt), 1, fp);

	snprintf(sztmp, sizeof(sztmp), "%s\\grub.cfg.in", cwd);
	if ((fp2 = fopen(sztmp, "r")) == NULL)
	{
		return FALSE;
	}

	while((nread=fread(txt, sizeof(char), BUFSIZ, fp2)) > 0)
		fwrite(txt, sizeof(char), nread,fp);
	fclose(fp2);
	fclose(fp);
	return TRUE;
}

static BOOL ExecCmdOut(HWND hwnd, const char* cmd, char* buf)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;
	DWORD bytesRead;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char buffer[4096] = {0};

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead,&hWrite,&sa,0)) {
		MessageBox(hwnd, "Error on CreatePipe()", "hello", MB_OK|MB_ICONWARNING);
		return FALSE;
	}

	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	if (!CreateProcess(NULL, (LPSTR)cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) 
	{
		return FALSE;
	}
	CloseHandle(hWrite);

	while(TRUE)
	{
		if (!ReadFile(hRead, buffer, 4095, &bytesRead, NULL))
			break;
		if (buf != NULL)
			strncat(buf, buffer, bytesRead);
		Sleep(200);
	}
	return TRUE;
}

static BOOL getRegKey(const char* key, char* value, size_t size)
{
	HKEY hKey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\qomowin"),
				0,
				KEY_READ|KEY_WRITE,
				&hKey
				)== ERROR_SUCCESS)
	{
		int iret;
		DWORD dwType = REG_SZ;
		iret = RegQueryValueEx(hKey, (LPSTR)key, NULL, &dwType, (LPBYTE)value, (LPDWORD) &size);
		RegCloseKey(hKey);

		if(iret == 0)
		{
			value[size] = 0;
			return TRUE;
		}
	}
	return FALSE;
}

static BOOL writeRegKey(HWND hwnd, const char* key, const char* value)
{
	HKEY hKey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\qomowin"),
				0,
				KEY_WRITE,
				&hKey
				)== ERROR_SUCCESS)
	{
		long lRet;
		lRet = RegSetValueEx(hKey, key, 0, REG_SZ, (BYTE *)value, strlen(value));
		RegCloseKey(hKey);
		if(lRet != ERROR_SUCCESS)
		{
			PrintError(hwnd, "write register failed!");
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;

}

BOOL CheckBootMgr(HWND hwnd)
{
	char sysdriver[BUFSIZ] = {0}, *p = NULL;
	char buf[1024] = {0};
	char id[64] = {0};
	int isInstalled = 0;

	if (getRegKey("VistaBootDrive", (char*) buf, sizeof(buf)))
	{
		if (strlen(buf) > 0)
		{
			isInstalled = 1;
		}
	}

	if (!isInstalled)  /* 检查 bootMgr是否已经安装 */
	{
		ExecCmdOut(hwnd, "bcdedit /create /d \"Qomo Linux\" /application bootsector", buf);
		if ((p = strrchr(buf, '}')) == NULL)
		{
			MessageBox(hwnd, buf, "create menu entry error", MB_OK | MB_ICONWARNING);
			return FALSE;
		}
		*++p = '\0';
		if ((p = strchr(buf, '{')) == NULL)
		{
			MessageBox(hwnd, buf, "create menu entry error", MB_OK | MB_ICONWARNING);
			return FALSE;
		}
		strcpy(id, p);

		/* 得到系统盘根目录的 X:\ */
		if(!GetSystemDirectory(sysdriver, BUFSIZ))
		{
			PrintError(hwnd, "GetSystemDirectory Failed!"); 
			return FALSE;
		}
		p = sysdriver + 2;
		memset(p, '\0', BUFSIZ - 2);

		snprintf(buf, 1024, "bcdedit /set %s device partition=%s", id, sysdriver);
		if (!ExecCmd(hwnd, buf))
			return FALSE;

		snprintf(buf, 1024, "bcdedit /set %s path \\qomoldr.mbr", id);
		if (!ExecCmd(hwnd, buf))
			return FALSE;

		snprintf(buf, 1024, "bcdedit /displayorder %s /addlast", id);
		if (!ExecCmd(hwnd, buf))
			return FALSE;
		writeRegKey(hwnd, "VistaBootDrive", id);

		if (!CopyMbrFiles(hwnd, sysdriver))
			return FALSE;
	}
	return TRUE;
}

BOOL FormatDriver(HWND hwnd, const char* driver, const char* label)
{
	int driver_number;
	char volume[12] = {0};
	DWORD ret;

	driver_number = toupper(driver[0]) -'A';
	if (driver_number <0 || driver_number > 25)
		return FALSE;
	ret = SHFormatDrive(hwnd, driver_number, SHFMT_ID_DEFAULT, 0);
	if (ret == -1)
	{
		if (msg_yesno(hwnd, "Format failed, continue?") == IDNO)
			return FALSE;
	}
	snprintf(volume, sizeof(volume), label);
	if (!SetVolumeLabel(driver, volume))
	{
		msg_info(hwnd, "Setup Volume Label Failed.");
		return FALSE;
	}
	return TRUE;
}

BOOL ISO2USB(HWND hwnd, const char* driver, const char* label)
{
	char cwd[MAX_PATH] = {0};

	/* 7z 解压命令：
	 * 7z x -y -o/tmp/ ~/Qomo1.iso
	 * 将~/Qomo1.iso中的文件全部解压到/tmp目录中
	 * */

	if (getExeDir(cwd))
	{
		int ret;
		char szFile[MAX_PATH] = {0};
		char cmd[BUFSIZ] = {0};
		//char tmppath[255] = {0};
		char line[BUFSIZ];
		FILE *fp1;
		FILE *fp2;

		/* 解压 ISO到临时文件 */
		//GetTempPath(255, tmppath);
		//strcat(tmppath, "qomoliveusb");
		SendMessage(GetDlgItem(hwnd, IDC_FILE_PATH), WM_GETTEXT, MAX_PATH, (LPARAM)szFile);
		snprintf(cmd, sizeof(cmd), "%s\\bin\\7z.exe x -y \"-o%s\" \"%s\" ", cwd, driver, szFile);
		debug_msg(hwnd, "%s", cmd);
		if (!ExecCmd(hwnd, cmd))
		{
			PrintError(hwnd, "extract initrd from iso failed.");
			return FALSE;
		}

		/* 重命名isolinux到syslinux */
		snprintf(szFile, sizeof(szFile), "%s\\isolinux", driver);
		snprintf(cmd, sizeof(cmd), "%s\\syslinux", driver);
		if (rename(szFile, cmd) != 0)
		{
			msg_error(hwnd, "Rename %s to %s failed", szFile, cmd);
			return FALSE;
		}

		/* 重命名isolinux.cfg到syslinux.cfg,并修改root参数 */
		snprintf(szFile, sizeof(szFile), "%s\\syslinux\\isolinux.cfg", driver);
		snprintf(cmd, sizeof(cmd), "%s\\syslinux\\syslinux.cfg", driver);
		fp1 = fopen(szFile, "r");
		fp2 = fopen(cmd, "w");
		while (fgets(line, sizeof(line), fp1) != NULL)
		{
			char *p;
			if ((p = strstr(line, "root=live")) != NULL)
			{
				char *p2;
				char newline[BUFSIZ];
				p2 = strchr(p, ' ');
				*p='\0';
				snprintf(newline, sizeof(newline), "%sroot=live:LABEL=%s%s", line, label, p2);
				debug_msg(hwnd, "p=%s, p2=%s", p, p2);
				debug_msg(hwnd, newline);
				if (fputs(newline, fp2) == EOF)
				{
					msg_error(hwnd, "Write file %s error!", cmd);
					fclose(fp1);
					fclose(fp2);
					return FALSE;
				}
			} else {
				if (fputs(line, fp2) == EOF)
				{
					msg_error(hwnd, "Write file %s error!", cmd);
					fclose(fp1);
					fclose(fp2);
					return FALSE;
				}
			}
		}
		fclose(fp1);
		fclose(fp2);

		/* 安装syslinux 的引导项 */
		snprintf(cmd, sizeof(cmd), "%s\\bin\\syslinux.exe -sfmar -d \\syslinux %s", cwd, driver);
		debug_msg(hwnd, cmd);
		if (!ExecCmd(hwnd, cmd))
		{
			PrintError(hwnd, "create boot loader failed.");
			return FALSE;
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

/*
vi:ts=4:wrap:ai:
*/
