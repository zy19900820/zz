#include <time.h>
#include <sys/time.h>
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

}

}
