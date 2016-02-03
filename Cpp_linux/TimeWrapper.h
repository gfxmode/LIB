/** TimeWrapper namespace for Linux
 * author: gfxmode@live.com
 * date: 2015-12-27
 * version: 1.0.0.0
 */

#ifndef TIMEWRAPPER_H
#define TIMEWRAPPER_H

#include <string.h>
#include <stdint.h>
#include <time.h>

#include "GlobalDefine.h"

// UNIX StartTime to Delphi DateTime
const double UNIX_START_DELPHI_DATETIME = 25569.0;
// Seconds in a Day
const unsigned long DAY_SECONDS = 86400;
// Seconds in a Year
const unsigned long YEAR_SECONDS = 86400 * 365;
// Seconds in 4 Years
const unsigned long FOUR_YEAR_SECONDS = 86400 * 365 * 4 + 86400;

typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef double DATETIME;
typedef double DELPHIDATETIME;      // Delphi TDateTime
typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME;

namespace TimeWrapper
{
    enum TIME_OPTIONS
    {
        LOCAL_TIME,
        SYSTEM_TIME
    };
    // get now time in SYSTEMTIME format with user specified timezone
    SYSTEMTIME getLocalTime();
    SYSTEMTIME getSystemTime();

    // Convert systemTime to/from DateTime. The convertion may cause 0.001ms deviation
    DATETIME systemTimeToDateTime(SYSTEMTIME sysTime);
    SYSTEMTIME dateTimeToSystemTime(DATETIME dtTime);

    // convert systemTime to/from Delphi TDateTime
    DELPHIDATETIME systemtimeToDelphiDateTime(SYSTEMTIME sysTime);
    SYSTEMTIME delphiDateTimeToSystemTime(DELPHIDATETIME dtTime);

    // convert UnixTime to/from Systemtime
    time_t systemTimeToUnixTime(SYSTEMTIME sysTime);
    SYSTEMTIME unixTimeToSystemTime(time_t tmt);

    SYSTEMTIME _getSysTime(TIME_OPTIONS tmOp);

    void sleep(unsigned long milliseconds);
    unsigned long getTickCount();
}


#endif // TIMEWRAPPER_H
