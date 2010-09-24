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
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "resource.h" 
#include "qomowin.h" 
#include "utils.h" 

#define MAX_LOADSTRING	100	

HMODULE		hLangDll;		// Handle for multi language

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static BOOL InitApplication(HINSTANCE hInstance);
static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
static void CreateWindowUI(HWND hwnd);
static void SelectISOFile(HWND hwnd);

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
		SetWindowText(GetDlgItem(hwnd, IDE_FILE_PATH), szFile);
	}
}

static void CreateWindowUI(HWND hwnd)
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
	HINSTANCE hInstance;
	hInstance = GetModuleHandle(NULL);

	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);
	GetTextMetrics(hdc, &tm);
	dwCharW = 6;

	lstrcpy((LPSTR) logfont.lfFaceName, (LPSTR)"SimSun Sans Serif");
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
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|BS_GROUPBOX|WS_GROUP | WS_TABSTOP,50,55,395,120,hwnd,(HMENU)IDC_STATIC,hInstance,NULL);                           
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_HD_INST, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON | WS_TABSTOP, 75,80,w,20, hwnd,(HMENU) IDM_HD_INST , hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_HD_UNINST, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON | WS_TABSTOP, 75,110,w,20,hwnd,(HMENU) IDM_HD_UNINST, hInstance,   NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_USB_INST, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON | WS_TABSTOP, 75, 140, w, 20, hwnd,(HMENU) IDM_USB_INST, hInstance,   NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_ISO_FILE, szText, MAX_LOADSTRING);
	w = dwCharW * strlen(szText);
	hCtrl = CreateWindow("STATIC", szText, WS_CHILD | WS_VISIBLE |SS_LEFT | WS_TABSTOP, 50, 187, w, 120, hwnd, (HMENU)IDC_STATIC, hInstance, NULL);                          
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	hCtrl = CreateWindow("EDIT",NULL,WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOVSCROLL|ES_LEFT|ES_AUTOHSCROLL | WS_TABSTOP, 122, 185, 270, 20, hwnd,(HMENU)IDE_FILE_PATH,hInstance,NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_BROWSER, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE, 395,185, w,26, hwnd, (HMENU)IDM_BROWSER, hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_QUIT, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE, rect.right-85, rect.bottom-33, w, 26, hwnd, (HMENU)IDM_QUIT, hInstance, NULL);   
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);

	LoadString(hLangDll, IDS_APP_CONFIRM, szText, MAX_LOADSTRING);
	w = dwCharW * (strlen(szText) + 4);
	hCtrl = CreateWindow("BUTTON", szText, WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE, rect.right-160, rect.bottom-33, w, 26, hwnd, (HMENU)IDM_CONFIRM, hInstance, NULL);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, 0);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
			CreateWindowUI(hwnd);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDM_HD_INST:
				case IDM_HD_UNINST:
				case IDM_USB_INST:
				case IDM_CONFIRM:
					MessageBox(hwnd, "confirm", "This program is", MB_OK | MB_ICONINFORMATION);
					break;
				case IDM_BROWSER:
					SelectISOFile(hwnd);
					break;
				case IDM_QUIT:
					DestroyWindow(hwnd);
					Boot(1);
					break;
				default:
					return (DefWindowProc(hwnd, msg, wParam, lParam));
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
	TCHAR 		g_szClassName[MAX_LOADSTRING];			// The title bar text

	LoadString(hLangDll, IDS_APP_CLASS, g_szClassName, MAX_LOADSTRING);

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
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));

	return (RegisterClassEx(&wc) != 0);
}

static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND		hWndMain;
	TCHAR 		szTitle[MAX_LOADSTRING];				// The title bar text
	TCHAR 		g_szClassName[MAX_LOADSTRING];			// The title bar text

	LoadString(hLangDll, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hLangDll, IDS_APP_CLASS, g_szClassName, MAX_LOADSTRING);

	hWndMain = CreateWindowEx(
			0,
			g_szClassName,
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

	if (! InitApplication(hInstance))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	if (! InitInstance(hInstance, nCmdShow))
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
