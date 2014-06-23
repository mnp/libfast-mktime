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
    s.tm_isdst = 0;

    time_t result = mktime(&s);

    printf("  %s: got %10d for %s\n", 
	(result == expect ? "okay" : "fail"), 
	result,
	title);
}

int main()
{
    // force UTC
    putenv((char *)"TZ=UTC");
    tzset();

    // basic function checks
    //  s   m   h   D   M    Y      expect   title
    ok(39, 19, 20, 28,  5, 113, 1372450779, "sometime   Fri Jun 28 20:19:39 2013 UTC");
    ok(40, 20, 21, 28,  5, 113, 1372454440, "same day   Fri Jun 28 21:20:40 2013 UTC");
    ok(59, 59, 23, 29,  5, 113, 1372550399, "end of day Sat Jun 29 23:59:59 2013 UTC");
    ok(0,   0,  0, 30,  5, 113, 1372550400, "next day   Sun Jun 30 00:00:00 2013 UTC");
    ok(40, 20, 21, 29,  5, 113, 1372540840, "next day   Sat Jun 29 21:20:40 2013 UTC");
    ok(0,   0,  0,  1,  0,  70,          0, "min epoch  Thu Jan  1 00:00:00 1970 UTC");
    ok(59, 59, 18, 31, 11, 137, 2145898799, "max epoch  Thu Dec 31 18:59:59 2037 UTC");

    return 0;
}
