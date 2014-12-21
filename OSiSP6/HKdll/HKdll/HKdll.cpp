// HKdll.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"

#pragma once
#include "HKdll.h"


using namespace HKdll;


typedef HANDLE(WINAPI *PCREATEFILEW) (
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile);
typedef BOOL(WINAPI *PWRITEFILE)(
	HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped);
typedef BOOL(WINAPI *PCLOSEHANDLE)(
	HANDLE hObject);
typedef BOOL(WINAPI *PREADFILE)(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped);
typedef LONG(WINAPI *PREGCREATEKEYEXW)(
	HKEY hKey,
	LPCTSTR lpSubKey,
	DWORD Reserved,
	LPTSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition);
typedef LONG(WINAPI *PREGOPENKEYEXW)(
	HKEY hKey,
	LPCTSTR lpSubKey,
	DWORD ulOptions,
	REGSAM samDesired,
	PHKEY phkResult);
typedef LONG(WINAPI* PREGCLOSEKEY)(
	HKEY hKey);
typedef LONG(WINAPI* PREGDELETEKEYW)(
	HKEY hKey,
	LPCTSTR lpSubKey);
typedef LONG(WINAPI* PREGDELETETREEW)(
	HKEY hKey,
	LPCTSTR lpSubKey);
typedef LONG(WINAPI* PREGDELETEVALUEW)(
	HKEY hKey,
	LPCTSTR lpValueName);
typedef LONG(WINAPI* PREGDELETEKEYVALUEW)(
	HKEY hKey,
	LPCTSTR lpSubKey,
	LPCTSTR lpValueName);
typedef LONG(WINAPI* PREGREPLACEKEYW)(
	HKEY hKey,
	LPCTSTR lpSubKey,
	LPCTSTR lpNewFile,
	LPCTSTR lpOldFile);
typedef LONG(WINAPI* PREGRESTOREKEYW)(
	HKEY hKey,
	LPCTSTR lpFile,
	DWORD dwFlags);
typedef LONG(WINAPI* PREGSAVEKEYW)(
	HKEY hKey,
	LPCTSTR lpFile,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes);
typedef LONG(WINAPI* PREGSAVEKEYEXW)(
	HKEY hKey,
	LPCTSTR lpFile,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD Flags);
typedef LONG(WINAPI* PREGGETVALUEW)(
	HKEY hkey,
	LPCTSTR lpSubKey,
	LPCTSTR lpValue,
	DWORD dwFlags,
	LPDWORD pdwType,
	PVOID pvData,
	LPDWORD pcbData);
typedef LONG(WINAPI* PREGLOADKEYW)(
	HKEY hKey,
	LPCTSTR lpSubKey,
	LPCTSTR lpFile);
typedef LONG(WINAPI* PREGSETKEYVALUEW)(
	HKEY hKey,
	LPCTSTR lpSubKey,
	LPCTSTR lpValueName,
	DWORD dwType,
	LPCVOID lpData,
	DWORD cbData);
typedef LONG(WINAPI* PREGSETVALUEEXW)(
	HKEY hKey,
	LPCTSTR lpValueName,
	DWORD Reserved,
	DWORD dwType,
	const BYTE* lpData,
	DWORD cbData);
typedef LONG(WINAPI* PREGCOPYTREEW)(
	HKEY hKeySrc,
	LPCTSTR lpSubKey,
	HKEY hKeyDest);

HANDLE file;
CRITICAL_SECTION cs;
FunctionInfo HookedWriteFile,
HookedCreateFileW,
HookedReadFile,
HookedRegCreateKeyExW,
HookedRegOpenKeyExW,
HookedRegCloseKey,
HookedRegDeleteKeyW,
HookedRegDeleteTreeW,
HookedRegDeleteValueW,
HookedRegDeleteKeyValueW,
HookedRegReplaceKeyW,
HookedRegRestoreKeyW,
HookedRegSaveKeyW,
HookedRegSaveKeyExW,
HookedRegGetValueW,
HookedRegLoadKeyW,
HookedRegSetKeyValueW,
HookedRegSetValueExW,
HookedRegCopyTreeW;
void WriteToLog(LPCWSTR func)
{
	EnterCriticalSection(&cs);
	WCHAR res[100] = L"";
	wcscat(res, func);
	wcscat(res, L"\n");
	DWORD size;
	((PWRITEFILE)HookedWriteFile.originalFunction)(file, res, (wcslen(res) * sizeof(WCHAR)), &size, NULL);
	LeaveCriticalSection(&cs);
}


