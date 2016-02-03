#include "TimeWrapper.h"

SYSTEMTIME TimeWrapper::getLocalTime()
{
    return _getSysTime(LOCAL_TIME);
}

SYSTEMTIME TimeWrapper::getSystemTime()
{
    return _getSysTime(SYSTEM_TIME);
}

SYSTEMTIME TimeWrapper::_getSysTime(TIME_OPTIONS tmOp)
{
    SYSTEMTIME sysTime;

    struct timespec ts;
    struct tm* p;
    clock_gettime(CLOCK_REALTIME, &ts);

    memset(&sysTime, 0, sizeof(sysTime));
    switch(tmOp)
    {
    case LOCAL_TIME:
        p = localtime(&(ts.tv_sec));
        break;
    case SYSTEM_TIME:
        p = gmtime(&(ts.tv_sec));
        break;
    default:
        p = NULL;
        break;
    }

    if(!p)
        return sysTime;

    sysTime.wYear = p->tm_year + 1900;
    sysTime.wMonth = p->tm_mon + 1;
    sysTime.wDay = p->tm_mday;
    sysTime.wDayOfWeek = p->tm_wday;
    sysTime.wHour = p->tm_hour;
    sysTime.wMinute = p->tm_min;
    sysTime.wSecond = p->tm_sec;
    sysTime.wMilliseconds = ts.tv_nsec / 1000000;

    return sysTime;
}

DATETIME TimeWrapper::systemTimeToDateTime(SYSTEMTIME sysTime)
{
    //SystemTimeToDatetime
    DATETIME dateTime = 0;
    dateTime += systemTimeToUnixTime(sysTime);
    dateTime = dateTime + double(sysTime.wMilliseconds)/1000.0;

    return dateTime;
}

DATETIME TimeWrapper::systemtimeToDelphiDateTime(SYSTEMTIME sysTime)
{
    time_t tmt = systemTimeToUnixTime(sysTime);
    DATETIME result = (double)((tmt + sysTime.wMilliseconds / 1000.0) / DAY_SECONDS) + UNIX_START_DELPHI_DATETIME;

    return result;
}

time_t TimeWrapper::systemTimeToUnixTime(SYSTEMTIME sysTime)
{
    int mon[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
    int sign = 0;
    int temp = sysTime.wYear;
    if ((temp < 1970) || (temp > 2100))
    {
        return false;
    }
    temp -= 1900;
    unsigned long ulTime = (temp - 70) * YEAR_SECONDS;
    int tp = (temp - 69) / 4;
    ulTime = ulTime + DAY_SECONDS * tp;
    if ( ((temp % 4) == 0) && (((sysTime.wYear % 100) != 0) || ((sysTime.wYear % 400) == 0)) )
    {
        sign = 1;
    }
    for ( int i = 1; i < sysTime.wMonth; i++ )
    {
        ulTime += mon[sign][i - 1] * DAY_SECONDS;
    }
    ulTime += (unsigned long)(sysTime.wDay - 1) * DAY_SECONDS;
    ulTime += (unsigned long)(sysTime.wHour) * 3600;
    ulTime += (unsigned long)(sysTime.wMinute) * 60;
    ulTime += (unsigned long)(sysTime.wSecond);

    return (time_t) ulTime;
}

SYSTEMTIME TimeWrapper::delphiDateTimeToSystemTime(DATETIME dtTime)
{
    DATETIME dt = (double)(dtTime - UNIX_START_DELPHI_DATETIME) * DAY_SECONDS;

    return dateTimeToSystemTime(dt);
}

SYSTEMTIME TimeWrapper::dateTimeToSystemTime(DATETIME dtTime)
{
    SYSTEMTIME sysTime;
    memset(&sysTime, 0, sizeof(sysTime));

    long  x0, x1, y, z;
    int i=0;
    int m[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    double dttime;

    dttime=dtTime;

    x0 = (long)(dttime / FOUR_YEAR_SECONDS);
    y = (long)((unsigned long long)dttime % FOUR_YEAR_SECONDS);

    do
    {
        y-=YEAR_SECONDS;
        if (i == 2) y-=DAY_SECONDS;
        i++;
    }while(y>= 0);

    x1 = i - 1;
    y+=YEAR_SECONDS;

    if (x1 == 2){
        y += DAY_SECONDS;
        m[1] = 29;
    }
    else
        m[1] = 28;
    sysTime.wYear = (unsigned short)(1970 + 4 * x0 + x1);
    z =y/DAY_SECONDS;
    y %= DAY_SECONDS;
    i = 0;
    do
    {
        z-=m[i];
        i++;
    }while(z >= 0);

    sysTime.wMonth = i;

    if (i == 0)
    {
        sysTime.wDay = (unsigned short)z + 1;
        sysTime.wMonth = 1;
    }
    else
        sysTime.wDay = (unsigned short)z + m[i - 1] + 1;
    sysTime.wHour = (unsigned short)(y / 3600);
    y-=sysTime.wHour * 3600;
    sysTime.wMinute = (unsigned short)(y / 60);
    sysTime.wSecond = (unsigned short)y - sysTime.wMinute * 60;
    sysTime.wMilliseconds=(unsigned short)((unsigned long long)(dttime * 1000) % 1000);

    time_t tmt = systemTimeToUnixTime(sysTime);
    struct tm* p;
    p = gmtime(&tmt);

    sysTime.wDayOfWeek = p->tm_wday;

    return sysTime;
}

SYSTEMTIME TimeWrapper::unixTimeToSystemTime(time_t tmt)
{
    DATETIME dtTime = 0;
    dtTime += tmt;

    return dateTimeToSystemTime(dtTime);
}

void TimeWrapper::sleep(unsigned long milliseconds)
{
    struct timespec ts;

    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds%1000) * 1000000;

    nanosleep(&ts, NULL);
}

unsigned long TimeWrapper::getTickCount()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
