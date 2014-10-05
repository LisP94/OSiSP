// OSiSP1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OSiSP1.h"
#include <commdlg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define pi 3.141592654
#define IM_ABOUT 0
#define IM_PENCIL 1
#define IM_LINE 2
#define IM_RECTANGLE 3
#define IM_EXIT 4
#define IM_COLOR 5
#define IM_TEXT 6
#define IM_ELLIPSE 7
#define IM_POLYLINE 8
#define IM_POLYGON 9
#define IM_WIDTH_1 10
#define IM_WIDTH_2 11
#define IM_WIDTH_3 12
#define IM_WIDTH_4 13
#define IM_WIDTH_5 14
#define IM_WIDTH_6 15
#define IM_WIDTH_7 16
#define IM_WIDTH_8 17
#define IM_WIDTH_9 18
#define IM_WIDTH_10 19
#define IM_SAVE 21
#define IM_OPEN 22
#define IM_PRINT 23

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
TCHAR szChildTitle[] = L"Clock";
TCHAR szChildClass[] = L"ClockClass";
HDC hdc1, hdc2;
HWND  GlWnd;
int scrvert, scrhor;
struct TIME
{
	int xs, ys, xm, ym, xh, yh, min, hour;
};
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
ATOM				MyChildRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndclProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL GetPenColor(HWND hwnd, COLORREF *clrref);
void SaveEnhMetaFile(HWND hWnd);
void OpenEnhMetaFile(HWND hWnd);
void Print(HWND hWnd);
void NewHand(int &x, int &y);
void SetTime(TIME &t);
LPACCEL TableAccel();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_OSISP1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	MyChildRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = CreateAcceleratorTable(TableAccel(),4);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	DestroyAcceleratorTable(hAccelTable);
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSISP1));
	wcex.hCursor		= LoadCursor(NULL, IDC_CROSS);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_OSISP1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

