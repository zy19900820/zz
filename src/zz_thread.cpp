#include <errno.h>
#include <string.h>
#include "zz_thread.h"

namespace zz
{

static void *workThread(void *data)
{
  Thread *thread = (Thread *) data;

  if (thread != NULL)
  {
    thread->fireThreadCreateEvent();
    thread->run();
    thread->fireThreadExitEvent();
  }

  return NULL;
}

Runnable::Runnable() : ready_stop_(false)
{
}

void Runnable::notifyStop()
{
  ready_stop_ = true;
}

void Runnable::reset()
{
  ready_stop_ = false;
}

bool Runnable::isReadyStop()
{
  return ready_stop_;
}

Thread::Thread() : target_(NULL), thread_event_listener_(NULL),
                  tid_(-1), delete_target_(false)
{
}

Thread::Thread(Runnable *target) : target_(target), thread_event_listener_(NULL),
                  tid_(-1), delete_target_(false)
{
}

void Thread::setThreadEventListener(ThreadEventListener *thread_event_listener)
{
  thread_event_listener_ = thread_event_listener;
}

void Thread::setTarget(Runnable *target, bool delete_target)
{
  target_ = target;
  delete_target_ = delete_target;
}

void Thread::fireThreadCreateEvent()
{
  if (thread_event_listener_)
  {
    thread_event_listener_->onThreadCreate(this);
  }
}

void Thread::fireThreadExitEvent()
{
  if (thread_event_listener_)
  {
    thread_event_listener_->onThreadExit(this);
  }
}

bool Thread::start()
{
  if (tid_ != -1)
  {
    return true;
  }
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  const int32_t THREAD_STACK_SIZE = 1024 * 1024 * 2;
  pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);
  int ret = pthread_create(&(tid_), &attr, workThread, (void *) this);
  pthread_attr_destroy(&attr);
  if (ret != 0)
  {
    LOG_FATAL(-1, "thread create fail![ret:%d] reason:%s num:%d", ret, strerror(errno), errno);
    return false;
  }
  return true;
}

void Thread::run()
{
  if (target_ != NULL)
  {
    target_->reset();
    target_->run();
    if (delete_target_)
    {
      Runnable *target = target_;
      target_ = NULL;
      delete target;
    }
  }
}

void Thread::notifyStop()
{
  if (tid_ == -1)
    return ;

  if (target_ != NULL)
  {
    target_->notifyStop();
  }
}

void Thread::join(bool isNotifyStop)
{
  if (isNotifyStop)
  {
    notifyStop();
  }
  if (tid_ != -1)
  {
    pthread_join(tid_, NULL);
  }
  tid_ = -1;
}

}
