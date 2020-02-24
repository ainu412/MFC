#include <iostream>
#include <Windows.h>
#include <process.h>
using namespace std;

#if 0
unsigned  __stdcall thread_main_rr(void* lapse)
{
	int n = 10;
	while (n--)
	{
		int l = *(int*)lapse;
		cout << "肉肉在拉琴~" << endl;
		Sleep(l * 500);//每0.5个lapse执行一次
	}
	return 0;
}

unsigned  __stdcall thread_main_xx(void* lapse)
{
	int n = 10;
	while (n--)
	{
		int l = *(int*)lapse;
		cout << "虾虾在健身~" << endl;
		Sleep(l * 500);//每0.5个lapse执行一次
	}
	return 0;
}

int main()
{
	int r = 2, x = 1;
	unsigned rrID, xxID;
	_beginthreadex(NULL, 0, thread_main_rr, &r, 0, &rrID);
	_beginthreadex(NULL, 0, thread_main_xx, &x, 0, &xxID);

	cout << "肉肉ID:" << rrID << "\t虾虾ID:" << xxID << endl;
	Sleep(50000);
	return 0;
}
#endif
#if 0
DWORD WINAPI ThreadFunc(LPVOID arg)
{
	cout << "子线程ID:" << GetCurrentThreadId()
		<< ",传入参数值:" << *((int*)arg) << endl;
	Sleep(10000);
	return 0;
}
int main()
{
	int arg = 412;
	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, &arg, 0, &dwThreadId);
	cout << "主线程ID:" << GetCurrentThreadId() << endl;
	CloseHandle(hThread);

	Sleep(50000);
	return 0;
}

#endif
#if 0//线程随进程结束
unsigned __stdcall ThreadFunc(LPVOID arg)
{
	for (int i = 0; i < 5; i++)
	{
		cout << "running thread" << endl;
	}
	//Sleep(1000);
	return 0;
}
int main()
{
	int arg = 412;
	//DWORD dwThreadId;
	//HANDLE hThread=	CreateThread(NULL, 0, ThreadFunc, &arg, 0, &dwThreadId);
	unsigned uThreadId;
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, &arg, 0, &uThreadId);

	cout << "main end:" << GetCurrentThreadId() << endl;

	Sleep(2000);
	return 0;
}

#endif
#if 0
//WaitForSingleObject
//来等待一个内核对象变为已通知状态
unsigned __stdcall ThreadFunc(LPVOID arg)
{
	int cnt = *(int*)arg;
	for (int i = 0; i < cnt; i++)
	{
		cout << "running thread" << endl;
		Sleep(500);
	}
	return 0;
}
int main()
{
	cout << "main begin" << endl;
	int arg = 5;
	unsigned threadId;
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, &arg, 0, &threadId);

	WaitForSingleObject(hThread, 1000);
	cout << "main end" << endl;

	return 0;
}
#endif
#if 0//互斥体Mutex
#include <iostream>
#include <Windows.h>
#include <process.h>
using namespace std;

#define ADD_TIMES 50000
#define THREAD_NUM 50

long long num = 0;
HANDLE hMutex;//记得全局!

unsigned WINAPI main_thread_add1(LPVOID arg);
unsigned WINAPI main_thread_substract1(LPVOID arg);

int main()
{
	HANDLE hThread[THREAD_NUM];
	hMutex = CreateMutex(NULL, FALSE, NULL);//无特定对象

	for (int i = 0; i < THREAD_NUM; i++)
	{
		if (i % 2)
		{
			hThread[i] = (HANDLE)_beginthreadex(NULL, 0, main_thread_add1, NULL, 0, NULL);
		}
		else
		{
			hThread[i] = (HANDLE)_beginthreadex(NULL, 0, main_thread_substract1, NULL, 0, NULL);
		}
	}
	WaitForMultipleObjects(THREAD_NUM, hThread, TRUE, INFINITE);
	CloseHandle(hMutex);

	cout << "Final num = " << num << endl;

	return 0;
}

unsigned WINAPI main_thread_add1(LPVOID arg)
{
	WaitForSingleObject(hMutex, INFINITE);

	for (int i = 0; i < ADD_TIMES; i++)
	{
		num++;
	}

	ReleaseMutex(hMutex);
	return 0;
}

unsigned WINAPI main_thread_substract1(LPVOID arg)
{
	WaitForSingleObject(hMutex, INFINITE);

	for (int i = 0; i < ADD_TIMES; i++)
	{
		num--;
	}

	ReleaseMutex(hMutex);
	return 0;
}
#endif
#if 0
//WaitForMultipleObjects
//起两个线程，一个加+1，一个减1
#define ADD_TIMES 50000
#define THREAD_NUM 50

