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
		cout << "����������~" << endl;
		Sleep(l * 500);//ÿ0.5��lapseִ��һ��
	}
	return 0;
}

unsigned  __stdcall thread_main_xx(void* lapse)
{
	int n = 10;
	while (n--)
	{
		int l = *(int*)lapse;
		cout << "ϺϺ�ڽ���~" << endl;
		Sleep(l * 500);//ÿ0.5��lapseִ��һ��
	}
	return 0;
}

int main()
{
	int r = 2, x = 1;
	unsigned rrID, xxID;
	_beginthreadex(NULL, 0, thread_main_rr, &r, 0, &rrID);
	_beginthreadex(NULL, 0, thread_main_xx, &x, 0, &xxID);

	cout << "����ID:" << rrID << "\tϺϺID:" << xxID << endl;
	Sleep(50000);
	return 0;
}
#endif
#if 0
DWORD WINAPI ThreadFunc(LPVOID arg)
{
	cout << "���߳�ID:" << GetCurrentThreadId()
		<< ",�������ֵ:" << *((int*)arg) << endl;
	Sleep(10000);
	return 0;
}
int main()
{
	int arg = 412;
	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, &arg, 0, &dwThreadId);
	cout << "���߳�ID:" << GetCurrentThreadId() << endl;
	CloseHandle(hThread);

	Sleep(50000);
	return 0;
}

#endif
#if 0//�߳�����̽���
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
//���ȴ�һ���ں˶����Ϊ��֪ͨ״̬
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
#if 0//������Mutex
#include <iostream>
#include <Windows.h>
#include <process.h>
using namespace std;

#define ADD_TIMES 50000
#define THREAD_NUM 50

long long num = 0;
HANDLE hMutex;//�ǵ�ȫ��!

unsigned WINAPI main_thread_add1(LPVOID arg);
unsigned WINAPI main_thread_substract1(LPVOID arg);

int main()
{
	HANDLE hThread[THREAD_NUM];
	hMutex = CreateMutex(NULL, FALSE, NULL);//���ض�����

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
//�������̣߳�һ����+1��һ����1
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
#if 0// �¼�����
// gets(str[i])�����A���һ
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
//�����ΪA���һ
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
	// ׼���߳�
	HANDLE hThread[2];
	// ��ÿ���̨�����ַ�
	fputs("Please input string :", stdout);
	fgets(str, MAX_STR, stdin);
	// �����߳�
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, threadA, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, threadNA, NULL, 0, NULL);
	// �ȴ��߳̽���
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	// ������Դ
	ResetEvent(hEvent);
	CloseHandle(hEvent);

	return 0;
}
#endif
//#if 0//event�¼�����
static HANDLE hEvent;
static int iTickets = 100;//����100��Ʊ
unsigned WINAPI selA(LPVOID arg)
{
	while (iTickets > 0)
	{
		WaitForSingleObject(hEvent, INFINITE);
		Sleep(1);//��֤˳��
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
		WaitForSingleObject(hEvent, INFINITE);//�ȴ���������״̬����ִ��
		//Sleep(1);һ��֮����һ��ִ��,���߶̵Ŀɽ��յ��Լ�������,���Ķ�
		if (iTickets == 0)
		{
			break;
		}
		iTickets--;
		printf("B iTickets = %d\n", iTickets);
		SetEvent(hEvent);//��һ���������Դ�,��set�Ժ���ִ�������Դ��ĺ���
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
	hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);//manual set�Լ����߳��ͷ�event,�Լ���Ϊ���ź�״̬; �˹������߳̽���return 0���Զ���Ϊ���ź�״̬

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	ResetEvent(hEvent);
	CloseHandle(hEvent);
	
	return 0;
}
//#endif
#if 0//�ź���
//����������ֵ
//һ�߳�ȡ��������ֵ����ȫ��num
//��һ�߳�ʵ�����
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
	// �ȴ��߳̽���
	WaitForSingleObject(hThread, INFINITE);

	// ������Դ
	CloseHandle(hSema);

	return 0;
}
#endif
#if 0
static int num = 0;
HANDLE hSema1, hSema2;

unsigned __stdcall getNum(LPVOID arg)//getnum���õ�getsum??
{
	ReleaseSemaphore(hSema1, 1, 0);//���Ը�Ϊ��ʼʱ�̾��ͷ�һ��
	for (size_t i = 0; i < 2; i++)
	{
		WaitForSingleObject(hSema1, INFINITE);//һ���ȵ�, �ͷ���-1, �ͷ���+��ʼ��=0, ��Ҫ�ٴ��ͷŲ��ܵȵ�
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
	// �ȴ��߳̽���
	WaitForSingleObject(hThread, INFINITE);

	// ������Դ
	CloseHandle(hSema1);
	CloseHandle(hSema2);
	CloseHandle(hThread);

	return 0;
}
#endif
#if 0
//�ؼ������
// ��֤һ��ֻ��һ������, ���޷���֤���湤��
CRITICAL_SECTION g_cs;
static HANDLE hEvent;
static int iTickets = 100;//����100��Ʊ
unsigned WINAPI selA(LPVOID arg)
{
	while (iTickets > 0)
	{
		//WaitForSingleObject(hEvent, INFINITE);
		EnterCriticalSection(&g_cs);
		//Sleep(1);//��֤˳��
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
		//WaitForSingleObject(hEvent, INFINITE);//�ȴ���������״̬����ִ��
		EnterCriticalSection(&g_cs);
		//Sleep(1);һ��֮����һ��ִ��,���߶̵Ŀɽ��յ��Լ�������,���Ķ�
		if (iTickets == 0)
		{
			LeaveCriticalSection(&g_cs);
			break;
		}
		iTickets--;
		printf("B iTickets = %d\n", iTickets);
		//SetEvent(hEvent);//��һ���������Դ�,��set�Ժ���ִ�������Դ��ĺ���
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
	CloseHandle(hThread[0]);//Ϊɶ���ù���??
	CloseHandle(hThread[1]);//���ܹ�ָ��,ֻ��һ��һ����
	//CloseHandle(*hThread);//��֪������һ�����ǹ�����..����������...
	DeleteCriticalSection(&g_cs);
	return 0;
}
#endif