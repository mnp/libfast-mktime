/*
 * Copyright (C) 2014 Mitchell Perilstein
 * Licensed under GNU LGPL Version 3. See LICENSING file for details.
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define __USE_GNU
#include <dlfcn.h>

time_t mktime(struct tm *tm)
{
    static struct tm cache = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    static time_t time_cache = 0;
    static time_t (*mktime_real)(struct tm *tm) = NULL;
    time_t result;
    time_t hmsarg;

    /* Load real mktime once into a static */
    if (!mktime_real)
    {
        void *handle;

        /* To forgive this cast, please see man dlopen(3). */
        dlerror();
        handle = dlsym(RTLD_NEXT, "mktime");
        *(void **) (&mktime_real) = handle;

        if (!mktime_real)
        {
            fprintf(stderr, "loading mktime: %s\n", dlerror());
            exit(EXIT_FAILURE);
        }
    }

    /* the epoch time portion of the request */
    hmsarg = 3600 * tm->tm_hour
            +  60 * tm->tm_min
            +       tm->tm_sec;

    if ( cache.tm_mday    == tm->tm_mday
         && cache.tm_mon  == tm->tm_mon
         && cache.tm_year == tm->tm_year )
    {
        /* cached - just add h,m,s from request to midnight */
        result = time_cache + hmsarg;

        /* Obscure, but documented, return value: only this value in arg struct.         
         * 
         * BUG: dst switchover was computed by mktime_real() for time 00:00:00
         * of arg day. So this return value WILL be wrong for switchover days
         * after the switchover occurs.  There is no clean way to detect this
         * situation in stock glibc.  This bug will be reflected in unit test
         * until fixed.  See also github issues #1 and #2.
         */
        tm->tm_isdst  = cache.tm_isdst;
    }
    else
    {
        /* not cached - recompute midnight on requested day */
        cache.tm_mday = tm->tm_mday;
        cache.tm_mon  = tm->tm_mon;
        cache.tm_year = tm->tm_year;
        time_cache    = mktime_real(&cache);
        tm->tm_isdst  = cache.tm_isdst;

        result = (-1 == time_cache) ? -1 : time_cache + hmsarg;
    }

    return result;
}