long long num = 0;
unsigned __stdcall main_thread_add1(LPVOID arg)
{
	for (int i = 0; i < ADD_TIMES; i++)
	{
		num++;
	}
	return 0;
}
unsigned __stdcall main_thread_substract1(LPVOID arg)
{
	for (int i = 0; i < ADD_TIMES; i++)
	{
		num--;
	}
	return 0;
}
int main()
{
	HANDLE hThread[THREAD_NUM];
	//hThread[0] = (HANDLE)_beginthreadex(NULL, 0, main_thread_add1, NULL, 0, NULL);
	//hThread[1] = (HANDLE)_beginthreadex(NULL, 0, main_thread_substract1, NULL, 0, NULL);

	for (int i = 0; i < THREAD_NUM; i++)
	{
		if (i%2)
		{
			hThread[i] = (HANDLE)_beginthreadex(NULL, 0, main_thread_add1, NULL, 0, NULL);
		} 
		else
		{
			hThread[i] = (HANDLE)_beginthreadex(NULL, 0, main_thread_substract1, NULL, 0, NULL);
		}
	}

	WaitForMultipleObjects(THREAD_NUM, hThread, TRUE, INFINITE);
	cout << "Final num = " << num << endl;
	return 0;
}
#endif
#if 0// 事件对象
// gets(str[i])如果是A则加一
#define MAX_STR 1024
static char str[MAX_STR];
static HANDLE hEvent;

unsigned WINAPI threadA(void* arg)
{
	WaitForSingleObject(hEvent, INFINITE);
	int cnt = 0;
	for (size_t i = 0; str[i] != 0; i++)
	{
		if (str[i] == 'A')
		{
			cnt++;
		}
	}
	printf("Number of A: %d", cnt);
	return 0;
}
//如果不为A则加一
unsigned WINAPI threadNA(void* arg)
{
	//WaitForSingleObject(hEvent, INFINITE);
	int cnt = 0;
	for (size_t i = 0; str[i] != 0; i++)
	{
		if (str[i] != 'A' && str[i] != '\n')
		{
			cnt++;
		}
	}
	printf("Number of not A: %d", cnt);
	SetEvent(hEvent);
	return 0;
}

int main()
{
	// 准备线程
	HANDLE hThread[2];
	// 获得控制台输入字符
	fputs("Please input string :", stdout);
	fgets(str, MAX_STR, stdin);
	// 开启线程
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, threadA, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, threadNA, NULL, 0, NULL);
	// 等待线程结束
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	// 清理资源
	ResetEvent(hEvent);
	CloseHandle(hEvent);

	return 0;
}
#endif
//#if 0//event事件互斥
static HANDLE hEvent;
static int iTickets = 100;//共卖100张票
unsigned WINAPI selA(LPVOID arg)
{
	while (iTickets > 0)
	{
		WaitForSingleObject(hEvent, INFINITE);
		Sleep(1);//保证顺序
		if (iTickets == 0)
		{
			break;
		}
		iTickets--;
		printf("A iTickets = %d\n", iTickets);
		SetEvent(hEvent);
	}
	return 0;
}
unsigned WINAPI selB(LPVOID arg)
{
	while (TRUE)
	{
		WaitForSingleObject(hEvent, INFINITE);//等待处于启动状态后再执行
		//Sleep(1);一次之能有一个执行,休眠短的可接收到自己的启动,卖的多
		if (iTickets == 0)
		{
			break;
		}
		iTickets--;
		printf("B iTickets = %d\n", iTickets);
		SetEvent(hEvent);//另一函数严阵以待,故set以后先执行严阵以待的函数
	}
	return 0;
}
int main()
{
	HANDLE hThread[2];
	//hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, selA, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, selB, NULL, 0, NULL);
	//SetEvent(hEvent);
	hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);//manual set自己让线程释放event,自己设为有信号状态; 人工就是线程结束return 0后自动设为有信号状态

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	ResetEvent(hEvent);
	CloseHandle(hEvent);
	
	return 0;
}
//#endif
#if 0//信号量
//增加输入数值
//一线程取得输入数值存入全局num
//另一线程实现相加
static int num = 0;
HANDLE hSema;

