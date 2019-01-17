#ifndef _ZZ_THREADPOOL_H
#define _ZZ_THREADPOOL_H

#include "zz_thread.h"
#include "zz_mailbox.h"

namespace zz
{

class Task
{
public:
  Task() : task_(NULL), finish_delete_(false), thread_exit_task_(false)
  {}

public:
  Runnable *task_;      
  bool finish_delete_;   
  bool thread_exit_task_;    // 是否退出
};

// 任务调度线程负责写 工作线程负责读
typedef MailBox<Task> TaskMailBox;

// MailBoxR 内部加锁,可以多线程同时写入
typedef MailBoxR<Task> TaskMailBoxR;

// 工作线程没有任务的时候 
// 将自己信箱信息放入里面
// 方便调度线程分配任务
typedef MailBoxR<TaskMailBox *> WaitMailBox;

typedef vector<TaskMailBox *> TaskMailBoxList;

class WorkThread : public Thread, public MailBoxEventListener
{
public:
  WorkThread(TaskMailBox *task_mailbox, WaitMailBox *wait_mailbox);

  virtual void run();

  virtual bool onWait();

  virtual void onActive();

protected:
  TaskMailBox *task_mailbox_;
  WaitMailBox *wait_mailbox_;
};

typedef vector<WorkThread *> WorkThreadList;

class ThreadPool;

class TaskDispatchThread : public Thread
{
public:
  explicit TaskDispatchThread(ThreadPool *thread_pool);

  virtual void run();

protected:
  ThreadPool *thread_pool_;
};

class ThreadPool : public SupportErrorMsg
{
public:
  ThreadPool();

  explicit ThreadPool(int32_t thread_num);

  ~ThreadPool();

  void start();

  void setThreadEventListener(ThreadEventListener *thread_event_listener);

  bool processRequest(Runnable *request, bool finish_delete=true);

  void notifyStop();

  void join();

  // 只允许增加
  bool resizeWorkThreadNum(int32_t thread_num);

  int32_t getCurWorkThreadNum();

  static ThreadPool *getDefaultThreadPool();

protected:
  int32_t init_work_thread_num_;
  int32_t work_thread_num_;
  TaskMailBoxR input_mailbox_;
  WaitMailBox wait_mailbox_;
  TaskMailBoxList task_mailbox_;
  WorkThreadList work_thread_;
  TaskDispatchThread task_dispatch_thread_;
  ThreadEventListener *thread_event_listener_;
  MutexLock lock_;
  volatile bool ready_stop_;
  friend class TaskDispatchThread;
};

}

#endif // _ZZ_THREADPOOL_H
