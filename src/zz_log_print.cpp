#include "zz_log_print.h"
#include <sys/time.h>

namespace zz
{

namespace log
{

std::string LoggerPrint::getTimeStr() const 
{
  struct timeval tv;
  gettimeofday(&tv, NULL);

  struct tm t;
  struct tm *pt = localtime_r(&tv.tv_sec, &t);

  char buf[64];
  snprintf(buf, 64, "%04d-%02d-%02d %02d:%02d:%02d.%06d", t.tm_year + 1900,
        t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, tv.tv_usec);

  return std::string(buf);
}

void LoggerPrint::log(int32_t level, const char *file_name, const char *func_name,
                      int32_t file_line, int err_num, const char *format, ...)
{
  time_t tm = ::time(NULL);
  char timebuf[64];

  printf("[%s] (%s:%d) (%d:0x%lx) ", getTimeStr().c_str(),
        file_name, file_line, getpid(), (unsigned long int) pthread_self());

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  printf("\n");
}

}

}
