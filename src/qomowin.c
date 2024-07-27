/* vi: set sw=4 ts=4: */
/*
 * qomowin.c: This file is part of ____
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

/* This is for ATTACH_PARENT_PROCESS */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x501
#endif

#include <windows.h>
#include <dbt.h>
#include <shlwapi.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "resource.h"
#include "qomowin.h"
#include "utils.h"
#include "qw-debug.h"

#define MAX_LOADSTRING	100

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static BOOL InitApplication(HINSTANCE hInstance);
static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
static void CreateControls(HWND hwnd);
static void SelectISOFile(HWND hwnd);
static void InitControls(HWND hwnd);
static void RefreshUSBList(HWND hwnd);
static void InstallMbr(HWND hwnd);
static void CenterWindow(HWND hwnd);

static void SelectISOFile(HWND hwnd)
{
	TCHAR 	szText[MAX_LOADSTRING];
	TCHAR	szFile[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	LoadString(hLangDll, IDS_OFN_TITLE, szText, MAX_LOADSTRING);

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = _T('\0');
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("ISO File(*.iso)\0*.iso\0All File(*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = szText;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// show the selection dialog
	if (GetOpenFileName(&ofn) )
	{
		SetWindowText(GetDlgItem(hwnd, IDC_FILE_PATH), szFile);
	}
}

static void CenterWindow(HWND hwnd)
{
	int scrWidth,scrHeight;
	int w,h;
	RECT rect;
	//获得屏幕尺寸
	scrWidth=GetSystemMetrics(SM_CXSCREEN);
	scrHeight=GetSystemMetrics(SM_CYSCREEN);
	//获取窗体尺寸
	GetWindowRect(hwnd, &rect);
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
	rect.left=(scrWidth - w)/2;
	rect.top=(scrHeight - h)/2;
	//设置窗体位置
	SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, w, h, SWP_SHOWWINDOW);
}

static void CreateControls(HWND hwnd)
{
	int x, w;
	RECT rect;
	HDC hdc;
	TEXTMETRIC tm;
	DWORD dwCharW;
	HFONT hFont;
	LOGFONT   logfont;
	HWND hCtrl;
	TCHAR 	szText[MAX_LOADSTRING];
	TCHAR 	szFont[MAX_LOADSTRING];
	HINSTANCE hInstance;
	hInstance = GetModuleHandle(NULL);

	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);
	GetTextMetrics(hdc, &tm);
	dwCharW = 6;

	LoadString(hLangDll, IDS_APP_FONT, szFont, MAX_LOADSTRING);

	lstrcpy((LPSTR) logfont.lfFaceName, (LPSTR)szFont);
	logfont.lfWeight=FW_NORMAL;
	logfont.lfWidth=dwCharW;
	logfont.lfHeight=12;
	logfont.lfEscapement=0;
	logfont.lfUnderline=FALSE;
	logfont.lfItalic=FALSE;
	logfont.lfStrikeOut=FALSE;
	logfont.lfCharSet=DEFAULT_CHARSET;

	hFont = CreateFontIndirect(&logfont);
	SelectObject(hdc, hFont);
	ReleaseDC(hwnd, hdc);

	LoadString(hLangDll, IDS_APP_SUMMARY, szText, MAX_LOADSTRING);
	x = (rect.right - rect.left - strlen(szText) * dwCharW) / 2;
	w = tm.tmAveCharWidth * (strlen(szText) + 4);
	hCtrl = CreateWindow("STATIC", szText, WS_CHILD | WS_VISIBLE | SS_SIMPLE, x, 25, w, 20, hwnd, (HMENU)234, hInstance, NULL);

	LoadString(hLangDll, IDS_APP_FRAME, szText, MAX_LOADSTRING);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|BS_GROUPBOX|WS_GROUP | WS_TABSTOP,50,55,395,90,hwnd,(HMENU)IDC_STATIC,hInstance,NULL);                           
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_HD_INST, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON | WS_TABSTOP, 75,80,w,20, hwnd,(HMENU) IDC_HD_INST , hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_USB_INST, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON | WS_TABSTOP, 75, 110, w, 20, hwnd,(HMENU) IDC_USB_INST, hInstance,   NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	hCtrl = CreateWindow("COMBOBOX", NULL, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_BORDER, 75 + w,110, 60, 150, hwnd,(HMENU)IDC_USB_LIST, hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_ISO_FILE, szText, MAX_LOADSTRING);
	w = dwCharW * strlen(szText);
	hCtrl = CreateWindow("STATIC", szText, WS_CHILD | WS_VISIBLE |SS_LEFT | WS_TABSTOP, 50, 157, w, 20, hwnd, (HMENU)IDC_STATIC, hInstance, NULL);                          
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	hCtrl = CreateWindow("EDIT",NULL,WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOVSCROLL|ES_LEFT|ES_AUTOHSCROLL | WS_TABSTOP, 122, 155, 270, 20, hwnd,(HMENU)IDC_FILE_PATH, hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_BROWSER, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE, 395, 155, w, 26, hwnd, (HMENU)IDC_BROWSER, hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	/* 画水平线 */

	LoadString(hLangDll, IDS_APP_HOMEPAGE, szText, MAX_LOADSTRING);
	w = dwCharW * strlen(szText);
	hCtrl = CreateWindow("STATIC", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE, 50, rect.bottom-63, w, 20, hwnd, (HMENU)IDC_STATIC, hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	sprintf(szText, " http://www.linux-ren.org");
	hCtrl = CreateWindow("STATIC", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE, 50 + w, rect.bottom-63, dwCharW * (strlen(szText) + 4), 20, hwnd, (HMENU)IDC_HOMEPAGE, hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_QUIT, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE, rect.right-85, rect.bottom-63, w, 26, hwnd, (HMENU)IDC_QUIT, hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_CONFIRM, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE, rect.right-160, rect.bottom-63, w, 26, hwnd, (HMENU)IDC_CONFIRM, hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	/* create status bar */
	hCtrl = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);

	/* create progress bar */
	GetWindowRect(hCtrl, &rect);
	hCtrl = CreateWindowEx(0, PROGRESS_CLASS, (LPSTR)NULL, WS_CHILD | WS_VISIBLE, 1, 50, rect.right - rect.left, rect.bottom - rect.top, hwnd, (HMENU)IDC_MAIN_PROGRESS, hInstance, NULL);
	SendMessage(hCtrl, PBM_SETRANGE, 0, MAKELPARAM (0,100));
	SendMessage(hCtrl, PBM_SETPOS, (WPARAM)33, 0);

}

static void RefreshUSBList(HWND hwnd)
{
	int i, count;
	UINT type;
	char driver[3] = "C:";
	count = SendMessage(GetDlgItem(hwnd, IDC_USB_LIST), CB_GETCOUNT, 0, 0);

	for(i=0; i<count; i++)
	{
		SendMessage(GetDlgItem(hwnd, IDC_USB_LIST), CB_DELETESTRING, i, 0);
	}

	for (i = 0; i < 24; i++)
	{
		driver[0] = 'C' + i;
		type = GetDriveType(driver);
		if ( type == DRIVE_REMOVABLE)
			SendMessage(GetDlgItem(hwnd, IDC_USB_LIST), CB_ADDSTRING, 0, (LPARAM)driver) ;
	}

	count = SendMessage(GetDlgItem(hwnd, IDC_USB_LIST), CB_GETCOUNT, 0, 0);
	if (count >0)
		SendMessage(GetDlgItem(hwnd, IDC_USB_LIST), CB_SETCURSEL, 0, 0);

	InvalidateRect(GetDlgItem(hwnd, IDC_USB_LIST), NULL, TRUE);
}

static void InitControls(HWND hwnd)
{
	SendMessage(GetDlgItem(hwnd, IDC_HD_INST), BM_SETCHECK, BST_CHECKED, 0);
	RefreshUSBList(hwnd);
}

static BOOL CheckISOFile(HWND hwnd)
{
	char szFile[MAX_PATH];
	char MsgInfo[BUFSIZ];
	int len;

	LoadString(hLangDll, IDS_MSG_NOT_ISO, MsgInfo, sizeof(MsgInfo));
	SendMessage(GetDlgItem(hwnd, IDC_FILE_PATH), WM_GETTEXT, MAX_PATH, (LPARAM)szFile);
	if ((len = strlen(szFile)) < 6)
	{
		msg_error(hwnd, MsgInfo);
		return FALSE;
	}
	else if (szFile[1] != ':' || szFile[2] != '\\')
	{
		msg_error(hwnd, MsgInfo);
		return FALSE;
	}
	else
	{
		char *p = szFile + strlen(szFile) - 4 ;
		if (strncmp(p, ".iso", 4) != 0 && strncmp(p, ".ISO", 4) != 0)
		{
			msg_error(hwnd, MsgInfo);
			return FALSE;
		}
		else
		{
			if (! PathFileExists(szFile))
			{
				LoadString(hLangDll, IDS_MSG_FILE_NOT_EXISTS, MsgInfo, sizeof(MsgInfo));
				msg_error(hwnd, MsgInfo);
				return FALSE;
			}
		}
	}
	return TRUE;
}

static void InstallMbr(HWND hwnd)
{
	OSVERSIONINFO   versionInfo;
	versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	//   获得版本信息
	if (!GetVersionEx (&versionInfo))
	{
		char MsgInfo[BUFSIZ];
		LoadString(hLangDll, IDS_MSG_GET_VER_ERR, MsgInfo, sizeof(MsgInfo));
		msg_error(hwnd, MsgInfo);
		return;
	}

	/*
	 *	OS VERSION TABLE
	 *  OSName  | dwPlatformID | dwMajorVersion | dwMinorVersion | dwBuildNumber
	 *  95      | 1 | 4 | 0    | 950
	 *  95 SP1  | 1 | 4 | 0    | > 950 && <= 1080
	 *  95 OSR2 | 1 | 4 | < 10 | > 1080
	 *  98      | 1 | 4 | 10   | 1998
	 *  98 SP1  | 1 | 4 | 10   | >1998 && < 2183
	 *  98 SE   | 1 | 4 | 10   | >= 2183
	 *  Me      | 1 | 4 | 90   | 3000
	 *  NT 3.51 | 2 | 3 | 51   | 1057
	 *  NT 4    | 2 | 4 | 0    | 1381
	 *  2000    | 2 | 5 | 0    | 2195
	 *  XP      | 2 | 5 | 1    |
	 *  2003    | 2 | 5 | 2    |
	 *  CE 1.0  | 3 | 1 | 0    |
	 *  CE 2.0  | 3 | 2 | 0    |
	 *  CE 2.1  | 3 | 2 | 1    |
	 *  CE 3.0  | 3 | 3 | 0    |
	 *  Vista   |   | 6 | 0    |
	 *  Win7    |   | 6 | 1    |
	 *  */
	if (versionInfo.dwMajorVersion == 5) /* Support 2000, XP, 2003 */
	{
		CheckNtLdr(hwnd);
	}
	else if (versionInfo.dwMajorVersion == 6) /* Support Vista, Win7 */
	{
		CheckBootMgr(hwnd);
	}
}

static void OnConfirm(HWND hwnd)
{
	char MsgInfo[BUFSIZ];
	if (CheckISOFile(hwnd) != TRUE)
	{
		return;
	}

	if ( BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_HD_INST), BM_GETCHECK, 0, 0))
	{
		if (ExtractKernel(hwnd) != TRUE)
		{
			LoadString(hLangDll, IDS_EXTRACT_KERNEL_E, MsgInfo, sizeof(MsgInfo));
			msg_error(hwnd, MsgInfo);
			return;
		}
		if (UpdateGrubCfg(hwnd) != TRUE)
		{
			LoadString(hLangDll, IDS_UPDATE_GRUBCFG_E, MsgInfo, sizeof(MsgInfo));
			msg_error(hwnd, MsgInfo);
			return;
		}
		InstallMbr(hwnd);
		LoadString(hLangDll, IDS_REBOOT_NOW, MsgInfo, sizeof(MsgInfo));
		if (msg_yesno(hwnd, MsgInfo)  == IDYES)
			ReBoot(1);
	}
	else if ( BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_USB_INST), BM_GETCHECK, 0, 0))
	{
		int len;
		char driver[10] = {0};

		len = GetWindowText(GetDlgItem(hwnd, IDC_USB_LIST), driver, 8);
		if (len == 0)
		{
			LoadString(hLangDll, IDS_INSERT_USB_DISK, MsgInfo, sizeof(MsgInfo));
			msg_error(hwnd, MsgInfo);
			return;
		}
		FormatDriver(hwnd, driver, "QOMOLIVEUSB");
		ISO2USB(hwnd, driver, "QOMOLIVEUSB");
	}
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
			CenterWindow(hwnd);
			CreateControls(hwnd);
			InitControls(hwnd);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_CONFIRM:
					OnConfirm(hwnd);
					break;
				case IDC_BROWSER:
					SelectISOFile(hwnd);
					break;
				case IDC_QUIT:
					DestroyWindow(hwnd);
					break;
				default:
					return (DefWindowProc(hwnd, msg, wParam, lParam));
			}
			break;
		case WM_DEVICECHANGE:
			{
				RefreshUSBList(hwnd);
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

static BOOL InitApplication(HINSTANCE hInstance)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

	wc.lpszMenuName = NULL;
	wc.lpszClassName = "QomoWinClass";
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));

	return (RegisterClassEx(&wc) != 0);
}

static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND		hWndMain;
	TCHAR 		szTitle[MAX_LOADSTRING];				// The title bar text

	LoadString(hLangDll, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

	hWndMain = CreateWindowEx(
			0,
			"QomoWinClass",
			szTitle,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,
			NULL, NULL, hInstance, NULL);
	if (hWndMain == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);

	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG Msg;
	const char *locale;

	locale = winpidgin_get_locale();

	hLangDll = LoadLangDll(locale);
	if( hLangDll == NULL ) hLangDll = hInstance;
	GetPrivileges();

	if (!InitApplication(hInstance))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	if (!InitInstance(hInstance, nCmdShow))
	{
		MessageBox(NULL, "Init Window Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
