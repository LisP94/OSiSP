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
int asdf=0, qqq=0;
 CRITICAL_SECTION cs,csFile;

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
		InitializeCriticalSection(&csFile);
		fprintf(file, "Created %d threads.\n", n);
		for (int i = 0; i < n; i++)
		{
			threadparam[i].access = true;
			threadparam[i].remove = false;
			threadparam[i].function = NULL;
			threadparam[i].semaphore = CreateSemaphore(NULL, 0, 1, NULL);
			pool[i] = CreateThread(NULL, 0, ThreadProc, (LPVOID)i, 0, NULL);
		}
	}
	
	void AddTask(FunctionInThread* f)
	{
		taskQueue.push(f);
		for (int i = 0; i < n; i++)
		{
			if (threadparam[i].access)
			{
				ReleaseSemaphore(threadparam[i].semaphore,1,NULL);
				break;
			}
		}
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
		SYSTEMTIME time;
		bool flag = true, stopthread=false;
		int index = (int)lParam;
		FunctionInThread * function;
			while (flag)
			{
				if (TryEnterCriticalSection(&cs))
				{
					if (!taskQueue.empty())
					{
						function = taskQueue.front();
						if (function == &FinFunction)
						{
							stopthread = true;
						}
						for (int i = 0; i < n; i++)
						{
							if (threadparam[i].access && i!=index)
							{
								function = taskQueue.front();
								threadparam[i].function = function;
								taskQueue.pop();
								EnterCriticalSection(&csFile);
								fprintf(file, "Add new task.\n");
								LeaveCriticalSection(&csFile);
								ReleaseSemaphore(threadparam[i].semaphore, 1, NULL);
								if (taskQueue.empty())
									break;
							}
						}
						if (!stopthread && taskQueue.size()>1)
						{
							n++;
							pool = (HANDLE*)realloc(pool, sizeof(HANDLE)*n);
							threadparam = (ThreadParam*)realloc(threadparam, sizeof(ThreadParam)*n);
							threadparam[n - 1].access = true;
							threadparam[n - 1].remove = false;
							threadparam[n - 1].semaphore = CreateSemaphore(NULL, 1, 1, NULL);
							threadparam[n - 1].function = function;
							taskQueue.pop();
							pool[n - 1] = CreateThread(NULL, 0, ThreadProc, (LPVOID)(n - 1), 0, NULL);
							Sleep(100);
							EnterCriticalSection(&csFile);
							fprintf(file, "Exceeded the maximum number of threads. Created new thread.\n");
							LeaveCriticalSection(&csFile);
						}
						if ((taskQueue.size() < (n - 2)) && !stopthread && threadparam[n - 1].remove && n > beginthreadcount)
						{
							n--;
							threadparam[n].function = &FinFunction;
							EnterCriticalSection(&csFile);
							fprintf(file, "Removed thread %d.\n",(int)pool[n]);
							LeaveCriticalSection(&csFile);
							ReleaseSemaphore(threadparam[n].semaphore, 1, NULL);
							CloseHandle(threadparam[n].semaphore);
						}
						if (!taskQueue.empty())
						{
							threadparam[index].function = taskQueue.front();
							taskQueue.pop();
							EnterCriticalSection(&csFile);
							fprintf(file, "Add new task.\n");
							LeaveCriticalSection(&csFile);
							ReleaseSemaphore(threadparam[index].semaphore, 1, NULL);
						}
					}
					LeaveCriticalSection(&cs);
				}
				EnterCriticalSection(&csFile);
				fprintf(file, "Thread %d will wait 1 minute.\n", (int)pool[index]);
				LeaveCriticalSection(&csFile);
				if (WaitForSingleObject(threadparam[index].semaphore, 30000) == WAIT_TIMEOUT)
				{
					threadparam[index].remove = true;
				}
				else
				{
					threadparam[index].access = false;
					function = threadparam[index].function;
					if (function != NULL)
					{
						EnterCriticalSection(&csFile);
						GetSystemTime(&time);
						fprintf(file, "Thread %d start executing task: %d %d\n", (int)pool[index], time.wMinute,time.wSecond);
						LeaveCriticalSection(&csFile);
						flag = (*function)();
						EnterCriticalSection(&csFile);
						GetSystemTime(&time);
						fprintf(file, "Thread %d end executing task: %d %d\n", (int)pool[index], time.wMinute, time.wSecond);
						LeaveCriticalSection(&csFile);
					}
					if (flag)
					{
						threadparam[index].access = true;
					}
					threadparam[index].function = NULL;
				}
			}
			EnterCriticalSection(&csFile);
			fprintf(file, "Terminated thread %d.\n", (int)pool[index]);
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
		Sleep(1000);
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
		Sleep(2000);
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
		Sleep(3000);
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