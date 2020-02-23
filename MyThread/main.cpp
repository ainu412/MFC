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
	for (size_t i = 0; i < 5; i++)
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
	for (size_t i = 0; i < cnt; i++)
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

	for (size_t i = 0; i < THREAD_NUM; i++)
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

	for (size_t i = 0; i < ADD_TIMES; i++)
	{
		num++;
	}

	ReleaseMutex(hMutex);
	return 0;
}

unsigned WINAPI main_thread_substract1(LPVOID arg)
{
	WaitForSingleObject(hMutex, INFINITE);

	for (size_t i = 0; i < ADD_TIMES; i++)
	{
		num--;
	}

	ReleaseMutex(hMutex);
	return 0;
}
#endif
//WaitForMultipleObjects
//�������̣߳�һ����+1��һ����1
#define ADD_TIMES 50000
#define THREAD_NUM 50

long long num = 0;
unsigned __stdcall main_thread_add1(LPVOID arg)
{
	for (size_t i = 0; i < ADD_TIMES; i++)
	{
		num++;
	}
	return 0;
}
unsigned __stdcall main_thread_substract1(LPVOID arg)
{
	for (size_t i = 0; i < ADD_TIMES; i++)
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

	for (size_t i = 0; i < THREAD_NUM; i++)
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