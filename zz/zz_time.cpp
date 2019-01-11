#include <time.h>
#include <sys/time.h>
#include <string>
#include <stdio.h>
#include "zz_time.h"

namespace zz
{

namespace time
{

void sleepBySec(int32_t sec)
{ 
  sleep(sec);
}

void sleepByMs(int32_t ms)
{
  struct timespec time;
  time.tv_sec = ms / 1000;
  time.tv_nsec = (ms % 1000) * 1000000;
  nanosleep(&time, 0);
}

void sleepByUs(int32_t us)
{
  struct timespec time;
  time.tv_sec = us / 1000000;
  time.tv_nsec = us % 1000000;
  nanosleep(&time, 0);
}

time_t getCurrentSec()
{
  return ::time(NULL);
}

int64_t getCurrentMs()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int64_t getCurrentUs()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

std::string getTimeString(time_t tm, const char *pFormat)
{
  char date[48] = {};
  if (tm <= 0)
  {
    snprintf(date, sizeof(date), "0000-00-00 00:00:00");
  }
  else
  {
    struct tm tmp;
    localtime_r(&tm, &tmp);
    strftime(date, 50, pFormat, &tmp);
  }
  return std::string(date);
}

}

}
