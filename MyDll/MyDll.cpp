#include "pch.h"
#include "MyDll.h"
#include <stdio.h>

#define	MYDLL_API extern "C" _declspec(dllexport)

MYDLL_API int MyAdd(int a, int b)
{
	return (a + b);
}
// Delphi  pascal�Ĵ������ҵ�ѹջ��ʽ ���ں������@8�ķ���
int _stdcall MySubstract(int a, int b)
{
	return (a - b);
}

void ltPrint()
{
	// ��ȡ��ǰ���ý��̵Ĵ���
	HWND hWnd = GetForegroundWindow();
	// ��ȡ��ǰ����DC
	HDC hDC = GetDC(hWnd);
	// ��ʼ����ӡ����
	char buf[64] = { 0 };
	sprintf_s(buf, 64, "��ӡһ��%d~", 6);
	// �������Ļ
	TextOutA(hDC, 0, 0, buf, strlen(buf));
	// �ͷ�
	ReleaseDC(hWnd, hDC);

}

void PrintPoint::middlePrint()
{
	// ��ȡ��ǰ���ý��̵Ĵ���
	HWND hWnd = GetForegroundWindow();
	// ��ȡ��ǰ����DC
	HDC hDC = GetDC(hWnd);
	// ��ʼ����ӡ����
	char buf[64] = { 0 };
	sprintf_s(buf, 64, "��ӡһ��%d~", 6);
	// �������Ļ
	TextOutA(hDC, 100, 100, buf, strlen(buf));
	// �ͷ�
	ReleaseDC(hWnd, hDC);
}
