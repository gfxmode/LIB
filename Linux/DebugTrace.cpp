#include "DebugTrace.h"

CDebugTrace* CDebugTrace::m_dbgObj = NULL;

CFileLog::CFileLog()
{
    m_strLogName = "debug.log";
    m_pFile = NULL;
    m_pMutexLock = new CMutexLock();

    // initial m_iLineCnt, count debug.log file lines
    m_iLineCnt = getFileLineNum();
}

CFileLog::~CFileLog()
{
    if(NULL != m_pFile)
        m_pFile = NULL;

    if(NULL != m_pMutexLock)
    {
        delete m_pMutexLock;
        m_pMutexLock = NULL;
    }
}
 void CFileLog::writeFile(const char *format, ...)
{
    va_list argp;
    va_start(argp, format);
    formatDebugStr(format, argp);

    m_pMutexLock->lock();

    m_pFile = fopen(m_strLogName.c_str(), "a+");
    if(NULL != m_pFile)
    {
        SYSTEMTIME sysTime = TimeWrapper::getLocalTime();
        char arrTimeStamp[MAX_TIME_STAMP_LENGTH];
        memset(arrTimeStamp, 0, sizeof(arrTimeStamp));
        sprintf(arrTimeStamp, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d.%3.3d", sysTime.wYear, sysTime.wMonth, \
                sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

        fprintf(m_pFile, "%s %s\n", arrTimeStamp, m_arrLogMsg);
        m_iLineCnt++;       // increase m_iLineCnt;

        if(m_iLineCnt > MAX_LOG_SIZE)
        {
            fclose(m_pFile);

            string strDest, strSrc;
            // backup log file: mv *debug.log[i]* to *debug.log[ i + 1 ]*
            char arrNewBack[10];
            char arrOldBack[10];
            for(int i = 1; i < MAX_LOG_BACKUP_NUM; i++)
            {
                memset(arrOldBack, 0, sizeof(arrOldBack));
                GlobalFun::my_itoa(MAX_LOG_BACKUP_NUM - i, arrOldBack);
                strSrc = m_strLogName + arrOldBack;
                memset(arrNewBack, 0, sizeof(arrNewBack));
                GlobalFun::my_itoa(MAX_LOG_BACKUP_NUM - i + 1, arrNewBack);
                strDest = m_strLogName + arrNewBack;

                rename(strSrc.c_str(), strDest.c_str());
            }
            // backup log file: mv *debug.log* to *debug.log1*
            memset(arrNewBack, 0, sizeof(arrNewBack));
            GlobalFun::my_itoa(1, arrNewBack);
            strDest = m_strLogName + arrNewBack;
            rename(m_strLogName.c_str(), strDest.c_str());

            m_iLineCnt = 0;     // reset m_iLineCnt after backup
        }
        else
            fclose(m_pFile);
    }

    m_pMutexLock->unLock();
}

int CFileLog::getFileLineNum()
{
    FILE* pFile = fopen(m_strLogName.c_str(), "r");
    int cnt = 0;
    char c;

    if(NULL == pFile)
        return cnt;

    while((c = getc(pFile)) != EOF)
    {
        if( c == '\n')
            cnt++;
    }
    fclose(pFile);

    return cnt;
}

// Format DebugStr to arrLogMsg
void CDebugTrace::formatDebugStr(const char *format, va_list argp)
{
    if (NULL == format) return;
    memset(m_arrLogMsg, 0, sizeof(m_arrLogMsg));
    vsnprintf(m_arrLogMsg, sizeof(m_arrLogMsg), format, argp);
}

void CDebugTrace::writeFile(const char *format, ...)
{
    va_list argp;
    va_start(argp, format);

    formatDebugStr(format, argp);
}

CDebugTrace *CDebugTrace::getInstance()
{
    if (NULL == m_dbgObj)
    {
        m_dbgObj = new CFileLog();
    }

    return m_dbgObj;
}

void CDebugTrace::freeInstance()
{
    if (NULL != m_dbgObj)
    {
        delete m_dbgObj;
        m_dbgObj = NULL;
    }
}
