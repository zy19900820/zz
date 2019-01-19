#ifndef _ZZ_BASEDEF_H
#define _ZZ_BASEDEF_H

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <list>
#include <map>
#include <vector>

using namespace std;

#if defined(__GNUC__) && __GNUC__ >= 3
#define ZZ_UNLIKELY(p) __builtin_expect(!!(p),0)
#define ZZ_LIKELY(p) __builtin_expect(!!(p),1)
#else
#define ZZ_UNLIKELY(p) (p)
#define ZZ_LIKELY(p) (p)
#endif

#endif // _ZZ_BASEDEF_H
