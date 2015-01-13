/*
 * Copyright (C) 2014 Mitchell Perilstein
 * Licensed under GNU LGPL Version 3. See LICENSING file for details.
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>

void ok(int sec, int min, int hour, int day, int month, int year,
        time_t expect, const char *title)
{
    struct tm s;
    s.tm_sec = sec;
    s.tm_min = min;
    s.tm_hour = hour;
    s.tm_mday = day;
    s.tm_mon = month;
    s.tm_year = year;
    s.tm_isdst = -1;

    time_t result = mktime(&s);

    printf("  %s: got %10lu expected %10lu (isdst %d) --  %s\n",
           (result == expect ? "okay" : "fail"),
           (unsigned long)result,
           (unsigned long)expect,
           s.tm_isdst,
           title);
}

int main()
{
    // force UTC
    putenv((char *)"TZ=UTC");
    tzset();

    // basic function checks for UTC
    //  s   m   h   D   M    Y      expect   title
    ok(39, 19, 20, 28,  5, 113, 1372450779, "sometime   Fri Jun 28 20:19:39 2013 UTC");
    ok(40, 20, 21, 28,  5, 113, 1372454440, "same day   Fri Jun 28 21:20:40 2013 UTC");
    ok(59, 59, 23, 29,  5, 113, 1372550399, "end of day Sat Jun 29 23:59:59 2013 UTC");
    ok(0,   0,  0, 30,  5, 113, 1372550400, "next day   Sun Jun 30 00:00:00 2013 UTC");
    ok(40, 20, 21, 29,  5, 113, 1372540840, "next day   Sat Jun 29 21:20:40 2013 UTC");
    ok(0,   0,  0,  1,  0,  70,          0, "min epoch  Thu Jan  1 00:00:00 1970 UTC");
    ok(59, 59, 18, 31, 11, 137, 2145898799, "max epoch  Thu Dec 31 18:59:59 2037 UTC");

    // force EDT/EST
    putenv((char *)"TZ=US/Eastern");
    tzset();

    // dst switchover day
    // zdump -v -c 2012,2013 US/Eastern
    // US/Eastern  Sun Mar 11 06:59:59 2012 UT = Sun Mar 11 01:59:59 2012 EST isdst=0 gmtoff=-18000
    // US/Eastern  Sun Mar 11 07:00:00 2012 UT = Sun Mar 11 03:00:00 2012 EDT isdst=1 gmtoff=-14400
    // US/Eastern  Sun Nov  4 05:59:59 2012 UT = Sun Nov  4 01:59:59 2012 EDT isdst=1 gmtoff=-14400
    // US/Eastern  Sun Nov  4 06:00:00 2012 UT = Sun Nov  4 01:00:00 2012 EST isdst=0 gmtoff=-18000
    //  s   m   h   D   M    Y      expect   title
    ok(59, 59, 01, 11, 02, 112,    1331449199, "March 2012 dst before"); 
    ok(00, 00, 03, 11, 02, 112,    1331449200, "March 2012 dst after"); 
    ok(59, 59, 01, 04, 10, 112,    1352008799, "Nov   2012 dst before"); 
    ok(00, 00, 01, 04, 10, 112,    1352005200, "Nov   2012 dst after"); 

    return 0;
}
