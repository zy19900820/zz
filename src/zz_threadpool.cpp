#include "zz_threadpool.h"
#include "zz_singleton.h"

namespace zz
{

WorkThread::WorkThread(TaskMailBox *task_mailbox, WaitMailBox *wait_mailbox)
    : task_mailbox_(task_mailbox), wait_mailbox_(wait_mailbox)
{
  task_mailbox_->setEventListener(this);
}

void WorkThread::run()
{
  LOG(INFO_LOGGER, 0, "thread pool work thread start!");
  while (1)
  {
    Task task;
    if (task_mailbox_->recv(task, -1))
    {
      if (task.thread_exit_task_)
      {
        break;
      }
      if (task.task_)
      {
        task.task_->run();
        if (task.finish_delete_)
        {
          delete task.task_;
        }
      }
    }
    else
    {
      LOG(FATAL_LOGGER, 1, "get task form mail box fail!!!!");
      break;
    }
  }
  LOG(INFO_LOGGER, 0, "thread pool work thread exit!");
}

bool WorkThread::onWait()
{
  wait_mailbox_->send(task_mailbox_);
  return true;
}

void WorkThread::onActive()
{}

ThreadPool::ThreadPool() : init_work_thread_num_(0),
                            work_thread_num_(0),
                            task_dispatch_thread_(this),
                            thread_event_listener_(NULL),
                            ready_stop_(false)
{}

ThreadPool::ThreadPool(int32_t thread_num) : init_work_thread_num_(thread_num),
                                             work_thread_num_(0),
                                             task_dispatch_thread_(this),
                                             thread_event_listener_(NULL),
                                             ready_stop_(false)
{}

ThreadPool::~ThreadPool()
{}

void ThreadPool::start()
{
  resizeWorkThreadNum(init_work_thread_num_);
}

bool ThreadPool::processRequest(Runnable *request, bool finish_delete)
{
  if (ready_stop_)
  {
    SET_ERROR_MSG("thread pool is shutdown!");
    return false;
  }
  Task task;
  task.finish_delete_ = finish_delete;
  task.task_ = request;
  input_mailbox_.send(task);
  return true;
}

void ThreadPool::notifyStop()
{
  ready_stop_ = true;
  if (!ready_stop_)
  {
    ready_stop_ = true;
    Task task;
    task.thread_exit_task_ = true;
    for (int32_t i = 0; i < work_thread_num_; i++)
    {
      task_mailbox_[i]->send(task);
    }
  }
}

void ThreadPool::join()
{
  notifyStop();

  task_dispatch_thread_.join();

  Task task;
  task.thread_exit_task_ = true;
  for (int32_t i = 0; i < work_thread_num_; i++)
  {
    task_mailbox_[i]->send(task);
  }

  for (int32_t i = 0; i < work_thread_num_; i++)
  {
    work_thread_[i]->join();
    delete task_mailbox_[i];
    delete work_thread_[i];
  }
  work_thread_.clear();
  task_mailbox_.clear();
}

bool ThreadPool::resizeWorkThreadNum(int32_t thread_num)
{
  ScopeLock scope_lock(lock_);
  if (work_thread_num_ <= 0)
  {
    task_dispatch_thread_.start();
  }
  if (thread_num <= work_thread_num_)
  {
    SET_ERROR_MSG(
          "thread pool current work thread is great set number!"
          "[current_work_thread_num:%d, set_work_thread_num:%d",
          work_thread_num_, thread_num);
    return false;
  }
  while (work_thread_num_ < thread_num)
  {
    TaskMailBox *task_mailbox = new TaskMailBox();
    WorkThread *work_thread = new WorkThread(task_mailbox, &wait_mailbox_);
    work_thread->setThreadEventListener(thread_event_listener_);
    task_mailbox_.push_back(task_mailbox);
    if (work_thread->start())
    {
      work_thread_.push_back(work_thread);
      work_thread_num_++;
    }
    else
    {
      SET_ERROR_MSG("thread pool start work thread fail!");
      delete work_thread;
      return false;
    }
  }
  LOG(INFO_LOGGER, 0,
      "thread pool set work thread successful![current_work_thread_num:%d]",
      work_thread_num_);
  return true;
}

int32_t ThreadPool::getCurWorkThreadNum()
{
  return work_thread_num_; 
}

void ThreadPool::setThreadEventListener(ThreadEventListener *thread_event_listener)
{
  thread_event_listener_ = thread_event_listener;
}

TaskDispatchThread::TaskDispatchThread(ThreadPool *thread_pool) : thread_pool_(thread_pool)
{}

void TaskDispatchThread::run()
{
  while (true)
  {
    Task task;
    if (!thread_pool_->input_mailbox_.recv(task, 1000))
    {
      if (thread_pool_->ready_stop_)
      {
        break;
      }
      continue;
    }

    TaskMailBox *task_mailbox = NULL;
    if (thread_pool_->wait_mailbox_.recv(task_mailbox, -1))
    {
      task_mailbox->send(task);
    }
    else
    {
      LOG(FATAL_LOGGER, 1, "thread pool find wait mailbox is error!");
      thread_pool_->input_mailbox_.send(task);
    }
  }
}

ThreadPool *ThreadPool::getDefaultThreadPool()
{
  static SingletonHolder<ThreadPool> inst;
  inst.get();
}


}