unsigned __stdcall getNum(LPVOID arg)
{
	for (size_t i = 0; i < 5; i++)
	{
		WaitForSingleObject(hSema, INFINITE);
		printf("Please input num:");
		scanf_s("%d", &num);
		ReleaseSemaphore(hSema, 1, 0);
	}
	return 0;
}
unsigned __stdcall getSum(LPVOID arg)
{
	int sum = 0;
	for (size_t i = 0; i < 5; i++)
	{
		WaitForSingleObject(hSema, INFINITE);
		sum += num;
		ReleaseSemaphore(hSema, 1, 0);
	}
	printf("sum =%d", sum);
	return 0;
}
int main()
{
	hSema = CreateSemaphore(NULL, 1, 2, NULL);

	//HANDLE hThread[2];
	//hThread[0]=
	_beginthreadex(NULL, 0, getNum, NULL, 0, NULL);
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, getSum, NULL, 0, NULL);

	//OpenThread()
	// 等待线程结束
	WaitForSingleObject(hThread, INFINITE);

	// 清理资源
	CloseHandle(hSema);

	return 0;
}
#endif
#if 0
static int num = 0;
HANDLE hSema1, hSema2;

unsigned __stdcall getNum(LPVOID arg)//getnum不用等getsum??
{
	ReleaseSemaphore(hSema1, 1, 0);//可以改为初始时刻就释放一个
	for (size_t i = 0; i < 2; i++)
	{
		WaitForSingleObject(hSema1, INFINITE);//一旦等到, 释放量-1, 释放量+初始量=0, 需要再次释放才能等到
		printf("Please input num:");
		scanf_s("%d", &num);
		ReleaseSemaphore(hSema2, 1, 0);
	}
	return 0;
}
unsigned __stdcall getSum(LPVOID arg)
{
	int sum = 0;
	for (size_t i = 0; i < 2; i++)
	{
		WaitForSingleObject(hSema2, INFINITE);
		sum += num;
		ReleaseSemaphore(hSema1, 1, 0);
	}
	printf("sum =%d", sum);
	return 0;
}
int main()
{
	hSema1 = CreateSemaphore(NULL, 0, 1, NULL);
	hSema2 = CreateSemaphore(NULL, 0, 1, NULL);
	//HANDLE hThread[2];
	//hThread[0]=
	unsigned threadID;
	_beginthreadex(NULL, 0, getNum, NULL, 0, NULL);
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, getSum, NULL, 0, NULL);

	//OpenThread()
	// 等待线程结束
	WaitForSingleObject(hThread, INFINITE);

	// 清理资源
	CloseHandle(hSema1);
	CloseHandle(hSema2);
	CloseHandle(hThread);

	return 0;
}
#endif
#if 0
//关键代码段
// 保证一次只能一个工作, 但无法保证交替工作
CRITICAL_SECTION g_cs;
static HANDLE hEvent;
static int iTickets = 100;//共卖100张票
unsigned WINAPI selA(LPVOID arg)
{
	while (iTickets > 0)
	{
		//WaitForSingleObject(hEvent, INFINITE);
		EnterCriticalSection(&g_cs);
		//Sleep(1);//保证顺序
		if (iTickets == 0)
		{
			LeaveCriticalSection(&g_cs);
			break;
		}
		iTickets--;
		printf("A iTickets = %d\n", iTickets);
		//SetEvent(hEvent);
		LeaveCriticalSection(&g_cs);
	}
	return 0;
}
unsigned WINAPI selB(LPVOID arg)
{
	while (TRUE)
	{
		//WaitForSingleObject(hEvent, INFINITE);//等待处于启动状态后再执行
		EnterCriticalSection(&g_cs);
		//Sleep(1);一次之能有一个执行,休眠短的可接收到自己的启动,卖的多
		if (iTickets == 0)
		{
			LeaveCriticalSection(&g_cs);
			break;
		}
		iTickets--;
		printf("B iTickets = %d\n", iTickets);
		//SetEvent(hEvent);//另一函数严阵以待,故set以后先执行严阵以待的函数
		LeaveCriticalSection(&g_cs);
	}
	return 0;
}
int main()
{
	HANDLE hThread[2];
	//hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, selA, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, selB, NULL, 0, NULL);
	//SetEvent(hEvent);
	//hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	InitializeCriticalSection(&g_cs);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	//ResetEvent(hEvent);
	//CloseHandle(hEvent);
	CloseHandle(hThread[0]);//为啥不用关了??
	CloseHandle(hThread[1]);//不能关指针,只能一个一个关
	//CloseHandle(*hThread);//不知道关了一个还是关了俩..反正好用了...
	DeleteCriticalSection(&g_cs);
	return 0;
}
#endif