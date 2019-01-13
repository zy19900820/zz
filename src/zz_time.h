#ifndef _ZZ_TIME_H
#define _ZZ_TIME_H

#include <unistd.h>
#include <stdint.h>

namespace zz
{

namespace time
{

void sleepBySec(int32_t sec);

void sleepByMs(int32_t ms);

void sleepByUs(int32_t us);

// 获取当前系统时间 s
time_t getCurrentSec();

int64_t getCurrentMs();

int64_t getCurrentUs();

std::string getTimeString(time_t tm = getCurrentSec(), const char *format = "20%y-%m-%d %H:%M:%S");

}

}

#endif // _ZZ_TIME_H
