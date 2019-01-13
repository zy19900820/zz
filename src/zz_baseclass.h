#ifndef _ZZ_BASECLASS_H
#define _ZZ_BASECLASS_H

#include "zz_threadlocal.h"

#define COPY_ERROR_MSG(obj) this->_CopyErrorMsg(__FILE__, __LINE__, obj)
#define SET_ERROR_MSG(...) \
      do { LOG(FATAL_LOGGER, 1,  __VA_ARGS__); \
      this->_SetErrorMsg(__FILE__, __LINE__, FATAL_LOGGER, __VA_ARGS__);}while(0);

#define LOG_STACK_TRACE(obj) \
    do { LOG(FATAL_LOGGER, 1, "%s", (obj)->GetStackTrace().c_str());}while(0);

namespace zz
{

class NonCopyAble
{
protected:
  NonCopyAble()
  {}

  virtual ~NonCopyAble()
  {}

private:
  NonCopyAble(const NonCopyAble&)
  {}

  const NonCopyAble& operator=(const NonCopyAble&)
  {}
};

class ErrorMsgData
{
  std::string error_msg_;
  list<std::string> stack_trace_;
  friend class SupportErrorMsg;
};

class ErrorMsgStore
{
public:
  virtual ~ErrorMsgStore()
  {}

  virtual ErrorMsgData *getErrorMsgData()=0;
};

class ErrorMsgLocalStore : public ErrorMsgStore
{
public:
  ErrorMsgData *getErrorMsgData()
  {
    return &m_cErrorMsgData;
  }
private:
  ErrorMsgData m_cErrorMsgData;
};

class ErrorMsgThreadStore : public ErrorMsgStore
{
public:
  ErrorMsgData *getErrorMsgData()
  {
    return m_cErrorMsgData.get();
  }
private:
  ThreadLocal<ErrorMsgData> m_cErrorMsgData;
};

class SupportErrorMsg : public NonCopyAble
{
public:
  SupportErrorMsg();

  explicit SupportErrorMsg(bool use_thread_store);

  ~SupportErrorMsg();
 
  string getErrorMsg();

  void clearErrorMsg();

  // 获取异常堆栈
  string getStackTrace();
  
  ErrorMsgData *getErrorMsgData();

  string _SetErrorMsg(const char *file_name, int32_t line, int32_t log_level,
                    const char *format, ...);

  string _CopyErrorMsg(const char *file_name, int32_t log_level, SupportErrorMsg *support_error_msg);

  string _CopyErrorMsg(const char *file_name, int32_t log_level, SupportErrorMsg& support_error_msg);

private:
  ErrorMsgStore *error_msg_store_;
};

}

#endif // _ZZ_BASECLASS_H
