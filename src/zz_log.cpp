#include "zz_log.h"
#include "zz_singleton.h"
#include "zz_log_print.h"

namespace zz
{

namespace log
{

static Logger *g_custom_logger = NULL;

void setLogger(Logger *logger)
{
  g_custom_logger = logger;
}

Logger *getLogger()
{
  if (g_custom_logger != NULL)
  {
    return g_custom_logger;
  }

  static SingletonHolder<LoggerPrint> print_logger;
  setLogger(print_logger.get());
}

void setPrintLogger()
{
  static SingletonHolder<LoggerPrint> print_logger;
  setLogger(print_logger.get());
}

void setNullLogger()
{
  static SingletonHolder<Logger> null_logger;
  setLogger(null_logger.get());
}



}


}
