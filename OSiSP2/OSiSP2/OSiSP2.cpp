// OSiSP2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <Windows.h>
#include <queue>
using namespace std;

typedef void FunctionInThread();
void f1();
void f2();
void f3();
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

	void AddTask(FunctionInThread* f)
	{
		functionQueue.push(f);
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
	queue<FunctionInThread*> functionQueue;
};

int _tmain(int argc, _TCHAR* argv[])
{
	int n;
	ThreadPool * threadPool;
	puts("Enter number of thread");
	scanf("%d", &n);
	threadPool = new ThreadPool(n);
	do
	{
		n = getch();
		if (n == '1')
			threadPool->AddTask(&f1);
		if (n == '2')
			threadPool->AddTask(&f2);
		if (n == '3')
			threadPool->AddTask(&f3);
	} while (n != 13);
	delete threadPool;
	return 0;
}

void f1()
{
	Sleep(1000);
}
void f2()
{
	Sleep(2000);
}
void f3()
{
	Sleep(3000);
}