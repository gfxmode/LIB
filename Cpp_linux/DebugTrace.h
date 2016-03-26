/** DebugTrace class for linux
 * author: gfxmode@live.com
 * date: 2016-01-25
 * version: 1.0.0.0
 */

#ifndef CDEBUGTRACE_H
#define CDEBUGTRACE_H

#include <string>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include "Lock.h"
#include "GlobalDefine.h"
#include "TimeWrapper.h"

using namespace std;
const int MAX_LOG_SIZE = 10000;
const int MAX_LOG_CHAR_LENGTH = 256;
const int MAX_TIME_STAMP_LENGTH = 30;
const int MAX_LOG_FILE_STRING_BUFF = 100;
const int MAX_LOG_BACKUP_NUM = 10;

// Base class
class CConsoleLog
{
public:
    CConsoleLog();
    virtual ~CConsoleLog();

public:
    virtual void writeFile(const char* arrMsg);

public:
    static CConsoleLog* getInstance();
    static void freeInstance();
    static CConsoleLog* m_pDbgObj;
};

// Write log to File
class CFileLog : public CConsoleLog
{
public:
    CFileLog();
    virtual ~CFileLog();

public:
    void writeFile(const char* arrMsg);

public:
    static CFileLog* getInstance();
    static void freeInstance();
    static CFileLog* m_pDbgObj;

private:
    int getFileLineNum();

private:
    int m_iLineCnt;
    string m_strLogName;
    FILE* m_pFile;
    CMutexLock* m_pMutexLock;
};

// CDebug composite class
class CDebugComposite
{
public:
    CDebugComposite();
    virtual ~CDebugComposite();

    void writeLog(const char* format, ...);

public:
    static CDebugComposite* getInstance();
    static void freeInstance();
    static void writeToConsole();
    static void writeToFile();

private:
    void formatDebugStr(const char* format, va_list arg);

private:
    static vector<CConsoleLog*> m_vecDbgComp;
    static CDebugComposite* m_pCompObj;
    char m_arrLogMsg[MAX_LOG_CHAR_LENGTH];
};

#define _dToFile CDebugComposite::writeToFile
#define _dToConsole CDebugComposite::writeToConsole
#define _dout CDebugComposite::getInstance()->writeLog

#endif // CDEBUGTRACE_H
