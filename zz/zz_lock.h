#ifndef _ZZ_LOCK_H
#define _ZZ_LOCK_H

#include <pthread.h>
#include "zz_baseclass.h"

namespace zz
{

class ILock : public NonCopyAble
{
public:
  virtual void Lock()=0;

  virtual void UnLock()=0;

  virtual bool TryLock()=0;
};


class ScopeLock : public NonCopyAble
{
public:
  explicit ScopeLock(ILock& lock) : lock_(lock)
  {
    lock_.Lock();
  }

  ~ScopeLock()
  {
    lock_.UnLock();
  }

private:
  ILock& lock_;
};

/*
 * 自旋锁适用于锁使用者保持锁时间比较短的情况
 *
 */
class SpinLock : public ILock
{
public:
  /*
   * share_process  是否进程间共享
   * 如果进程间需要共享，那么锁必须放在共享内存中才有效果
   */
  explicit SpinLock(bool share_process=false);

  ~SpinLock();

  virtual void Lock();

  virtual bool TryLock();

  virtual void UnLock();
private:
  pthread_spinlock_t lock_;
};

// 互斥锁
class MutexLock : public ILock
{
public:
  /*
   * mutex_recursive 是否可线程递归加锁
   * true: 上锁多次 解锁一次
   *
   * share_process 是否进程间共享
   * 如果进程间需要共享，那么锁必须放在共享内存中才有效果
   */
  explicit MutexLock(bool mutex_recursive=false, bool share_process=false);

  ~MutexLock();

  virtual void Lock();

  virtual bool TryLock();

  virtual void UnLock();
private:
  pthread_mutex_t lock_;
};

class RWLockHelper
{
public:
  RWLockHelper(pthread_rwlock_t *rwlock, int state);

  ~RWLockHelper();

  int state() const
  {
    return state_;
  }

  int error() const
  {
    return error_;
  }

public:
  enum __lock_state
  {
    kUnlock = 0,
    kRdlock = 1,
    kWrlock = 2
  };

private:
  pthread_rwlock_t *rwlock_;
  int state_;
  int error_;
};

}

#endif // _ZZ_LOCK_H
