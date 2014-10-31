// OSiSP2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <Windows.h>
#include <queue>

using namespace std;


typedef bool FunctionInThread(); 
bool f1();
bool f2();
bool f3();
bool FinFunction();



HANDLE semaphore, semaphoreEmptyQueue;
struct ThreadParam
{
	bool access;
	FunctionInThread* function;
	HANDLE semaphore;
};


 CRITICAL_SECTION cs;

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
		pool = (HANDLE*)malloc(sizeof(HANDLE)*n);
		threadparam = (ThreadParam*)malloc(sizeof(ThreadParam)*n);
		InitializeCriticalSection(&cs);
		for (int i = 0; i < n; i++)
		{
			threadparam[i].access = true;
			threadparam[i].semaphore = CreateSemaphore(NULL, 1, 1, NULL);
			pool[i] = CreateThread(NULL, 0, ThreadProc, &threadparam[i], CREATE_SUSPENDED, NULL);
		}
	}
	
	void AddTask(FunctionInThread* f)
	{
		taskQueue.push(f);
	}

    void Start()
	{
		ResumeThread(pool[0]);
	}

	~ThreadPool() 
	{
		for (int i = 0; i < n; i++)
		{
			AddTask(&FinFunction);
		}
		WaitForMultipleObjects(n, pool, true, 5000);
		free(pool);
		free(threadparam);
	}

private:
    static DWORD WINAPI ThreadProc(LPVOID lParam)
	{
		bool flag = true;
		HANDLE a;
		ThreadParam * param = (ThreadParam*)lParam;
		FunctionInThread * function;
		while (flag)
		{
			if (TryEnterCriticalSection(&cs))
			{
				for (int i = 1; i < n; i++)
				{
					WaitForSingleObject(threadparam[i].semaphore, 1);
					ResumeThread(pool[i]);
					a = param->semaphore;
				}
				while (true)
				{
					while (taskQueue.empty());
					bool add;
					for (int i = 1; i < n; i++)
					{
						if (threadparam[i].access)
						{
							threadparam[i].function = taskQueue.front();
							taskQueue.pop();
							ReleaseSemaphore(threadparam[i].semaphore, 1, NULL);
							WaitForSingleObject(threadparam[i].semaphore, 50);
							add = false;
							break;
						}
					}
					if (add)
					{
						//add new thread
					}
				}
				LeaveCriticalSection(&cs);
			}
   			WaitForSingleObject(param->semaphore, INFINITE);
			ReleaseSemaphore(param->semaphore, 1, NULL);
			param->access = false;
			function = param->function;
			flag = (*function)();
			param->access = true;
		}
		return 0;
	}
	static int n;
	static HANDLE * pool;
	static ThreadParam * threadparam;
    static queue <FunctionInThread*> taskQueue;
};

int ThreadPool::n;
HANDLE * ThreadPool::pool;
ThreadParam * ThreadPool::threadparam;
queue <FunctionInThread*> ThreadPool::taskQueue;

int _tmain(int argc, _TCHAR* argv[])
{
	int n;
	ThreadPool * threadPool;
	puts("Enter number of thread");  
	scanf("%d", &n);
	semaphore = CreateSemaphore(NULL, 1, 1, NULL);
	semaphoreEmptyQueue = CreateSemaphore(NULL, 1, 1, NULL);
	threadPool = new ThreadPool(n);
	threadPool->Start();
	//ThreadPool::Create(n);
	//ThreadPool::Start();
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
	//ThreadPool::Destroy();
	return 0;
}

bool f1()
{
	//for (int i = 0; i < 100; i++);
	Sleep(1000);
	puts("aaa");
	return true;
}

bool f2()
{
	//for (int i = 0; i < 1000; i++);
	Sleep(2000);
	puts("bbb");
	return true;
}

bool f3()
{
	//for (int i = 0; i < 10000; i++);
	Sleep(3000);
	puts("ccc");
	return true;
}

bool FinFunction()
{
	return false;
}