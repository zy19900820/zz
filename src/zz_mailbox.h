#ifndef _ZZ_MAILBOX_H
#define _ZZ_MAILBOX_H

#include <errno.h>
#include <string.h>
#include "zz_baseclass.h"
#include "zz_signaler.h"
#include "zz_pipe.h"
#include "zz_lock.h"

namespace zz
{

class MailBoxEventListener
{
public:
  virtual ~MailBoxEventListener()
  {}

  virtual bool onWait()=0;

  virtual void onActive()=0;
};

template<class T, int N=16>
class MailBox
{
public:
  MailBox() : active_(true), created_(false), event_listener_(NULL)
  {}

  ~MailBox()
  {}

  void setEventListener(MailBoxEventListener *event_listener)
  {
    event_listener_ = event_listener;
  }

  void send(const T& msg)
  {
    pipe_.write(msg);
    if (pipe_.flush())
    {
      signaler_.notify();
    }
  }

  bool recv(T& msg, int32_t timeout_ms)
  {
    if (active_)
    {
      if (pipe_.read(msg))
      {
        return true;
      }
      active_ = false;
      if (event_listener_)
      {
        if (!event_listener_->onWait())
        {
          return false;
        }
      }
    }

    if (!signaler_.wait(timeout_ms))
    {
      return false;
    }

    if (event_listener_)
    {
      event_listener_->onActive();
    }

    active_ = true;
    if (pipe_.read(msg))
    {
      return true;
    }
    LOG(FATAL_LOGGER, 1,
        "mailbox recv msg fail!signaler is notify, but msg is empty![errno:%d, errstring:%s]",
        errno, strerror(errno));

    return false;
  }

protected:
  ZZPipe<T, N> pipe_;
  Signaler signaler_;
  bool active_;
  bool created_;
  MailBoxEventListener *event_listener_;
};

template<class T, int N=16>
class MailBoxR : public MailBox<T, N>
{
public:
  MailBoxR() : MailBox<T, N>()
  {}

  ~MailBoxR()
  {}

  void send(const T& msg)
  {
    bool ok = true;
    {
      ScopeLock scop_lock(send_lock_);
      this->pipe_.write(msg);
      ok = this->pipe_.flush();
    }
    if (ok)
    {
      this->signaler_.notify();
    }
  }

protected:
  MutexLock send_lock_;
};

}

#endif // _ZZ_MAILBOX_H