ATOM MyChildRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX  wclcex;

	wclcex.cbSize = sizeof(WNDCLASSEX);

	wclcex.style = CS_HREDRAW | CS_VREDRAW;
	wclcex.lpfnWndProc = WndclProc;
	wclcex.cbClsExtra = 0;
	wclcex.cbWndExtra = 0;
	wclcex.hInstance = hInstance;
	wclcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSISP1));
	wclcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wclcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wclcex.lpszMenuName = NULL;
	wclcex.lpszClassName = szChildClass;
	wclcex.hIconSm = LoadIcon(wclcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wclcex);
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd,hWndclock;

   hInst = hInstance; // Store instance handle in our global variable
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   GlWnd = hWnd;
   HMENU hMenu, hSubMenuTools,hSubMenuWidth,hSubMenuFile;
   hMenu = CreateMenu();
   hSubMenuTools = CreatePopupMenu();
   hSubMenuWidth = CreatePopupMenu();
   hSubMenuFile = CreatePopupMenu();

   AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenuFile, L"File");
   AppendMenu(hSubMenuFile, MF_STRING, IM_OPEN, L"Open             Shift+O");
   AppendMenu(hSubMenuFile, MF_STRING, IM_SAVE, L"Save               Shift+S");
   AppendMenu(hSubMenuFile, MF_STRING, IM_PRINT, L"Print               Shift+P");
   AppendMenu(hSubMenuFile, MF_STRING, IM_ABOUT, L"About");
   AppendMenu(hSubMenuFile, MF_STRING, IM_EXIT, L"Exit                 Shift+E");
   AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenuTools, L"Tools");
   AppendMenu(hSubMenuTools, MF_STRING, IM_PENCIL, L"Pencil");
   AppendMenu(hSubMenuTools, MF_STRING, IM_LINE, L"Line");
   AppendMenu(hSubMenuTools, MF_STRING, IM_RECTANGLE, L"Rectangle");
   AppendMenu(hSubMenuTools, MF_STRING, IM_ELLIPSE, L"Ellipse");
   AppendMenu(hSubMenuTools, MF_STRING, IM_POLYLINE, L"Polyline");
   AppendMenu(hSubMenuTools, MF_STRING, IM_POLYGON, L"Polygon");
   AppendMenu(hSubMenuTools, MF_STRING, IM_TEXT, L"Text");
   AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenuWidth, L"Width");
   AppendMenu(hSubMenuWidth, MF_STRING, IM_WIDTH_1, L"1");
   AppendMenu(hSubMenuWidth, MF_STRING, IM_WIDTH_2, L"2");
   AppendMenu(hSubMenuWidth, MF_STRING, IM_WIDTH_3, L"3");
   AppendMenu(hSubMenuWidth, MF_STRING, IM_WIDTH_4, L"4");
   AppendMenu(hSubMenuWidth, MF_STRING, IM_WIDTH_5, L"5");
   AppendMenu(hSubMenuWidth, MF_STRING, IM_WIDTH_6, L"6");
   AppendMenu(hSubMenuWidth, MF_STRING, IM_WIDTH_7, L"7");
   AppendMenu(hSubMenuWidth, MF_STRING, IM_WIDTH_8, L"8");
   AppendMenu(hSubMenuWidth, MF_STRING, IM_WIDTH_9, L"9");
   AppendMenu(hSubMenuWidth, MF_STRING, IM_WIDTH_10, L"10");
   AppendMenu(hMenu, MF_STRING, IM_COLOR, L"Color");
   SetMenu(hWnd, hMenu);
   SetMenu(hWnd, hSubMenuTools);
   SetMenu(hWnd, hSubMenuFile);
   if (!hWnd)
   {
	   return FALSE; 
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   hWndclock = CreateWindow(szChildClass, szChildTitle, WS_CAPTION | WS_CHILD | WS_DISABLED,
	   100, 100, 200, 200, hWnd,NULL, hInstance, NULL); 
   if (!hWndclock)
   {
	   return FALSE;
   }
   ShowWindow(hWndclock, SW_SHOW);
   UpdateWindow(hWndclock);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent,length;
	static char *str;
	static int mouseX,mouseY,menu,width,polX,polY;
	static bool polFl = true, fl = false, text=false,zoom=false;
	static float k=1;
	PAINTSTRUCT ps;
	static HDC hdc;
	HBITMAP hbmp1,hbmp2;
	HGDIOBJ hobj;
	HFONT hFont;
	static RECT r;
	static COLORREF clr;
	static HPEN hpen;
	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		scrhor =3* GetDeviceCaps(hdc, HORZRES);
		scrvert =3* GetDeviceCaps(hdc, VERTRES);
		hdc1 = CreateCompatibleDC(hdc);
		hdc2 = CreateCompatibleDC(hdc);
		hbmp1 = CreateCompatibleBitmap(hdc, scrhor, scrvert);
		hbmp2 = CreateCompatibleBitmap(hdc, scrhor, scrvert);
		SelectObject(hdc1, hbmp1);
		SelectObject(hdc2, hbmp2);
		r.left = 0;
		r.top = 0;
		r.right = scrhor;
		r.bottom = scrvert;
		FillRect(hdc1, &r, (HBRUSH)CreateSolidBrush(RGB(255, 255, 255)));
		FillRect(hdc2, &r, (HBRUSH)CreateSolidBrush(RGB(255, 255, 255)));
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IM_PENCIL:
			menu = IM_PENCIL;
			break;
		case IM_LINE:
			menu = IM_LINE;
			break;
		case IM_TEXT:
			menu = IM_TEXT;
			break;
		case IM_RECTANGLE:
			menu = IM_RECTANGLE;
			break;
		case IM_COLOR:
			GetPenColor(hWnd, &clr);
			hpen = CreatePen(PS_SOLID, 1, clr);
			SelectObject(hdc, hpen);
			SelectObject(hdc1, hpen);
			SelectObject(hdc2, hpen);
			break;
		case IM_POLYLINE:
			menu = 8;
			break;
		case IM_POLYGON:
			menu = 9;
			break;
		case IM_SAVE:
			SaveEnhMetaFile(hWnd);
			break;
		case IM_OPEN:
			OpenEnhMetaFile(hWnd);
			break;
		case IM_PRINT:
			Print(hWnd);
			break;
		case IM_ELLIPSE:
			menu = IM_ELLIPSE;
			break;
		case IM_WIDTH_1:
			width = 1;
			break;
		case IM_WIDTH_2:
			width = 2;
			break;
		case IM_WIDTH_3:
			width = 3;
			break;
		case IM_WIDTH_4:
			width = 4;
			break;
		case IM_WIDTH_5:
			width = 5;
			break;
		case IM_WIDTH_6:
			width = 6;
			break;
		case IM_WIDTH_7:
			width = 7;
			break;
		case IM_WIDTH_8:
			width = 8;
			break;
		case IM_WIDTH_9:
			width = 9;
			break;
		case IM_WIDTH_10:
			width = 10;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		if (width > 0)
		{
			hpen = CreatePen(PS_SOLID, width, clr);
			SelectObject(hdc, hpen);
			SelectObject(hdc1, hpen); 
			SelectObject(hdc2, hpen);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (zoom)
		{
			StretchBlt(hdc2, scrhor/3, scrvert/3, scrhor, scrvert, hdc1, scrhor/3, scrvert/3, scrhor*k, scrvert*k, SRCCOPY);
			StretchBlt(hdc, 0, 0, scrhor, scrvert, hdc1, scrhor/3,scrvert/3, scrhor*k, scrvert*k, SRCCOPY);
		}
		else
		{
			BitBlt(hdc, 0, 0, scrhor, scrvert, hdc1, scrhor/3, scrvert/3, SRCCOPY);
		}
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (menu == IM_TEXT && text)
		{
			length = strlen(str);
			if (wParam == 8)
			{
				str[length - 1] = '\0';
				BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
				TextOutA(hdc1, mouseX+scrhor/3, mouseY+scrvert/3, str, strlen(str));
				BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, scrhor/3, scrvert/3, SRCCOPY);
				InvalidateRect(hWnd, NULL, false);
			}
			else
			{
				if (wParam == 13)
				{
					text = false;
				}
				else
				{
					str = (char*)realloc(str, length + 2);
					length = length + 2;
					str[length - 2] = wParam;
					str[length - 1] = '\0';
					TextOutA(hdc1, mouseX+scrhor/3, mouseY+scrvert/3, str, strlen(str));
					InvalidateRect(hWnd, NULL, false);
				}
			}
		}
		break;
	case WM_KEYUP:
		if (wParam == VK_SHIFT)
		{
			zoom = false;
			k = 1;
			BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
		}
		break;
	case WM_MOUSEWHEEL:
		if (HIWORD(wParam) > WHEEL_DELTA)
		{
			if (LOWORD(wParam)==MK_SHIFT)
			{
				k = k/1.1;
				zoom = true; 
			}
			else
			{
				if (LOWORD(wParam)==MK_CONTROL)
				{
					ScrollDC(hdc1, 5, 0, &r, NULL, NULL, NULL);
					BitBlt(hdc, 0, 0, scrhor, scrvert, hdc1, scrhor/3, scrvert/3, SRCCOPY);
				}
				else
				{
					ScrollDC(hdc1, 0, 5, &r, NULL, NULL, NULL);
					BitBlt(hdc, 0, 0, scrhor, scrvert, hdc1, scrhor/3, scrvert/3, SRCCOPY);
				}
			}
		}
		else
		{
			if (LOWORD(wParam)==MK_SHIFT)
			{
				k = k*1.1;
				zoom = true;
			}
			else
			{
				if (LOWORD(wParam)==MK_CONTROL)
				{
					ScrollDC(hdc1, -5, 0, &r, NULL, NULL, NULL);
					BitBlt(hdc, 0, 0, scrhor, scrvert, hdc1, scrhor/3, scrvert/3, SRCCOPY);
				}
				else
				{
					ScrollDC(hdc1, 0, -5, &r, NULL, NULL, NULL);
					BitBlt(hdc, 0, 0, scrhor, scrvert, hdc1, scrhor/3, scrvert/3, SRCCOPY);
				}
			}
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_LBUTTONDOWN:
		if (menu)
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
			if (menu == IM_POLYGON && polFl)
			{
				polX = mouseX;
				polY = mouseY;
				polFl = false;
			}
			MoveToEx(hdc1, mouseX+scrhor/3, mouseY+scrvert/3, NULL); 
			MoveToEx(hdc2, mouseX+scrhor/3, mouseY+scrvert/3, NULL);
			fl = !fl;
			if (menu == IM_POLYLINE || menu==IM_POLYGON)
			{
				fl = false;
				BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, scrhor/3, scrvert/3, SRCCOPY);
				BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			}
			if (menu == IM_TEXT)
			{
				BitBlt(hdc2, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
				text = true;
				str = (char*)malloc(1);
				str[0] = '\0';
			}
		}
		break;
     case WM_MOUSEMOVE:
			 if (fl)
			 {
				 hdc = GetDC(hWnd);
				 SelectObject(hdc1, hpen);
				 if (menu == IM_PENCIL)
				 {
					 mouseX = LOWORD(lParam);
					 mouseY = HIWORD(lParam);
					 LineTo(hdc1, mouseX+scrhor/3, mouseY+scrvert/3);
					 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc1, scrhor/3, scrvert/3, SRCCOPY);
				 }
				 if (menu == IM_LINE)
				 {
					 BitBlt(hdc2, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
					 MoveToEx(hdc2, mouseX+scrhor/3, mouseY+scrvert/3, NULL);
					 LineTo(hdc2, LOWORD(lParam)+scrhor/3,HIWORD(lParam)+scrvert/3);
					 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, scrhor/3, scrvert/3, SRCCOPY);
				 }
				 if (menu == IM_RECTANGLE)
				 {
					 BitBlt(hdc2, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
					 MoveToEx(hdc2, mouseX+scrhor/3, mouseY+scrhor/3, NULL);
					 hobj=SelectObject(hdc2, GetStockObject(NULL_BRUSH));
					 Rectangle(hdc2, mouseX+scrhor/3, mouseY+scrvert/3, LOWORD(lParam)+scrhor/3, HIWORD(lParam)+scrvert/3);
					 SelectObject(hdc2, hobj);
					 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, scrhor/3, scrvert/3, SRCCOPY);
				 }
				 if (menu == IM_ELLIPSE)
				 {
					 BitBlt(hdc2, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
					 MoveToEx(hdc2, mouseX, mouseY, NULL); 
					 hobj=SelectObject(hdc2, GetStockObject(NULL_BRUSH));
					 Ellipse(hdc2, mouseX+scrhor/3, mouseY+scrvert/3, LOWORD(lParam)+scrhor/3, HIWORD(lParam)+scrvert/3);
					 SelectObject(hdc2, hobj);
					 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, scrhor/3, scrvert/3, SRCCOPY);
				 }
				 if (menu == IM_POLYLINE || menu ==IM_POLYGON)
				 {
					 BitBlt(hdc2, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
					 MoveToEx(hdc2, mouseX+scrhor/3, mouseY+scrvert/3, NULL);
					 LineTo(hdc2, LOWORD(lParam)+scrhor/3, HIWORD(lParam)+scrvert/3);
					 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, scrhor/3, scrvert/3, SRCCOPY);
				 }
			 }
		break;
	 case WM_LBUTTONUP:
		 if (menu == IM_LINE || menu == IM_RECTANGLE || menu == IM_ELLIPSE)
		 {
			 BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, scrhor/3, scrvert/3, SRCCOPY);
		 }
		 if (menu == IM_POLYLINE || menu ==IM_POLYGON)
		 {
			 fl = false;
			 
		 }
		 fl = !fl;
		 break;
	 case WM_RBUTTONDOWN:
		 if (menu == IM_POLYLINE || menu==IM_POLYGON)
		 {
			 fl = false;
			 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, scrhor/3, scrvert/3, SRCCOPY);
			 BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY); 
		 }
		 if (menu == IM_POLYGON)
		 {
			 LineTo(hdc2, polX+scrhor/3, polY+scrvert/3);
			 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, scrhor/3, scrvert/3, SRCCOPY);
			 BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			 polFl = true;
		 }
		 break;
	case WM_DESTROY:
		ReleaseDC(hWnd, hdc);
		ReleaseDC(hWnd, hdc1);
		ReleaseDC(hWnd, hdc2);
		PostQuitMessage(0); 
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndclProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hdcch,hdcch1;
	int width=200, height=200;
	static HPEN pens, penm, penh;
	COLORREF clr;
	PAINTSTRUCT ps;
	TIME time;
	LPARAM param;
	RECT rect;
	static int timer,min,hour,xs,ys,xm,ym,xh,yh;
	HBITMAP hbmp;
	switch (message)
	{
	case WM_CREATE:
		hdcch = GetDC(hWnd);
		hdcch1 = CreateCompatibleDC(hdcch);
		hbmp = CreateCompatibleBitmap(hdcch, width, height);
		pens = CreatePen(PS_SOLID, 1,RGB(0,0,0));
		penm = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		penh = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		SelectObject(hdcch1, hbmp);
		rect.left = 0;
		rect.top = 0;
		rect.right = width;
		rect.bottom = height;
		FillRect(hdcch1, &rect, (HBRUSH)CreateSolidBrush(RGB(255, 255, 255)));
		SelectObject(hdcch1, penh);
		Ellipse(hdcch1, 30, 20, 160, 150);
		TextOut(hdcch1, 88, 25, L"12", 2);
		TextOut(hdcch1, 145, 75, L"3", 1);
		TextOut(hdcch1, 90, 130, L"6", 1);
		TextOut(hdcch1, 35, 75, L"9", 1);
		ReleaseDC(hWnd, hdcch);
		timer = SetTimer(hWnd, 1, 1000, NULL);
		time.xs = 0;
		time.ys = -60;
		time.xm = 0;
		time.ym = -50;
		time.xh = 0;
		time.yh = -30;
		time.min = 0;
		time.hour = 0;
		SetTime(time);
		xs=time.xs;
		ys=time.ys;
		xm=time.xm;
		ym=time.ym;
		xh=time.xh;
		yh=time.yh;
		min=time.min;
		hour=time.hour;
		break;
	case WM_TIMER:
		hdcch = GetDC(hWnd);
		min++;
		if (min== 60)
		{
			NewHand(xm, ym);
			min = 0;
			xs = 0;
			ys = -60;
			hour++;
		}
		if (hour == 12)
		{
			NewHand(xh, yh);
			hour = 0;
		}
		NewHand(xs, ys);
		BitBlt(hdcch, 0, 0, 200, 200, hdcch1, 0, 0, SRCCOPY);
		MoveToEx(hdcch, 95, 85, NULL);
		SelectObject(hdcch, pens);
		LineTo(hdcch, xs+95, ys+85);
		MoveToEx(hdcch, 95, 85, NULL);
		SelectObject(hdcch, penm);
		LineTo(hdcch, xm+95, ym+85);
		MoveToEx(hdcch, 95, 85, NULL);
		SelectObject(hdcch, penh);
		LineTo(hdcch, xh+95, yh+85);
		ReleaseDC(hWnd, hdcch);
		break;
	case WM_PAINT:
		hdcch = BeginPaint(hWnd, &ps);
		BitBlt(hdcch, 0, 0, 200,200 , hdcch1, 0, 0, SRCCOPY);
		MoveToEx(hdcch, 95, 85, NULL);
		SelectObject(hdcch, pens);
		LineTo(hdcch, xs+95, ys+85);
		MoveToEx(hdcch, 95, 85, NULL);
		SelectObject(hdcch, penm);
		LineTo(hdcch, xm+95, ym+85);
		MoveToEx(hdcch, 95, 85, NULL);
		SelectObject(hdcch, penh);
		LineTo(hdcch, xh+95, yh+85);
		EndPaint(hWnd, &ps);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL GetPenColor(HWND hwnd, COLORREF *clrref)
{
	CHOOSECOLOR cc;
	COLORREF aclrCust[16];
	int i;
	for (i = 0; i < 16; i++)
		aclrCust[i] = RGB(255, 255, 255);
	memset(&cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hwnd;
	cc.rgbResult = RGB(0, 0, 0);
	cc.lpCustColors = aclrCust;
	cc.Flags = 0;
	if (ChooseColor(&cc))
	{
		*clrref = cc.rgbResult;
		return TRUE;
	}
	else
		return FALSE;
}

void SaveEnhMetaFile(HWND hWnd)
{
	OPENFILENAME SFile;
	WCHAR FileName[MAX_PATH], FilePath[MAX_PATH];
	FileName[0] = '\0';
	FilePath[0] = '\0';
	SFile.lStructSize = sizeof(OPENFILENAME);
	SFile.hwndOwner = hWnd;
	SFile.lpstrFilter = L"EMF(*.emf)\0";
	SFile.lpstrCustomFilter = 0;
	SFile.lpstrFile = FilePath;
	SFile.nMaxFile = MAX_PATH * sizeof(WCHAR);
	SFile.lpstrFileTitle = FileName;
	SFile.nMaxFileTitle = MAX_PATH * sizeof(WCHAR);
	SFile.lpstrInitialDir = 0;
	SFile.lpstrDefExt = L"emf";
	SFile.lpstrTitle = L"Save file as:";
	SFile.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
	GetSaveFileName(&SFile);
	HDC hdcRef = GetDC(hWnd);
	int iWidthMM, iHeightMM, iWidthPels, iHeightPels;
	RECT EnhClient;
	iWidthMM = GetDeviceCaps(hdcRef, HORZSIZE);
	iHeightMM = GetDeviceCaps(hdcRef, VERTSIZE);
	iWidthPels = GetDeviceCaps(hdcRef, HORZRES);
	iHeightPels = GetDeviceCaps(hdcRef, VERTRES);
	GetClientRect(hWnd, &EnhClient);
	EnhClient.left = (EnhClient.left * iWidthMM * 100) / iWidthPels;
	EnhClient.top = (EnhClient.top * iHeightMM * 100) / iHeightPels;
	EnhClient.right = (EnhClient.right * iWidthMM * 100) / iWidthPels;
	EnhClient.bottom = (EnhClient.bottom * iHeightMM * 100) / iHeightPels;
	HDC hdcEmf = CreateEnhMetaFile(hdcRef, SFile.lpstrFile, &EnhClient, 0);
	BitBlt(hdcEmf, 0, 0, scrhor/3, scrvert/3, hdc1, scrhor/3, scrvert/3, SRCCOPY);
	CloseEnhMetaFile(hdcEmf);
	ReleaseDC(hWnd, hdcRef);
}

void OpenEnhMetaFile(HWND hWnd)
{
	OPENFILENAME LFile;
	WCHAR FileName[MAX_PATH], FilePath[MAX_PATH];
	FileName[0] = '\0';
 	FilePath[0] = '\0';
	LFile.lStructSize = sizeof(OPENFILENAME);
	LFile.hwndOwner = hWnd;
	LFile.lpstrFilter =L"EMF (*.emf)\0";
	LFile.lpstrCustomFilter = 0;
	LFile.lpstrFile = FilePath;
	LFile.nMaxFile = MAX_PATH * sizeof(WCHAR);
	LFile.lpstrFileTitle = FileName;
	LFile.nMaxFileTitle = MAX_PATH * sizeof(WCHAR);
	LFile.lpstrInitialDir = 0;
	LFile.lpstrDefExt = L"emf\0";
	LFile.lpstrTitle = L"Open file from:";
	LFile.Flags = OFN_PATHMUSTEXIST;
	GetOpenFileName(&LFile);
	HENHMETAFILE enhFile = GetEnhMetaFile(LFile.lpstrFile);
	RECT client;
	GetClientRect(hWnd, &client);
	client.left += scrhor / 3;
	client.top += scrvert / 3;
	client.right += scrhor / 3;
	client.bottom += scrvert / 3;
	PlayEnhMetaFile(hdc1, enhFile, &client);
	PlayEnhMetaFile(hdc2, enhFile, &client);
	DeleteEnhMetaFile(enhFile);
	InvalidateRect(hWnd, NULL, false);
}

void Print(HWND hWnd)
{
	PRINTDLG PrintDialog;
	memset(&PrintDialog, 0, sizeof(PRINTDLG));
	PrintDialog.lStructSize = sizeof(PRINTDLG);
	PrintDialog.hwndOwner = hWnd;
	PrintDialog.hDevMode = NULL;
	PrintDialog.hDevNames = NULL;
	PrintDialog.nCopies = 1;
	PrintDialog.Flags = PD_RETURNDC;
	PrintDialog.nMinPage = 1;
	PrintDialog.nMaxPage = 0xFFFF;
	PrintDlg(&PrintDialog);
	DOCINFO docInfo;
	docInfo.cbSize = sizeof(docInfo);
	docInfo.lpszDocName = L"Printing";
	docInfo.lpszOutput = 0;
	docInfo.lpszDatatype = 0;
	docInfo.fwType = 0;
	StartDoc(PrintDialog.hDC, &docInfo); 
	RECT printRect;
	GetClientRect(hWnd, &printRect);
	StretchBlt(PrintDialog.hDC, 0, 0, scrhor*4, scrvert*4, hdc1, scrhor/3, scrvert/3, scrhor/3*2,scrvert/3*2, SRCCOPY);
	EndDoc(PrintDialog.hDC);
}

LPACCEL TableAccel()
{
	ACCEL table[4];
	int i;
	for (i = 0; i < 4; i++)
	{
		table[i].fVirt = FSHIFT;
	}
	table[0].key = 0x53;
	table[0].cmd = IM_SAVE;
	table[1].key = 0x4f;
	table[1].cmd = IM_OPEN;
	table[2].key = 0x50;
	table[2].cmd = IM_PRINT;
	table[3].key = 0x45;
	table[3].cmd = IM_EXIT;
	return table;
}

void NewHand(int &x, int &y)
{
	int temp;
	float ftemp;
	temp = x;
	ftemp = (x*cos(pi / 30) - y*sin(pi / 30));
	if (ftemp > 0)
	{
		x = ftemp + 0.5;
	}
	else
		x = ftemp - 0.5;
	ftemp = (temp*sin(pi / 30) + y*cos(pi / 30));
	if (ftemp > 0)
	{
		y = ftemp + 0.5;
	}
	else
		y = ftemp - 0.5;
}

void SetTime(TIME &t)
{
	SYSTEMTIME time;
	int i,hour;
	GetSystemTime(&time);
	t.min = time.wSecond;
	t.hour = time.wMinute % 12;
	for (i = 0; i < time.wSecond; i++)
	{
		NewHand(t.xs, t.ys);
	}
	for (i = 0; i < time.wMinute; i++)
	{
		NewHand(t.xm, t.ym);
	}
	hour = (time.wHour % 12+3)%12*5+time.wMinute/12;
	for (i = 0; i < hour; i++)
	{
		NewHand(t.xh, t.yh);
	}
}