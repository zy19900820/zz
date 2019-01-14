#ifndef _ZZ_THREAD_H
#define _ZZ_THREAD_H

#include "zz_baseclass.h"

namespace zz
{

/*
 * @code
 * class Task : public Runnable
 * {
 * public:
 *  void run()
 *  {
 *    printf("hello world\n");
 *  }
 * };
 *
 * Task task;
 * Thread t1(&task);
 * t1.start();
 * t1.join();
 *
 */

class Runnable
{
public:
  Runnable();

  virtual ~Runnable()
  {}

  virtual void run()=0;
  
  void notifyStop();

  void reset();

  bool isReadyStop();

protected:
  volatile bool ready_stop_;
};

class Thread;
// 线程事件监听
class ThreadEventListener
{
public:
  virtual ~ThreadEventListener()
  {}

  virtual void onThreadCreate(Thread *pThread)=0;

  virtual void onThreadExit(Thread *pThread)=0;
};

class Thread : public Runnable
{
public:
  Thread();

  explicit Thread(Runnable *pTarget);

  // @param isDeleteTarget 线程结束是否删除任务
  void setTarget(Runnable *pTarget, bool isDeleteTarget=false);

  void setThreadEventListener(ThreadEventListener *pThreadEventListener);

  // 运行任务
  virtual void run();

  // 启动线程
  bool start();

  virtual void notifyStop();

  void join(bool isNotifyStop=false);

  pthread_t getTid()
  {
    return tid_;
  }
  
  void setDeleteTarget(bool isDeleteTarget)
  {
    delete_target_ = isDeleteTarget;
  }

  void fireThreadCreateEvent();

  void fireThreadExitEvent();

protected:
  Runnable *target_;
  ThreadEventListener *thread_event_listener_;

private:
  pthread_t tid_;
  bool delete_target_;
};

}

#endif // _ZZ_THREAD_H
