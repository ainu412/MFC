#include <process.h>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>//_T

void runExe()
{
 	//WCHAR lpApplicationName[] = 
 	//	_T("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
 	//WCHAR lpCommandLine[] = _T("http://amazon.cn");
	// 写一起才可以执行后面写的网址...
	WCHAR lpCommandLine[] = _T("\"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe\" http://amazon.cn");
	STARTUPINFOW StartupInfo;
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);
	PROCESS_INFORMATION ProcessInformation;
	int iRet = CreateProcess(
		NULL,
		lpCommandLine,
		NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInformation
	);
	if (iRet == 0)
	{
		printf("Process create failure, error code=%d", GetLastError());
	}
	else
	{
		printf("Process create success");
		WaitForSingleObject(ProcessInformation.hProcess, INFINITE);//等待子进程结束,康康要多久结束..
		CloseHandle(ProcessInformation.hThread);
		CloseHandle(ProcessInformation.hProcess);
		ProcessInformation.dwProcessId = 0;
		ProcessInformation.dwThreadId = 0;
		ProcessInformation.hProcess = NULL;
		ProcessInformation.hThread = NULL;
	}
}

int main()
{
	runExe();

	return 0;
}