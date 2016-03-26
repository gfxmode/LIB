#include "DebugTrace.h"

CConsoleLog* CConsoleLog::m_pDbgObj = NULL;
CFileLog* CFileLog::m_pDbgObj = NULL;
vector<CConsoleLog*> CDebugComposite::m_vecDbgComp;
CDebugComposite* CDebugComposite::m_pCompObj = NULL;

CFileLog::CFileLog()
{
    m_pDbgObj = NULL;

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

    if(NULL != m_pDbgObj)
    {
        delete m_pDbgObj;
        m_pDbgObj = NULL;
    }
}
 void CFileLog::writeFile(const char *arrMsg)
{
    m_pMutexLock->lock();

    m_pFile = fopen(m_strLogName.c_str(), "a+");
    if(NULL != m_pFile)
    {
        SYSTEMTIME sysTime = TimeWrapper::getLocalTime();
        char arrTimeStamp[MAX_TIME_STAMP_LENGTH];
        memset(arrTimeStamp, 0, sizeof(arrTimeStamp));
        sprintf(arrTimeStamp, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d.%3.3d", sysTime.wYear, sysTime.wMonth, \
                sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

        fprintf(m_pFile, "%s\n", arrMsg);
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

 CFileLog *CFileLog::getInstance()
 {
     if(NULL == m_pDbgObj)
         m_pDbgObj = new CFileLog();

     return m_pDbgObj;
 }

 void CFileLog::freeInstance()
 {
     if(NULL != m_pDbgObj)
     {
         delete m_pDbgObj;
         m_pDbgObj = NULL;
     }
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

CConsoleLog::CConsoleLog()
{
    m_pDbgObj = NULL;
}

CConsoleLog::~CConsoleLog()
{
    if(NULL != m_pDbgObj)
    {
        delete m_pDbgObj;
        m_pDbgObj = NULL;
    }
}

void CConsoleLog::writeFile(const char* arrMsg)
{
    // Write LogMsg to console
    printf("%s\n", arrMsg);
}

CConsoleLog *CConsoleLog::getInstance()
{
    if(NULL == m_pDbgObj)
        m_pDbgObj = new CConsoleLog();

    return m_pDbgObj;
}

void CConsoleLog::freeInstance()
{
    if(NULL != m_pDbgObj)
    {
        delete m_pDbgObj;
        m_pDbgObj = NULL;
    }
}

CDebugComposite::CDebugComposite()
{
    m_vecDbgComp.clear();
    m_pCompObj = NULL;
}

CDebugComposite::~CDebugComposite()
{
    // free CDebugTrace Object
    for(size_t i = 0; i < m_vecDbgComp.size(); i++)
    {
        if(NULL != m_vecDbgComp.at(i))
        {
            delete m_vecDbgComp.at(i);
            m_vecDbgComp.at(i) = NULL;
        }
    }
    m_vecDbgComp.clear();

    if(NULL != m_pCompObj)
    {
        delete m_pCompObj;
        m_pCompObj = NULL;
    }
}

void CDebugComposite::writeLog(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    formatDebugStr(format, arg);
    va_end(arg);

    for(size_t i = 0; i < m_vecDbgComp.size(); i++)
    {
        if(NULL == m_vecDbgComp.at(i))
            continue;

        (m_vecDbgComp.at(i))->writeFile(m_arrLogMsg);
    }
}

CDebugComposite *CDebugComposite::getInstance()
{
    if(NULL == m_pCompObj)
    {
        m_pCompObj = new CDebugComposite();
    }

    return m_pCompObj;
}

void CDebugComposite::freeInstance()
{
    // free CDebugTrace Object
    for(size_t i = 0; i < m_vecDbgComp.size(); i++)
    {
        if(NULL != m_vecDbgComp.at(i))
        {
            delete m_vecDbgComp.at(i);
            m_vecDbgComp.at(i) = NULL;
        }
    }
    m_vecDbgComp.clear();
}

void CDebugComposite::writeToConsole()
{
    getInstance();

    CConsoleLog* pConObj = new CConsoleLog();
    m_vecDbgComp.push_back(pConObj);
}

void CDebugComposite::writeToFile()
{
    getInstance();

    CFileLog* pFileObj = new CFileLog();
    m_vecDbgComp.push_back(pFileObj);
}

void CDebugComposite::formatDebugStr(const char *format, va_list arg)
{
    if (NULL == format) return;

    // Generate timestamp array
    SYSTEMTIME sysTime = TimeWrapper::getLocalTime();
    char arrTimeStamp[MAX_TIME_STAMP_LENGTH];
    memset(arrTimeStamp, 0, sizeof(arrTimeStamp));
    sprintf(arrTimeStamp, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d.%3.3d ", sysTime.wYear, sysTime.wMonth, \
            sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

    // Generate logMsg array
    memset(m_arrLogMsg, 0, sizeof(m_arrLogMsg));
    vsnprintf(m_arrLogMsg + strlen(arrTimeStamp), sizeof(m_arrLogMsg) - strlen(arrTimeStamp), format, arg);

    // Add timestamp array to head of logMsg array
    memcpy(m_arrLogMsg, arrTimeStamp, strlen(arrTimeStamp));

}

