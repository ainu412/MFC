#include "pch.h"
#include "MyDllDL.h"
#include <stdio.h>

void rttPrint()
{
	// ��ȡ��ǰ���ý��̵Ĵ���
	HWND hWnd = GetForegroundWindow();
	// ��ȡ��ǰ����DC
	HDC hDC = GetDC(hWnd);
	// ��ʼ����ӡ����
	char buf[64] = { 0 };
	sprintf_s(buf, 64, "��ӡһ��%d~", 6);
	// �������Ļ
	TextOutA(hDC, 300, 0, buf, strlen(buf));
	// �ͷ�
	ReleaseDC(hWnd, hDC);

}

void rbPrint()
{
	// ��ȡ��ǰ���ý��̵Ĵ���
	HWND hWnd = GetForegroundWindow();
	// ��ȡ��ǰ����DC
	HDC hDC = GetDC(hWnd);
	// ��ʼ����ӡ����
	char buf[64] = { 0 };
	sprintf_s(buf, 64, "��ӡһ��%d~", 6);
	// �������Ļ
	TextOutA(hDC, 300, 300, buf, strlen(buf));
	// �ͷ�
	ReleaseDC(hWnd, hDC);
}
