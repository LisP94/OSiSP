// OSiSP3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMap1,hMap2;
	char str1[5]="Bool", str2[5];
	LPVOID buf;
	hMap2 = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, L"SharedMemory");
	if (hMap2 == NULL)
	{
		hMap1 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1000, L"SharedMemory");
		buf = MapViewOfFile(hMap1, FILE_MAP_ALL_ACCESS, 0, 0, 10);
		CopyMemory(buf, str1, 5);
		getch();
		CloseHandle(hMap1);
		return 0;
	}
	buf=MapViewOfFile(hMap2, FILE_MAP_ALL_ACCESS, 0, 0, 10);
	CopyMemory(str2, buf, 5);
	cout << str2;
	getch();
	CloseHandle(hMap2);
	return 0;
}

