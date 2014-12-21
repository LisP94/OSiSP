#ifndef HK
#define HK

#include "stdafx.h"
#include <iostream>
#include <TlHelp32.h>
#include <ImageHlp.h>
#pragma comment(lib, "ImageHlp")

namespace HKdll
{
	struct FunctionInfo
	{
		PCSTR dllName;
		PROC originalFunction;
		PROC hookFunction;		
	};
	void StartHook();
}
#endif