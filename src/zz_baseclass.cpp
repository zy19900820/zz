#include "zz_baseclass.h"
#include "zz_string.h"

namespace zz
{

SupportErrorMsg::SupportErrorMsg()
{
  error_msg_store_ = new ErrorMsgThreadStore();
}

SupportErrorMsg::SupportErrorMsg(bool use_thread_store)
{
  if (use_thread_store)
  {
    error_msg_store_ = new ErrorMsgThreadStore();
  }
  else
  {
    error_msg_store_ = new ErrorMsgLocalStore();
  }
}

SupportErrorMsg::~SupportErrorMsg()
{
  if (error_msg_store_)
  {
    delete error_msg_store_;
    error_msg_store_ = NULL;
  }
}

ErrorMsgData *SupportErrorMsg::getErrorMsgData()
{
  return error_msg_store_->getErrorMsgData();
}

string SupportErrorMsg::getErrorMsg()
{
  return getErrorMsgData()->error_msg_;
}

void SupportErrorMsg::clearErrorMsg()
{
  getErrorMsgData()->stack_trace_.clear();
  getErrorMsgData()->error_msg_.clear();
}

string SupportErrorMsg::getStackTrace()
{
  string ret = "";
  list<string>::iterator it = getErrorMsgData()->stack_trace_.begin();
  for (; it != getErrorMsgData()->stack_trace_.end(); it++)
  {
    ret.append(*it);
    ret.append("\r\n");
  }
  ret.append("\r\n");
  ret.append(getErrorMsg());
  return ret;
}

string SupportErrorMsg::_SetErrorMsg(const char *file_name, int32_t line,
                                  int32_t log_level, const char *format, ...)
{
  getErrorMsgData()->stack_trace_.clear();
  getErrorMsgData()->stack_trace_.push_back(zz::format("%s:%d", file_name, line));
  
  if (format != NULL)
  {
    char buf[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    buf[sizeof(buf) - 1] = 0;
    va_end(args);
    getErrorMsgData()->error_msg_ = buf;
  }
  return getErrorMsgData()->error_msg_;
}

string SupportErrorMsg::_CopyErrorMsg(const char *file_name, int32_t line, SupportErrorMsg *support_error_msg)
{
  getErrorMsgData()->stack_trace_ = support_error_msg->getErrorMsgData()->stack_trace_;
  getErrorMsgData()->stack_trace_.push_back(zz::format("%s:%d", file_name, line));
  getErrorMsgData()->error_msg_ = support_error_msg->getErrorMsgData()->error_msg_;
  return getErrorMsgData()->error_msg_;
}


string SupportErrorMsg::_CopyErrorMsg(const char *file_name, int32_t line, SupportErrorMsg& support_error_msg)
{
  return _CopyErrorMsg(file_name, line, &support_error_msg);
}

}
