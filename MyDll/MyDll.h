//#pragma once
#ifndef	MYDLL_API
#define	MYDLL_API extern "C" _declspec(dllimport)
#endif

MYDLL_API int MyAdd(int a, int b);
MYDLL_API int _stdcall MySubstract(int a, int b);

void ltPrint();

class PrintPoint
{
public:
	void middlePrint();
};

void _stdcall rtPrint();