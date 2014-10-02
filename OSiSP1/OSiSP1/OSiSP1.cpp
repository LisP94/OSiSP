// OSiSP1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OSiSP1.h"
#include <commdlg.h>
#define PENCIL 1
#define LINE 2
#define POLYLINE 8
#define POLYGON 9
#define WIDTH_1 10
#define WIDTH_2 11
#define WIDTH_3 12
#define WIDTH_4 13
#define WIDTH_5 14
#define WIDTH_6 15
#define WIDTH_7 16
#define WIDTH_8 17
#define WIDTH_9 18
#define WIDTH_10 19

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL GetPenColor(HWND hwnd, COLORREF *clrref);


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

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OSISP1));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

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
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_OSISP1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   HMENU hMenu, hSubMenu,hSubMenuWidth;
   hMenu = CreateMenu();
   hSubMenu = CreatePopupMenu();

   hSubMenuWidth = CreatePopupMenu();
   AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"Shapes");
   AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenuWidth, L"Width");
   AppendMenu(hMenu, MF_STRING, 6, L"Zoom");
   AppendMenu(hMenu, MF_STRING, 20, L"Zoom-");
   AppendMenu(hMenu, MF_STRING, 21, L"Save");
   AppendMenu(hMenu, MF_STRING, 22, L"Open");
   AppendMenu(hSubMenuWidth, MF_STRING, WIDTH_1, L"1");
   AppendMenu(hSubMenuWidth, MF_STRING, WIDTH_2, L"2");
   AppendMenu(hSubMenuWidth, MF_STRING, WIDTH_3, L"3");
   AppendMenu(hSubMenuWidth, MF_STRING, WIDTH_4, L"4");
   AppendMenu(hSubMenuWidth, MF_STRING, WIDTH_5, L"5");
   AppendMenu(hSubMenuWidth, MF_STRING, WIDTH_6, L"6");
   AppendMenu(hSubMenuWidth, MF_STRING, WIDTH_7, L"7");
   AppendMenu(hSubMenuWidth, MF_STRING, WIDTH_8, L"8");
   AppendMenu(hSubMenuWidth, MF_STRING, WIDTH_9, L"9");
   AppendMenu(hSubMenuWidth, MF_STRING, WIDTH_10, L"10");
   AppendMenu(hMenu, MF_STRING, 5, L"Color");
   AppendMenu(hMenu, MF_STRING, 0, L"About");
   AppendMenu(hMenu, MF_STRING, 4, L"Exit");
   AppendMenu(hSubMenu, MF_STRING, PENCIL, L"Pencil");
   AppendMenu(hSubMenu, MF_STRING, LINE, L"Line");
   AppendMenu(hSubMenu, MF_STRING, 3, L"Rectangle"); 
   AppendMenu(hSubMenu, MF_STRING, 7, L"Ellipse");
   AppendMenu(hSubMenu, MF_STRING, POLYLINE, L"Polyline");
   AppendMenu(hSubMenu, MF_STRING, POLYGON, L"Polygon");
   SetMenu(hWnd, hMenu);
   SetMenu(hWnd, hSubMenu);
   if (!hWnd)
   {
	   return FALSE; 
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
	int wmId, wmEvent;
	static int scrvert,scrhor,mouseX,mouseY,menu,i=0,width,polX,polY;
	static bool polFl = true,fl=false;
	static float k=1;
	PAINTSTRUCT ps;
	static HDC hdc,hdc1,hdc2;
	HBITMAP hbmp1,hbmp2;
	HGDIOBJ hobj;
	static RECT r;
	static COLORREF clr;
	static HPEN hpen;
	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		scrhor = GetDeviceCaps(hdc, HORZRES);
		scrvert = GetDeviceCaps(hdc, VERTRES);
		hdc1 = CreateCompatibleDC(hdc);
		hdc2 = CreateCompatibleDC(hdc);
		hbmp1 = CreateCompatibleBitmap(hdc, scrhor+1,scrvert+1);
		hbmp2 = CreateCompatibleBitmap(hdc, scrhor+1, scrvert+1);
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
		case 0:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case 4:
			DestroyWindow(hWnd);
			break;
		case PENCIL:
			menu = 1;
			break;
		case LINE:
			menu = 2;
			break;
		case 3:
			menu = 3;
			break;
		case 5:
			GetPenColor(hWnd, &clr);
			hpen = CreatePen(PS_SOLID, 1, clr);
			SelectObject(hdc, hpen);
			SelectObject(hdc1, hpen);
			SelectObject(hdc2, hpen);
			break;
		case 6:
			StretchBlt(hdc2, 0, 0, 1366*k, 768*k, hdc1, 0, 0, 1366, 768, SRCCOPY);
			BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			k=k+0.01;
			break;
		case POLYLINE:
			menu = 8;
			break;
		case POLYGON:
			menu = 9;
			break;
		case 20:
			StretchBlt(hdc2, 0, 0, 1366*k, 768*k, hdc1, 0, 0, 1366, 768, SRCCOPY);
			BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY); 
			k=k-0.01;
			break;
		case 7:
			menu = 7;
			break;
		case WIDTH_1:
			width = 1;
			break;
		case WIDTH_2:
			width = 2;
			break;
		case WIDTH_3:
			width = 3;
			break;
		case WIDTH_4:
			width = 4;
			break;
		case WIDTH_5:
			width = 5;
			break;
		case WIDTH_6:
			width = 6;
			break;
		case WIDTH_7:
			width = 7;
			break;
		case WIDTH_8:
			width = 8;
			break;
		case WIDTH_9:
			width = 9;
			break;
		case WIDTH_10:
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
		BitBlt(hdc, 0, 0, scrvert, scrhor, hdc1, 0, 0, SRCCOPY);  
		//InvalidateRect(hWnd, &r, false);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_SHIFT)
		{
			
		}
		break; 
	case WM_MOUSEWHEEL:
		if (HIWORD(wParam) > WHEEL_DELTA)
		{
			ScrollDC(hdc1, 0, 10, &r, NULL, NULL, NULL);
			BitBlt(hdc, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
		}
		else
		{
			ScrollDC(hdc1, 0, -10, &r, NULL, NULL, NULL);
			BitBlt(hdc, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
		}
		break;
	case WM_LBUTTONDOWN:
		if (menu)
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
			if (menu == 9 && polFl)
			{
				polX = mouseX;
				polY = mouseY;
				polFl = false;
			}
			MoveToEx(hdc1, mouseX, mouseY, NULL); 
			MoveToEx(hdc2, mouseX, mouseY, NULL);
			fl = !fl;
			if (menu == 8 || menu==9)
			{
				fl = false;
				BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
				BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			}
		}
		break;
     case WM_MOUSEMOVE:
			 if (fl)
			 {
				 hdc = GetDC(hWnd);
				 SelectObject(hdc1, hpen);
				 if (menu == 1)
				 {
					 MoveToEx(hdc1, mouseX, mouseY, NULL);
					 mouseX = LOWORD(lParam);
					 mouseY = HIWORD(lParam);
					 LineTo(hdc1, mouseX, mouseY);
					 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
				 }
				 if (menu == 2)
				 {
					 BitBlt(hdc2, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
					 MoveToEx(hdc2, mouseX, mouseY, NULL);
					 LineTo(hdc2, LOWORD(lParam),HIWORD(lParam));
					 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
				 }
				 if (menu == 3)
				 {
					 BitBlt(hdc2, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
					 MoveToEx(hdc2, mouseX, mouseY, NULL);
					 hobj=SelectObject(hdc2, GetStockObject(NULL_BRUSH));
					 Rectangle(hdc2, mouseX, mouseY, LOWORD(lParam), HIWORD(lParam));
					 SelectObject(hdc2, hobj);
					 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
				 }
				 if (menu == 7)
				 {
					 BitBlt(hdc2, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
					 MoveToEx(hdc2, mouseX, mouseY, NULL); 
					 hobj=SelectObject(hdc2, GetStockObject(NULL_BRUSH));
					 Ellipse(hdc2, mouseX, mouseY, LOWORD(lParam), HIWORD(lParam));
					 SelectObject(hdc2, hobj);
					 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
				 }
				 if (menu == 8 || menu ==9)
				 {
					 BitBlt(hdc2, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
					 MoveToEx(hdc2, mouseX, mouseY, NULL);
					 LineTo(hdc2, LOWORD(lParam), HIWORD(lParam));
					 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
				 }
			 }
		break;
	 case WM_LBUTTONUP:
		 if (menu == 2 || menu == 3 || menu == 7)
		 {
			 BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
		 }
		 if (menu == 8 || menu ==9)
		 {
			 fl = false;
			 
		 }
		 fl = !fl;
		 break;
	 case WM_RBUTTONDOWN:
		 if (menu == 8 || menu==9)
		 {
			 fl = false;
			 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			 BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY); 
		 }
		 if (menu == 9)
		 {
			 LineTo(hdc2, polX, polY);
			 BitBlt(hdc, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			 BitBlt(hdc1, 0, 0, scrhor, scrvert, hdc2, 0, 0, SRCCOPY);
			 polFl = true;
		 }
		 break;
	case WM_DESTROY:
		PostQuitMessage(0); 
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

	// Подготавливаем массив цветов
	// "Custom Colors"
	for (i = 0; i < 16; i++)
		aclrCust[i] = RGB(255, 255, 255);

	// Записываем нулевые значения во все
	// неиспользуемые поля структуры CHOOSECOLOR
	memset(&cc, 0, sizeof(CHOOSECOLOR));

	// Заполняем необходимые поля
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hwnd;
	cc.rgbResult = RGB(0, 0, 0);
	cc.lpCustColors = aclrCust;
	cc.Flags = 0;

	// Выбираем цвет и возвращаем результат
	if (ChooseColor(&cc))
	{
		*clrref = cc.rgbResult;
		return TRUE;
	}
	else
		return FALSE;
}
