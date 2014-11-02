// OSiSP2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <Windows.h>
#include <queue>
#include <iostream>
using namespace std;


typedef bool FunctionInThread(); 
bool f1();
bool f2();
bool f3();
bool FinFunction();

struct ThreadParam
{
	bool access;
	bool remove;
	FunctionInThread* function;
	HANDLE semaphore;
};

FILE * file;
int beginthreadcount;
 CRITICAL_SECTION cs,csEmptyQueue,csFile;
 CONDITION_VARIABLE cvEmptyQueue;

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
		beginthreadcount = n;
		pool = (HANDLE*)malloc(sizeof(HANDLE)*n);
		threadparam = (ThreadParam*)malloc(sizeof(ThreadParam)*n);
		InitializeCriticalSection(&cs);
		InitializeCriticalSection(&csEmptyQueue);
		InitializeConditionVariable(&cvEmptyQueue);
		InitializeCriticalSection(&csFile);
		for (int i = 0; i < n; i++)
		{
			threadparam[i].access = true;
			threadparam[i].remove = false;
			threadparam[i].semaphore = CreateSemaphore(NULL, 1, 1, NULL);
			pool[i] = CreateThread(NULL, 0, ThreadProc, (LPVOID)i, CREATE_SUSPENDED, NULL);
		}
		fprintf(file, "Created %d threads.\n",n);
		ResumeThread(pool[0]);
	}
	
	void AddTask(FunctionInThread* f)
	{
		EnterCriticalSection(&csEmptyQueue);
		taskQueue.push(f);
		WakeConditionVariable(&cvEmptyQueue);
		LeaveCriticalSection(&csEmptyQueue);
	}

	~ThreadPool() 
	{
		for (int i = 0; i < n; i++)
		{
			AddTask(&FinFunction);
		}
		WaitForMultipleObjects(n, pool, true, 5000);
		free(pool);
		for (int i = 0; i < n; i++)
		{
			CloseHandle(threadparam[i].semaphore);
		}
		free(threadparam);
	}

private:
    static DWORD WINAPI ThreadProc(LPVOID lParam)
	{
		bool flag = true, stopthread=false;
		int index = (int)lParam;
		FunctionInThread * function;
			while (flag)
			{
				if (TryEnterCriticalSection(&cs))
				{
					for (int i = 1; i < n; i++)
					{
						WaitForSingleObject(threadparam[i].semaphore, 1);
						ResumeThread(pool[i]);
					}
					int workthreadcount = n;
					while (true)
					{
						EnterCriticalSection(&csEmptyQueue);
						while (taskQueue.empty())
						{
							SleepConditionVariableCS(&cvEmptyQueue, &csEmptyQueue, INFINITE);
						}
						LeaveCriticalSection(&csEmptyQueue);
						if (workthreadcount == 1)
						{
							break;
						}
						bool add = true;
						function = taskQueue.front();
						if (function == &FinFunction)
						{
							stopthread = true;
						}
						for (int i = 1; i < n; i++)
						{
							if (threadparam[i].access)
							{
								if (function == &FinFunction)
								{
									workthreadcount--;
								}
								threadparam[i].function = function;
								taskQueue.pop();
								EnterCriticalSection(&csFile);
								fprintf(file,"Add new task.\n");
								LeaveCriticalSection(&csFile);
								ReleaseSemaphore(threadparam[i].semaphore, 1, NULL);
								WaitForSingleObject(threadparam[i].semaphore, 50);
								add = false;
								break;
							}
						}
						if (add && !stopthread)
						{
							n++;
							workthreadcount++;
							pool = (HANDLE*)realloc(pool, sizeof(HANDLE)*n);
							threadparam = (ThreadParam*)realloc(threadparam, sizeof(ThreadParam)*n);
							threadparam[n - 1].access = true;
							threadparam[n - 1].remove = false;
							threadparam[n - 1].semaphore = CreateSemaphore(NULL, 1, 1, NULL);
							threadparam[n - 1].function = function;
							taskQueue.pop();
							pool[n - 1] = CreateThread(NULL, 0, ThreadProc, (LPVOID)(n - 1), 0, NULL);
							EnterCriticalSection(&csFile);
							fprintf(file, "Exceeded the maximum number of threads. Created new thread.\n");
							LeaveCriticalSection(&csFile);
							Sleep(500);
							WaitForSingleObject(threadparam[n - 1].semaphore, 50);
						}
						if ((taskQueue.size() < (n - 3)) && !stopthread && threadparam[n-1].remove && n>beginthreadcount)
						{
							n--;
							workthreadcount--;
							threadparam[n].function = &FinFunction;
							EnterCriticalSection(&csFile);
							fprintf(file,"Removed one thread.\n");
							LeaveCriticalSection(&csFile);
							ReleaseSemaphore(threadparam[n].semaphore, 1, NULL);
							CloseHandle(threadparam[n].semaphore);
						}
					}
					LeaveCriticalSection(&cs);
					break;
				}
				if (WaitForSingleObject(threadparam[index].semaphore, 5000) == WAIT_TIMEOUT)
				{
					if (index == (n - 1))
					{
						threadparam[n-1].remove = true;
					}
				}
				else
				{
					ReleaseSemaphore(threadparam[index].semaphore, 1, NULL);
					threadparam[index].access = false;
					function = threadparam[index].function;
					flag = (*function)();
					if (flag)
					{
						threadparam[index].access = true;
					}
				}
			}
			EnterCriticalSection(&csFile);
			fprintf(file, "Terminated one thread.\n");
			LeaveCriticalSection(&csFile);
		return 0;
	}
	static int n;
	static HANDLE *pool;
	static ThreadParam  *threadparam;
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
	file = fopen("LOGG.txt", "wt");
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
	fprintf(file, "Terminated program.\n");
	fclose(file);
	return 0;
}

bool f1()
{
	try
	{
		for (int i = 0; i < 10000000; i++);
		puts("aaa");
	}
	catch (...)
	{
		EnterCriticalSection(&csFile);
		fprintf(file,"Error of executing users function.\n");
		LeaveCriticalSection(&csFile);
	}
	return true;
}

bool f2()
{
	try
	{
		for (int i = 0; i < 100000000; i++);
		puts("bbb");
	}
	catch (...)
	{
		EnterCriticalSection(&csFile);
		fprintf(file, "Error of executing users function.\n");
		LeaveCriticalSection(&csFile);
	}
	return true;
}

bool f3()
{
	try
	{
		for (int i = 0; i < 1000000000; i++);
		puts("ccc");
	}
	catch (...)
	{
		EnterCriticalSection(&csFile);
		fprintf(file, "Error of executing users function.\n");
		LeaveCriticalSection(&csFile);
	}
	return true;
}

bool FinFunction()
{
	return false;
}