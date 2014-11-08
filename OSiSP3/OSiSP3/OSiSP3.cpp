// OSiSP3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
#define MemorySize 1000
#define BufferSize 100


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMap,Mutex;
	LPVOID buffer;
	char * string;
	hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, L"SharedMemory");
	if (hMap == NULL)
	{
		hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MemorySize, L"SharedMemory");
		Mutex = CreateMutex(NULL, false, L"Mutex");
		__try
		{
			buffer = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, BufferSize);
			string = (char*)malloc(BufferSize - 1);
			while (strcmp(string, ""))
			{
				cout << "Process waits enter to critical section\n";
				WaitForSingleObject(Mutex, INFINITE);
				cout << "Process enter in critical section\n";
				gets(string);
				CopyMemory(buffer, string, BufferSize);
				ReleaseMutex(Mutex);
				cout << "Process leave critical section\n";
			}
		}
		__finally
		{
			CloseHandle(hMap);
			CloseHandle(Mutex);
		}
	}
	else
	{
		Mutex = OpenMutex(MUTEX_ALL_ACCESS, false, L"Mutex");
		buffer = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, BufferSize);
		__try
		{
		    string = (char*)malloc(BufferSize - 1);
		    bool flag = true;
			while (flag)
			{
				cout << "Process waits enter to critical section\n";
				if (WaitForSingleObject(Mutex, INFINITE) == WAIT_ABANDONED)
				{
					flag = false;
				}
				cout << "Process enter in critical section\n";
				CopyMemory(string, buffer, BufferSize);
				if (!strcmp(string, ""))
				{
					flag = false;
				}
				cout << string << "\n";
				Sleep(3000);
				ReleaseMutex(Mutex);
				cout << "Process leave critical section\n";
			}
		}
		__finally
		{
			CloseHandle(hMap);
			CloseHandle(Mutex);
		}
	}
	return 0;
}