HANDLE WINAPI HookCreateFileW(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	HANDLE result = ((PCREATEFILEW)HookedCreateFileW.originalFunction)(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
	WriteToLog(L"CreateFileW");
	return result;
}
BOOL WINAPI HookWriteFile(
	HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped)
{
	BOOL result = ((PWRITEFILE)HookedWriteFile.originalFunction)(
		hFile,
		lpBuffer,
		nNumberOfBytesToWrite,
		lpNumberOfBytesWritten,
		lpOverlapped);
	WriteToLog(L"WriteFile");
	return result;
}

BOOL WINAPI HookReadFile(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped)
{
	BOOL result = ((PREADFILE)HookedReadFile.originalFunction)(
		hFile,
		lpBuffer,
		nNumberOfBytesToRead,
		lpNumberOfBytesRead,
		lpOverlapped);
	WriteToLog(L"ReadFile");
	return result;
}
LONG WINAPI HookRegCreateKeyExW(
	HKEY hKey,
	LPCTSTR lpSubKey,
	DWORD Reserved,
	LPTSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition)
{
	LONG result = ((PREGCREATEKEYEXW)HookedRegCreateKeyExW.originalFunction)(
		hKey,
		lpSubKey,
		Reserved,
		lpClass,
		dwOptions,
		samDesired,
		lpSecurityAttributes,
		phkResult,
		lpdwDisposition);
	WriteToLog(L"RegCreateKeyExW");
	return result;
}
LONG WINAPI HookRegOpenKeyExW(
	HKEY hKey,
	LPCTSTR lpSubKey,
	DWORD ulOptions,
	REGSAM samDesired,
	PHKEY phkResult)
{
	LONG result = ((PREGOPENKEYEXW)HookedRegOpenKeyExW.originalFunction)(
		hKey,
		lpSubKey,
		ulOptions,
		samDesired,
		phkResult);
	WriteToLog(L"RegOpenKeyExW");
	return result;
}
LONG WINAPI HookRegCloseKey(
	HKEY hKey)
{
	LONG result = ((PREGCLOSEKEY)HookedRegCloseKey.originalFunction)(hKey);
	WriteToLog(L"RegCloseKey");
	return result;
}
LONG WINAPI HookRegDeleteKeyW(
	HKEY hKey,
	LPCTSTR lpSubKey)
{
	LONG result = ((PREGDELETEKEYW)HookedRegDeleteKeyW.originalFunction)(hKey, lpSubKey);
	WriteToLog(L"RegDeleteKeyW");
	return result;
}
LONG WINAPI HookRegDeleteTreeW(
	HKEY hKey,
	LPCTSTR lpSubKey)
{
	LONG result = ((PREGDELETETREEW)HookedRegDeleteTreeW.originalFunction)(hKey, lpSubKey);
	WriteToLog(L"RegDeleteTreeW");
	return result;
}
LONG WINAPI HookRegDeleteValueW(
	HKEY hKey,
	LPCTSTR lpValueName)
{
	LONG result = ((PREGDELETEVALUEW)HookedRegDeleteValueW.originalFunction)(hKey, lpValueName);
	WriteToLog(L"RegDeleteValueW");
	return result;
}
LONG WINAPI HookRegDeleteKeyValueW(
	HKEY hKey,
	LPCTSTR lpSubKey,
	LPCTSTR lpValueName)
{
	LONG result = ((PREGDELETEKEYVALUEW)HookedRegDeleteKeyValueW.originalFunction)(hKey,
		lpSubKey, lpValueName);
	WriteToLog(L"RegDeleteKeyValueW");
	return result;
}
LONG WINAPI HookRegReplaceKeyW(
	HKEY hKey,
	LPCTSTR lpSubKey,
	LPCTSTR lpNewFile,
	LPCTSTR lpOldFile)
{
	LONG result = ((PREGREPLACEKEYW)HookedRegReplaceKeyW.originalFunction)(hKey,
		lpSubKey,
		lpNewFile,
		lpOldFile);
	WriteToLog(L"RegReplaceKeyW");
	return result;
}
LONG WINAPI HookRegRestoreKeyW(
	HKEY hKey,
	LPCTSTR lpFile,
	DWORD dwFlags)
{
	LONG result = ((PREGRESTOREKEYW)HookedRegRestoreKeyW.originalFunction)(
		hKey,
		lpFile,
		dwFlags);
	WriteToLog(L"RegRestoreKeyW");
	return result;
}
LONG WINAPI HookRegSaveKeyW(
	HKEY hKey,
	LPCTSTR lpFile,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	LONG result = ((PREGSAVEKEYW)HookedRegSaveKeyW.originalFunction)(
		hKey,
		lpFile,
		lpSecurityAttributes);
	WriteToLog(L"RegSaveKeyW");
	return result;
}
LONG WINAPI HookRegSaveKeyExW(
	HKEY hKey,
	LPCTSTR lpFile,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD Flags)
{
	LONG result = ((PREGSAVEKEYEXW)HookedRegSaveKeyExW.originalFunction)(
		hKey,
		lpFile,
		lpSecurityAttributes, Flags);
	WriteToLog(L"RegSaveKeyExW");
	return result;
}
LONG WINAPI HookRegGetValueW(
	HKEY hkey,
	LPCTSTR lpSubKey,
	LPCTSTR lpValue,
	DWORD dwFlags,
	LPDWORD pdwType,
	PVOID pvData,
	LPDWORD pcbData)
{
	LONG result = ((PREGGETVALUEW)HookedRegGetValueW.originalFunction)(
		hkey,
		lpSubKey,
		lpValue,
		dwFlags,
		pdwType,
		pvData,
		pcbData);
	WriteToLog(L"RegGetValueW");
	return result;
}
LONG WINAPI HookRegLoadKeyW(
	HKEY hKey,
	LPCTSTR lpSubKey,
	LPCTSTR lpFile)
{
	LONG result = ((PREGLOADKEYW)HookedRegLoadKeyW.originalFunction)(
		hKey,
		lpSubKey,
		lpFile);
	WriteToLog(L"RegLoadKeyW");
	return result;
}
LONG WINAPI HookRegSetKeyValueW(
	HKEY hKey,
	LPCTSTR lpSubKey,
	LPCTSTR lpValueName,
	DWORD dwType,
	LPCVOID lpData,
	DWORD cbData)
{
	LONG result = ((PREGSETKEYVALUEW)HookedRegSetKeyValueW.originalFunction)(
		hKey,
		lpSubKey,
		lpValueName,
		dwType,
		lpData,
		cbData);
	WriteToLog(L"RegSetKeyValueW");
	return result;
}
LONG WINAPI HookRegSetValueExW(
	HKEY hKey,
	LPCTSTR lpValueName,
	DWORD Reserved,
	DWORD dwType,
	const BYTE* lpData,
	DWORD cbData)
{
	LONG result = ((PREGSETVALUEEXW)HookedRegSetValueExW.originalFunction)(
		hKey,
		lpValueName,
		Reserved,
		dwType,
		lpData,
		cbData);
	WriteToLog(L"RegSetValueExW");
	return result;
}
LONG WINAPI HookRegCopyTreeW(
	HKEY hKeySrc,
	LPCTSTR lpSubKey,
	HKEY hKeyDest)
{
	LONG result = ((PREGCOPYTREEW)HookedRegCopyTreeW.originalFunction)(
		hKeySrc,
		lpSubKey,
		hKeyDest);
	WriteToLog(L"RegCopyTreeW");
	return result;
}

FunctionInfo SetStruct(LPCSTR dllName, LPCSTR originalFunction, PROC hookFunction)
{
	FunctionInfo result;
	result.dllName = dllName;
	result.hookFunction = hookFunction;
	result.originalFunction = GetProcAddress(GetModuleHandleA(dllName), originalFunction);
	return result;
}

void HKOneModul(PCSTR, PROC, PROC, HMODULE);

void HKAllModuls(FunctionInfo fi)
{
	HMODULE thisMod = NULL;
	MEMORY_BASIC_INFORMATION mbi;
	if (VirtualQuery(HKAllModuls, &mbi, sizeof(mbi)) != 0)
	{
		thisMod = (HMODULE)mbi.AllocationBase;
	}
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(moduleEntry);
	for (BOOL success = Module32First(hSnapshot, &moduleEntry); success; success = Module32Next(hSnapshot, &moduleEntry))
	{
		if (moduleEntry.hModule != thisMod)
		{
			HKOneModul(fi.dllName, fi.originalFunction, fi.hookFunction, moduleEntry.hModule);
		}
	}
}


void HKOneModul(PCSTR moduleName, PROC currentFunc, PROC newFunc, HMODULE hMod)
{
	ULONG ulSize;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) ImageDirectoryEntryToData(hMod, TRUE,
		                                      IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);
	if (pImportDesc == NULL)
		return;
	for (; pImportDesc->Name; pImportDesc++)
	{
		PSTR pszModName = (PSTR)((PBYTE)hMod + pImportDesc->Name);
		if (lstrcmpiA(pszModName, moduleName) == 0)
		{
			PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
				((PBYTE)hMod + pImportDesc->FirstThunk);

			for (; pThunk->u1.Function; pThunk++)
			{
				PROC* ppfn = (PROC*)&pThunk->u1.Function;
				BOOL bFound = (*ppfn == currentFunc);
				if (bFound)
				{
					if (!WriteProcessMemory(GetCurrentProcess(), ppfn, &newFunc, 
						 sizeof(newFunc), NULL) && (GetLastError() == ERROR_NOACCESS))
					{
						DWORD dwOldProtect;
						if (VirtualProtect(ppfn, sizeof(newFunc), PAGE_WRITECOPY, &dwOldProtect))
						{
							WriteProcessMemory(GetCurrentProcess(), ppfn, &newFunc,	sizeof(newFunc), NULL);
							VirtualProtect(ppfn, sizeof(newFunc), dwOldProtect,	&dwOldProtect);
						}
					}
					return;
				}
			}
		}
	}
}


