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
#include "Lock.h"
#include "../LIB/GlobalDefine.h"
#include "../LIB/TimeWrapper.h"

using namespace std;
const int MAX_LOG_SIZE = 10000;
const int MAX_LOG_CHAR_LENGTH = 512;
const int MAX_TIME_STAMP_LENGTH = 20;
const int MAX_LOG_FILE_STRING_BUFF = 100;
const int MAX_LOG_BACKUP_NUM = 10;

// Base class
class CDebugTrace
{
public:
    CDebugTrace() { ; }
    virtual ~CDebugTrace() { ; }

public:
    virtual void formatDebugStr(const char* format, va_list argp);
    virtual void writeFile(const char* format, ...);

public:
    static CDebugTrace* getInstance();
    static void freeInstance();

protected:
    char m_arrLogMsg[MAX_LOG_CHAR_LENGTH];
    static CDebugTrace* m_dbgObj;
};

// Write log to File
class CFileLog : public CDebugTrace
{
public:
    CFileLog();
    virtual ~CFileLog();

public:
    void writeFile(const char* format, ...);
private:
    int getFileLineNum();

private:
    int m_iLineCnt;
    string m_strLogName;
    FILE* m_pFile;
    CMutexLock* m_pMutexLock;
};

#define dout CDebugTrace::getInstance()->writeFile

#endif // CDEBUGTRACE_H
