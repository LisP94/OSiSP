// OSiSP6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	WCHAR str[30];
	std::cout << "Enter the process name:" << std::endl;
	std::wcin >> str;
	STARTUPINFO sti;
	ZeroMemory(&sti, sizeof(STARTUPINFO));
	sti.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;
	CreateProcess(str, L"", NULL, NULL, FALSE, 0, NULL, NULL, &sti, &pi);
	LPVOID buf = NULL;
	HANDLE hThread = NULL;
	__try
	{
		buf = VirtualAllocEx(pi.hProcess, 0, 15, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		WriteProcessMemory(pi.hProcess, buf, "E:\\HKdll.dll", 15, NULL);
		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"Kernel32"), "LoadLibraryA");
		hThread = CreateRemoteThread(pi.hProcess, NULL, 0, pfnThreadRtn, buf, 0, NULL);
		WaitForSingleObject(hThread, INFINITE);
	}
	__finally
	{
		if (buf != NULL)
			VirtualFreeEx(pi.hProcess, buf, 0, MEM_RELEASE);
		if (hThread != NULL)
			CloseHandle(hThread);
		if (pi.hProcess != NULL)
			CloseHandle(pi.hProcess);
	}
	return 0;
}