void HKdll::StartHook()
{
	HookedCreateFileW = SetStruct("Kernel32.dll", "CreateFileW", (PROC)HookCreateFileW);
	HKAllModuls(HookedCreateFileW);
	HookedWriteFile = SetStruct("Kernel32.dll", "WriteFile", (PROC)HookWriteFile);
	HKAllModuls(HookedWriteFile);
	HookedReadFile = SetStruct("Kernel32.dll", "ReadFile", (PROC)HookReadFile);
	HKAllModuls(HookedReadFile);
	HookedRegCreateKeyExW = SetStruct("Advapi32.dll", "RegCreateKeyExW", (PROC)HookRegCreateKeyExW);
	HKAllModuls(HookedRegCreateKeyExW);
	HookedRegOpenKeyExW = SetStruct("Advapi32.dll", "RegOpenKeyExW", (PROC)HookRegOpenKeyExW);
	HKAllModuls(HookedRegOpenKeyExW);
	HookedRegCloseKey = SetStruct("Advapi32.dll", "RegCloseKey", (PROC)HookRegCloseKey);
	HKAllModuls(HookedRegCloseKey);
	HookedRegDeleteKeyW = SetStruct("Advapi32.dll", "RegDeleteKeyW", (PROC)HookRegDeleteKeyW);
	HKAllModuls(HookedRegDeleteKeyW);
    HookedRegDeleteTreeW = SetStruct("Advapi32.dll", "RegDeleteTreeW", (PROC)HookRegDeleteTreeW);
	HKAllModuls(HookedRegDeleteTreeW);
	HookedRegDeleteValueW = SetStruct("Advapi32.dll", "RegDeleteValueW", (PROC)HookRegDeleteValueW);
	HKAllModuls(HookedRegDeleteValueW);
	HookedRegDeleteKeyValueW = SetStruct("Advapi32.dll", "RegDeleteKeyValueW", (PROC)HookRegDeleteKeyValueW);
	HKAllModuls(HookedRegDeleteKeyValueW);
	HookedRegReplaceKeyW = SetStruct("Advapi32.dll", "RegReplaceKeyW", (PROC)HookRegReplaceKeyW);
	HKAllModuls(HookedRegReplaceKeyW);
	HookedRegRestoreKeyW = SetStruct("Advapi32.dll", "RegRestoreKeyW", (PROC)HookRegRestoreKeyW);
	HKAllModuls(HookedRegRestoreKeyW);
	HookedRegSaveKeyW = SetStruct("Advapi32.dll", "RegSaveKeyW", (PROC)HookRegSaveKeyW);
	HKAllModuls(HookedRegSaveKeyW);
	HookedRegSaveKeyExW = SetStruct("Advapi32.dll", "RegSaveKeyExW", (PROC)HookRegSaveKeyExW);
	HKAllModuls(HookedRegSaveKeyExW);
	HookedRegGetValueW = SetStruct("Advapi32.dll", "RegGetValueW", (PROC)HookRegGetValueW);
	HKAllModuls(HookedRegGetValueW);
	HookedRegLoadKeyW = SetStruct("Advapi32.dll", "RegLoadKeyW", (PROC)HookRegLoadKeyW);
	HKAllModuls(HookedRegLoadKeyW);
	HookedRegSetKeyValueW = SetStruct("Advapi32.dll", "RegSetKeyValueW", (PROC)HookRegSetKeyValueW);
	HKAllModuls(HookedRegSetKeyValueW);
	HookedRegSetValueExW = SetStruct("Advapi32.dll", "RegSetValueExW", (PROC)HookRegSetValueExW);
	HKAllModuls(HookedRegSetValueExW);
	HookedRegCopyTreeW = SetStruct("Advapi32.dll", "RegCopyTreeW", (PROC)HookRegCopyTreeW);
	HKAllModuls(HookedRegCopyTreeW);
	IitializeCriticalSection(&cs);
        file = ((PCREATEFILEW)HookedCreateFileW.originalFunction)(L"E:\\results.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);      
}