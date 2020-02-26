#include "pch.h"
#include "MyDllDL.h"
#include <stdio.h>

void rttPrint()
{
	// 获取当前调用进程的窗口
	HWND hWnd = GetForegroundWindow();
	// 获取当前窗口DC
	HDC hDC = GetDC(hWnd);
	// 初始化打印内容
	char buf[64] = { 0 };
	sprintf_s(buf, 64, "打印一下%d~", 6);
	// 输出至屏幕
	TextOutA(hDC, 300, 0, buf, strlen(buf));
	// 释放
	ReleaseDC(hWnd, hDC);

}

void rbPrint()
{
	// 获取当前调用进程的窗口
	HWND hWnd = GetForegroundWindow();
	// 获取当前窗口DC
	HDC hDC = GetDC(hWnd);
	// 初始化打印内容
	char buf[64] = { 0 };
	sprintf_s(buf, 64, "打印一下%d~", 6);
	// 输出至屏幕
	TextOutA(hDC, 300, 300, buf, strlen(buf));
	// 释放
	ReleaseDC(hWnd, hDC);
}
