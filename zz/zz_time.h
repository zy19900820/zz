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


}

}

#endif // _ZZ_TIME_H
