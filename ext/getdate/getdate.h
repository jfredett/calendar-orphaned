#include <sys/types.h>
#include <time.h>
#ifdef HAVE_SYS_TIMES_H
#include <sys/times.h>
#endif
time_t get_date(const char *p, const time_t *now);
