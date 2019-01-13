#ifndef _ZZ_LOG_H
#define _ZZ_LOG_H

#include <stdint.h>


#define FATAL_LOGGER 6
#define ERROR_LOGGER 5
#define WARN_LOGGER  4
#define INFO_LOGGER  3
#define DEBUG_LOGGER 2
#define TRACE_LOGGER 1

#define LOG(level, err_num, ...) \
  do \
  { \
    if ( (level) >= zz::log::getLogger()->getLogLevel() ) \
    { \
      zz::log::getLogger()->log(level, \
      __FILENAME__, __FUNCTION__, __LINE__, err_num, __VA_ARGS__); \
    } \
  } while(0)

#define LOG_TRACE(...) LOG(TRACE_LOGGER, 0, __VA_ARGS__)
#define LOG_DEBUG(...) LOG(DEBUG_LOGGER, 0, __VA_ARGS__)
#define LOG_INFO(...) LOG(INFO_LOGGER,   0, __VA_ARGS__)
#define LOG_WARN(errnum, ...) LOG(WARN_LOGGER,   (errnum), __VA_ARGS__)
#define LOG_ERROR(errnum, ...) LOG(ERROR_LOGGER, (errnum), __VA_ARGS__)
#define LOG_FATAL(errnum, ...) LOG(FATAL_LOGGER, (errnum), __VA_ARGS__)

namespace zz
{

namespace log
{

// 基类不打印日志 只有日志级别
class Logger
{
public:
  Logger() : level_(TRACE_LOGGER)
  {}

  virtual ~Logger()
  {}

  void setLogLevel(int32_t level)
  {
    level_ = level;
  }

  int32_t getLogLevel()
  {
    return level_;
  }

  /*
   * @param level       级别
   * @param file_name   文件名
   * @param func_name   函数名
   * @param file_line   文件行号
   * @param err_num     错误码
   * @param format      格式
   *
   */
  virtual void log(int32_t level, const char *file_name, const char *func_name,
                int32_t file_line, int err_num, const char *format, ...)
  {
  }
private:
  int32_t level_;
};

Logger *getLogger();

void setLogger(Logger *logger);

// 屏幕打印日志
void setPrintLogger();

void setNullLogger();

// 文件日志
//int setFileLogger(const char *filename, bool direct_io=false);

}

}

#endif // _ZZ_LOG_H
