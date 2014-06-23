#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

void usage()
{
    fprintf(stderr, "Usage: benchmark TRIALS ITERATIONS\n");
    exit(1);
}

struct tm * makeit(long ndates)
{
    struct tm * date_array = (struct tm *)malloc(ndates * sizeof(struct tm));
    if (!date_array)
    {
	fprintf(stderr, "Too much malloc\n");
	exit(1);
    }

    long s;
    for (s=0; s<ndates; s++)
    {
        time_t t = s + 1;	// start at year 1
        gmtime_r(&t, &date_array[s]);
    }

    return date_array;
}

float timeit(struct tm *dates, long ndates)
{
    struct timeval tbeg, tend, tres;
    gettimeofday(&tbeg, NULL);

    long s;
    for (s=0; s<ndates; s++)
    {
        time_t made = mktime(&dates[s]);      
    }

    gettimeofday(&tend, NULL);
    return ((tend.tv_sec * 1e6 + tend.tv_usec)
	- (tbeg.tv_sec * 1e6 + tbeg.tv_usec)) / 1e6;
}

int main(int argc, char **argv) 
{
    if (argc < 3) 
	usage();

    long trials = atol(argv[1]);
    long ndates = atol(argv[2]);

    if (ndates<1 || trials<1)
	usage();

    struct tm *dates = makeit(ndates);
    float total      = 0.0;
    long i;

    for (i=0; i<trials; i++)
    {
        float secs = timeit(dates, ndates);
	printf("%3d: %f s\n", (int)i, secs);
        total += secs;
    }

    printf("avg: %f s\n", total/trials);

    exit(0);
}
