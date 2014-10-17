// OSiSP2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

DWORD WINAPI ThreadProc(LPVOID lParam)
{
	return 0;
}

class ThreadPool
{
public:
	ThreadPool(int count)
	{
		if (count < 1)
		{
			n = 5;
		}
		else
		{
			n = count;
		}
		pool = (HANDLE*)malloc(sizeof(HANDLE)*count);
		for (int i = 0; i < count; i++)
		{
			pool[i] = CreateThread(NULL, 0, ThreadProc, NULL, CREATE_SUSPENDED, NULL);
		}
	}
	~ThreadPool()
	{
		for (int i = 0; i < n; i++)
		{
			TerminateThread(pool[i], 0);
		}
		free(pool);
	}

private:
	int n;
	HANDLE * pool;
};

int _tmain(int argc, _TCHAR* argv[])
{
	int n;
	ThreadPool * threadPool;
	puts("Enter number of thread");
	scanf("%d", &n);
	threadPool = new ThreadPool(n);
	delete threadPool;
	return 0;
}

