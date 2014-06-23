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
    }
    else
    {
        /* not cached - recompute midnight on requested day */
        cache.tm_mday = tm->tm_mday;
        cache.tm_mon  = tm->tm_mon;
        cache.tm_year = tm->tm_year;
        time_cache    = mktime_real(&cache);

        result = (-1 == time_cache) ? -1 : time_cache + hmsarg;
    }

    return result;
}
