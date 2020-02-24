#include <process.h>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>//_T

void runExe()
{
 	//WCHAR lpApplicationName[] = 
 	//	_T("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");
 	//WCHAR lpCommandLine[] = _T("http://amazon.cn");
	// дһ��ſ���ִ�к���д����ַ...
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
		WaitForSingleObject(ProcessInformation.hProcess, INFINITE);//�ȴ��ӽ��̽���,����Ҫ��ý���..
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