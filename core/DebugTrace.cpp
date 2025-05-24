//---------------------------------------------------------------------------

//#include <vcl.h>
#pragma hdrstop

#include "DebugTrace.h"

//---------------------------------------------------------------------------
DebugTrace::DebugTrace() :
	hDebugDllInst(nullptr),
	m_hDebugForm(nullptr),
	m_pfStartSaveToFile(nullptr),
	m_pfStopSaveToFile(nullptr)
{
    m_pfCreateDebugForm  = nullptr;
    m_pfDestoryDebugForm = nullptr;
    m_pfCloseDebugForm   = nullptr;
    m_pfShowDebugForm    = nullptr;
    m_pfHideDebugForm    = nullptr;
    m_pfDebugPrint       = nullptr;

    m_pfSetColorSize     = nullptr;
    m_pfGetColorSize     = nullptr;
    m_pfOutPutMem        = nullptr;

    m_pPrintToBuffer     = nullptr;
    m_pfSetPrintSpeed    = nullptr;
    m_pfOutPutMemToBuffer= nullptr;
}

DebugTrace::~DebugTrace()
{
    if(hDebugDllInst != NULL)
    {
        m_pfCloseDebugForm();
        m_pfDestoryDebugForm();
        FreeLibrary(hDebugDllInst);
    }
}

enum RUNNING_BIT_SIZE DebugTrace::getRunningBitSize()
{
	int bits= sizeof(char *);
	if(bits == 4)
	{
		return RBS_x86;
	}
	else if(bits == 8)
	{
		return RBS_x64;
	}
	else
	{
        return RBS_BUTT;
    }
}

bool DebugTrace::Initial(const char *pTitle)
{
	enum RUNNING_BIT_SIZE enmRunningBitSize;

	enmRunningBitSize = getRunningBitSize();
	switch(enmRunningBitSize)
	{
		case RBS_x86:
#ifndef _WIN64
			hDebugDllInst = LoadLibrary(TEXT("DebugInfo.dll"));
			m_pfCreateDebugForm = (HWND (*)(char *pMainFrameTitle))GetProcAddress(hDebugDllInst, "_CreateDebugForm");
			m_pfDestoryDebugForm = (void (*)(void))GetProcAddress(hDebugDllInst, "_DestoryDebugForm");
			m_pfCloseDebugForm = (void (*)(void))GetProcAddress(hDebugDllInst, "_CloseDebugForm");
			m_pfShowDebugForm = (void (*)(void))GetProcAddress(hDebugDllInst, "_ShowDebugForm");
			m_pfHideDebugForm = (void (*)(void))GetProcAddress(hDebugDllInst, "_HideDebugForm");
			m_pfDebugPrint = (void (*)(char *Format, ...))GetProcAddress(hDebugDllInst, "_DebugPrint");

			m_pfSetColorSize = (void (*)(COLORREF FColor, int FSize))GetProcAddress(hDebugDllInst, "_SetColorSize");
			m_pfGetColorSize = (void (*)(COLORREF *FColor, int *FSize))GetProcAddress(hDebugDllInst, "_GetColorSize");
			m_pfOutPutMem = (void (*)(void *pMemPointer, unsigned int Length))GetProcAddress(hDebugDllInst, "_OutPutMem");

			m_pPrintToBuffer = (void (*)(bool StyleFlag, COLORREF FColor, int FSize, char *Format, ...))GetProcAddress(hDebugDllInst, "_DebugPrintToBuffer");
			m_pfSetPrintSpeed = (void (*)(int Interval))GetProcAddress(hDebugDllInst, "_SetBufferPrintSpeed");
			m_pfOutPutMemToBuffer = (void (*)(bool StyleFlag, COLORREF FColor, int FSize, void *pMemPointer, unsigned int Length))GetProcAddress(hDebugDllInst, "_OutPutMemToBuffer");
			m_pfStartSaveToFile = (pStartSaveToFile)GetProcAddress(hDebugDllInst, "_StartSaveToFile");
			m_pfStopSaveToFile = (pFunc)GetProcAddress(hDebugDllInst, "_StopSaveToFile");
#endif
			break;

		case RBS_x64:
#ifdef _WIN64
			hDebugDllInst = LoadLibrary(TEXT("DebugInfoX64.dll"));
			if (hDebugDllInst == nullptr) {
				throw "Can't load DebugInfoX64.dll";
			}
			m_pfCreateDebugForm = (HWND (*)(const char *pMainFrameTitle))GetProcAddress(hDebugDllInst, "CreateDebugForm");
			m_pfDestoryDebugForm = (void (*)(void))GetProcAddress(hDebugDllInst, "DestoryDebugForm");
			m_pfCloseDebugForm = (void (*)(void))GetProcAddress(hDebugDllInst, "CloseDebugForm");
			m_pfShowDebugForm = (void (*)(void))GetProcAddress(hDebugDllInst, "ShowDebugForm");
			m_pfHideDebugForm = (void (*)(void))GetProcAddress(hDebugDllInst, "HideDebugForm");
			m_pfDebugPrint = (void (*)(const char *Format, ...))GetProcAddress(hDebugDllInst, "DebugPrint");

			m_pfSetColorSize = (void (*)(COLORREF FColor, int FSize))GetProcAddress(hDebugDllInst, "SetColorSize");
			m_pfGetColorSize = (void (*)(COLORREF *FColor, int *FSize))GetProcAddress(hDebugDllInst, "GetColorSize");
			m_pfOutPutMem = (void (*)(void *pMemPointer, unsigned int Length))GetProcAddress(hDebugDllInst, "OutPutMem");

			m_pPrintToBuffer = (void (*)(bool StyleFlag, COLORREF FColor, int FSize, char *Format, ...))GetProcAddress(hDebugDllInst, "DebugPrintToBuffer");
			m_pfSetPrintSpeed = (void (*)(int Interval))GetProcAddress(hDebugDllInst, "SetBufferPrintSpeed");
			m_pfOutPutMemToBuffer = (void (*)(bool StyleFlag, COLORREF FColor, int FSize, void *pMemPointer, unsigned int Length))GetProcAddress(hDebugDllInst, "OutPutMemToBuffer");
			m_pfStartSaveToFile = (pStartSaveToFile)GetProcAddress(hDebugDllInst, "StartSaveToFile");
			m_pfStopSaveToFile = (pFunc)GetProcAddress(hDebugDllInst, "StopSaveToFile");
#endif
			break;

		default:
			break;
	}

	if(hDebugDllInst != nullptr && m_pfCreateDebugForm != nullptr
		&& m_pfDestoryDebugForm != nullptr && m_pfCloseDebugForm != nullptr
		&& m_pfShowDebugForm != nullptr && m_pfHideDebugForm != nullptr
		&& m_pfDebugPrint != nullptr && m_pfSetColorSize != nullptr
		&& m_pfGetColorSize != nullptr && m_pfOutPutMem != nullptr
		&& m_pPrintToBuffer != nullptr && m_pfSetPrintSpeed != nullptr
		&& m_pfOutPutMemToBuffer != nullptr	&& m_pfStartSaveToFile != nullptr
		&& m_pfStopSaveToFile != nullptr)
	{
		m_hDebugForm = m_pfCreateDebugForm(pTitle);
		return true;
	}
	else
	{
		PRINT_OUT_LAST_ERROR;
		return false;
	}
}
#pragma package(smart_init)
