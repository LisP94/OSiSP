// OSiSP1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OSiSP1.h"
#include <commdlg.h>
#include <stdlib.h>
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
#define SAVE 21
#define OPEN 22
#define PRINT 23

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HDC hdc1, hdc2;
int scrvert, scrhor;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL GetPenColor(HWND hwnd, COLORREF *clrref);
void SaveEnhMetaFile(HWND hWnd);
void OpenEnhMetaFile(HWND hWnd);
void Print(HWND hWnd);
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
   HMENU hMenu, hSubMenuTools,hSubMenuWidth,hSubMenuFile;
   hMenu = CreateMenu();
   hSubMenuTools = CreatePopupMenu();
   hSubMenuWidth = CreatePopupMenu();
   hSubMenuFile = CreatePopupMenu();

   AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenuFile, L"File");
   AppendMenu(hSubMenuFile, MF_STRING, OPEN, L"Open             Shift+O");
   AppendMenu(hSubMenuFile, MF_STRING, SAVE, L"Save               Shift+S");
   AppendMenu(hSubMenuFile, MF_STRING, PRINT, L"Print               Shift+P");
   AppendMenu(hSubMenuFile, MF_STRING, 0, L"About");
   AppendMenu(hSubMenuFile, MF_STRING, 4, L"Exit                 Shift+E");
   AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenuTools, L"Tools");
   AppendMenu(hSubMenuTools, MF_STRING, PENCIL, L"Pencil");
   AppendMenu(hSubMenuTools, MF_STRING, LINE, L"Line");
   AppendMenu(hSubMenuTools, MF_STRING, 3, L"Rectangle");
   AppendMenu(hSubMenuTools, MF_STRING, 7, L"Ellipse");
   AppendMenu(hSubMenuTools, MF_STRING, POLYLINE, L"Polyline");
   AppendMenu(hSubMenuTools, MF_STRING, POLYGON, L"Polygon");
   AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenuWidth, L"Width");
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
   AppendMenu(hMenu, MF_STRING, 6, L"Zoom");
   AppendMenu(hMenu, MF_STRING, 20, L"Zoom-");
   AppendMenu(hMenu, MF_STRING, 5, L"Color");
   SetMenu(hWnd, hMenu);
   SetMenu(hWnd, hSubMenuTools);
   SetMenu(hWnd, hSubMenuFile);
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
	static int mouseX,mouseY,menu,i=0,width,polX,polY;
	static bool polFl = true,fl=false;
	static float k=1;
	PAINTSTRUCT ps;
	static HDC hdc;
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
		case SAVE:
			SaveEnhMetaFile(hWnd);
			break;
		case OPEN:
			OpenEnhMetaFile(hWnd);
			break;
		case PRINT:
			Print(hWnd);
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
	//Creating an Enhanced Metafile
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
	BitBlt(hdcEmf, 0, 0, scrhor, scrvert, hdc1, 0, 0, SRCCOPY);
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
	LFile.lpstrTitle = L"Load file from:";
	LFile.Flags = OFN_PATHMUSTEXIST;
	GetOpenFileName(&LFile);
	HENHMETAFILE enhFile = GetEnhMetaFile(LFile.lpstrFile);
	RECT client;
	GetClientRect(hWnd, &client);
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
	StartDoc(PrintDialog.hDC, &docInfo); //start print job
	RECT printRect;
	GetClientRect(hWnd, &printRect);
	StretchBlt(PrintDialog.hDC, 0, 0, scrhor, scrvert, hdc1, 0, 0, scrhor,scrvert, SRCCOPY);
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
	table[0].cmd = SAVE;
	table[1].key = 0x4f;
	table[1].cmd = OPEN;
	table[2].key = 0x50;
	table[2].cmd = PRINT;
	table[3].key = 0x45;
	table[3].cmd = 4;
	return table;
}