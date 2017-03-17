#include <time/time.h>
#include <qcom/base.h>
#include <qcom_system.h>
#include <qcom/timetype.h>
#include <qcom_time.h>
#include <qcom_sntp.h>
#include <qcom_utils.h>
#include <threadx/tx_api.h>
#include <debug.h>
#include <config.h>

//workaround
static time_t current_time = 0;
extern QCOM_TIME_API_INDIRECTION_TABLE qcomTimeApiIndirectionTable;
#undef time
time_t time(time_t *t) {
  time_t tmp = qcomTimeApiIndirectionTable._time(NULL) + current_time;
  if ( t ) *t = tmp;
  return tmp;
}
//

struct tm* gmtime(const time_t* timer)
{
    #define YEAR0          1900
    #define EPOCH_YEAR     1970
    #define SECS_DAY       (24L * 60L * 60L)
    #define LEAPYEAR(year) (!((year) % 4) && (((year) % 100) || !((year) %400)))
    #define YEARSIZE(year) (LEAPYEAR(year) ? 366 : 365)

    static const int _ytab[2][12] =
    {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };

    static struct tm st_time;
    struct tm* ret = &st_time;
    time_t secs = *timer;
    unsigned long dayclock, dayno;
    int year = EPOCH_YEAR;

    dayclock = (unsigned long)secs % SECS_DAY;
    dayno    = (unsigned long)secs / SECS_DAY;

    ret->tm_sec  = (int) dayclock % 60;
    ret->tm_min  = (int)(dayclock % 3600) / 60;
    ret->tm_hour = (int) dayclock / 3600;
    ret->tm_wday = (int) (dayno + 4) % 7;        /* day 0 a Thursday */

    while(dayno >= (unsigned long)YEARSIZE(year)) {
        dayno -= YEARSIZE(year);
        year++;
    }

    ret->tm_year = year - YEAR0;
    ret->tm_yday = (int)dayno;
    ret->tm_mon  = 0;

    while(dayno >= (unsigned long)_ytab[LEAPYEAR(year)][ret->tm_mon]) {
        dayno -= _ytab[LEAPYEAR(year)][ret->tm_mon];
        ret->tm_mon++;
    }

    ret->tm_mday  = (int)++dayno;
    ret->tm_isdst = 0;

    return ret;
}

int gettimeofday(struct timeval *tv, struct timezone *tz) {
  SSP_PARAMETER_NOT_USED(tz);
  tv->tv_sec = time(NULL);
  tv->tv_usec = ( tx_time_get() % XT_TICK_PER_SEC );
  return 0;
}

char *
strptime (const char *buf, const char *format, struct tm *tm){
  SSP_PARAMETER_NOT_USED(format);
  SSP_PARAMETER_NOT_USED(tm);
  A_PRINTF("%s\n", buf);
  return NULL;
}

time_t mktime(struct tm *timeptr) {
  SSP_PARAMETER_NOT_USED(timeptr);
  time_t t;
  return t;
}

int stime(time_t *t) {
  DBG("set time %d", (int)*t);
//  tRtcTime newt;
  current_time = *t;
//  struct tm *tmt = gmtime(t);
//  newt.hour = tmt->tm_hour;
//  newt.min = tmt->tm_min;
//  newt.Sec = tmt->tm_sec;
//  newt.year = tmt->tm_year;
//  newt.yday = tmt->tm_yday;
//  newt.wday = tmt->tm_wday;
//  DBG("rtc time %d %d %d", newt.year, newt.hour, newt.min);
//  qcom_set_time(newt); // doesn't work!
  return 0;
}
#include <arrow/mem.h>
void get_time(char *ts) {
  struct tm *tmp;
  int ms;
  time_t s = time(NULL);

  tSntpTM tv;
//  qcom_sntp_get_time_of_day(0, &tv);

  tmp = gmtime(&s);
  ms = (tv.tv_usec/1000)%1000;
  sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", 1900+tmp->tm_year, tmp->tm_mon+1, tmp->tm_mday,
          tmp->tm_hour, tmp->tm_min, tmp->tm_sec, ms);
  ts[24] = 0;
  DBG("timestamp: %s", ts);
//  printf("ts: %s\r\n", ts);
}
