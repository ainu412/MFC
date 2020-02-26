#include "pch.h"
#include "MyDll.h"
#include <stdio.h>

#define	MYDLL_API extern "C" _declspec(dllexport)

MYDLL_API int MyAdd(int a, int b)
{
	return (a + b);
}
// Delphi  pascal的从左至右的压栈方式 会在后面产生@8的符号
int _stdcall MySubstract(int a, int b)
{
	return (a - b);
}

void ltPrint()
{
	// 获取当前调用进程的窗口
	HWND hWnd = GetForegroundWindow();
	// 获取当前窗口DC
	HDC hDC = GetDC(hWnd);
	// 初始化打印内容
	char buf[64] = { 0 };
	sprintf_s(buf, 64, "打印一下%d~", 6);
	// 输出至屏幕
	TextOutA(hDC, 0, 0, buf, strlen(buf));
	// 释放
	ReleaseDC(hWnd, hDC);

}

void PrintPoint::middlePrint()
{
	// 获取当前调用进程的窗口
	HWND hWnd = GetForegroundWindow();
	// 获取当前窗口DC
	HDC hDC = GetDC(hWnd);
	// 初始化打印内容
	char buf[64] = { 0 };
	sprintf_s(buf, 64, "打印一下%d~", 6);
	// 输出至屏幕
	TextOutA(hDC, 100, 100, buf, strlen(buf));
	// 释放
	ReleaseDC(hWnd, hDC);
}
