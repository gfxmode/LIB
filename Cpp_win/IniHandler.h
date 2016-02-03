/** Ini File handler for Windows
 * author from the Internet
 * modified by: gfxmode@live.com
 * date: 2016-02-03
 * version: 1.0.0
 */

#pragma once
//// Source: http://www.codeproject.com/Articles/10809/A-Small-Class-to-Read-INI-File ////
#ifndef INIHANDLER_H
#define INIHANDLER_H

#include <windows.h>

class CIniHandler
{
public:
	CIniHandler(LPCTSTR szFileName);
	~CIniHandler(void);
	bool isFileExist();					// check if iniFile exists
	int ReadInteger(LPCTSTR szSection, LPCTSTR szKey, int iDefaultValue);
	float ReadFloat(LPCTSTR szSection, LPCTSTR szKey, float fltDefaultValue);
	bool ReadBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolDefaultValue);
	LPTSTR ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefaultValue);

	void WriteInteger(LPCTSTR szSection, LPCTSTR szKey, int iValue);
	void WriteFloat(LPCTSTR szSection, LPCTSTR szKey, float fltValue);
	void WriteBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolValue);
	void WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);
private:
	TCHAR m_szFileName[255];
};

#endif INIHANDLER_H
