#ifndef _ZZ_STRING_H
#define _ZZ_STRING_H

#include "zz_basedef.h"

namespace zz
{

string itoa(int32_t i);

string itoa(int64_t i);

// 删除头尾的空格，回车
string trim(const string& src);

string toUpperCase(const string& src);

string toLowerCase(const string& src);

string format(const char *format, ...);

}

#endif // _ZZ_STRING_H
