//// Source: http://www.codeproject.com/Articles/10809/A-Small-Class-to-Read-INI-File ////

#include "stdafx.h"

#include <iostream>
#include <tchar.h>
#include "IniHandler.h"

CIniHandler::CIniHandler(LPCTSTR szFileName)
{
	memset(m_szFileName, 0x00, sizeof(m_szFileName));
	memcpy(m_szFileName, szFileName, _tcslen(szFileName)*sizeof(TCHAR));
}

CIniHandler::~CIniHandler(void)
{
}

bool CIniHandler::isFileExist()
{
	FILE* f = fopen(m_szFileName, "rb");
	if (f == NULL) 
		return false;
	else
	{
		fclose(f);
		return true;
	}
}

int CIniHandler::ReadInteger(LPCTSTR szSection, LPCTSTR szKey, int iDefaultValue)
{
	int iResult = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_szFileName); 
	return iResult;
}
float CIniHandler::ReadFloat(LPCTSTR szSection, LPCTSTR szKey, float fltDefaultValue)
{
	TCHAR szResult[255];
	TCHAR szDefault[255];
	float fltResult;
	_stprintf_s(szDefault, 255, TEXT("%f"),fltDefaultValue);
	GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_szFileName); 
	fltResult =  (float)_tstof(szResult);
	return fltResult;
}
bool CIniHandler::ReadBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolDefaultValue)
{
	TCHAR szResult[255];
	TCHAR szDefault[255];
	bool bolResult;
	_stprintf_s(szDefault, 255, TEXT("%s"), bolDefaultValue? TEXT("True") : TEXT("False"));
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName); 
	bolResult =  (_tcscmp(szResult, TEXT("True")) == 0 || 
		_tcscmp(szResult, TEXT("true")) == 0) ? true : false;
	return bolResult;
}
LPTSTR CIniHandler::ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefaultValue)
{
	LPTSTR szResult = new TCHAR[255];
	memset(szResult, 0x00, sizeof(szResult));
	GetPrivateProfileString(szSection,  szKey, szDefaultValue, szResult, 255, m_szFileName); 
	return szResult;
}

void CIniHandler::WriteInteger(LPCTSTR szSection, LPCTSTR szKey, int iValue)
{
	TCHAR szValue[255];
	_stprintf_s(szValue, 255, TEXT("%d"), iValue);
	WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName); 
}
void CIniHandler::WriteFloat(LPCTSTR szSection, LPCTSTR szKey, float fltValue)
{
	TCHAR szValue[255];
	_stprintf_s(szValue, 255, TEXT("%f"), fltValue);
	WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName); 
}
void CIniHandler::WriteBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolValue)
{
	TCHAR szValue[255];
	_stprintf_s(szValue, 255, TEXT("%s"), bolValue ? TEXT("True") : TEXT("False"));
	WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName); 
}
void CIniHandler::WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
	WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName);
}

