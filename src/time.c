#include <pelib/time.h>

#ifdef __cplusplus
extern "C" {
#endif

const long long int pelib_nsec_in_sec = 1000000000;

int
pelib_timespec_subtract(struct timespec *result, struct timespec *x, struct timespec *y)
{
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_nsec < y->tv_nsec)
	{
		int nsec = (y->tv_nsec - x->tv_nsec) / pelib_nsec_in_sec + 1;
		y->tv_nsec -= pelib_nsec_in_sec * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_nsec - y->tv_nsec > pelib_nsec_in_sec)
	{
		int nsec = (x->tv_nsec - y->tv_nsec) / pelib_nsec_in_sec;
		y->tv_nsec += pelib_nsec_in_sec * nsec;
		y->tv_sec -= nsec;
	}
     
	/* Compute the time remaining to wait. tv_nsec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_nsec = x->tv_nsec - y->tv_nsec;
     
	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}

#ifdef __cplusplus
}
#endif

