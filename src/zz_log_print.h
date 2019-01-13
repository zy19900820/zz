#ifndef _ZZ_LOG_PRINT_H
#define _ZZ_LOG_PRINT_H

#include "zz_log.h"
#include "zz_basedef.h"

namespace zz
{

namespace log
{

class LoggerPrint : public Logger
{
public:
  ~LoggerPrint()
  {}

  virtual void log(int32_t level, const char *file_name, const char *func_name,
                  int32_t file_line, int err_num, const char *format, ...);

protected:
  std::string getTimeStr() const;
};

}

}

#endif // _ZZ_LOG_PRINT_H
