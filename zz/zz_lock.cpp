#include "zz_lock.h"

namespace zz
{

SpinLock::SpinLock(bool share_process)
{
  if (share_process)
  {
    pthread_spin_init(&lock_, PTHREAD_PROCESS_SHARED);
  }
  else
  {
    pthread_spin_init(&lock_, PTHREAD_PROCESS_PRIVATE);
  }
}

SpinLock::~SpinLock()
{
  pthread_spin_destroy(&lock_);
}

void SpinLock::Lock()
{
  pthread_spin_lock(&lock_);
}

bool SpinLock::TryLock()
{
  return pthread_spin_trylock(&lock_) == 0;
}

void SpinLock::UnLock()
{
  pthread_spin_unlock(&lock_);
}

MutexLock::MutexLock(bool mutex_recursive, bool share_process)
{
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  if (mutex_recursive)
  {
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  }
  if (share_process)
  {
    pthread_mutexattr_settype(&attr, PTHREAD_PROCESS_SHARED);
  }

  pthread_mutex_init(&lock_, &attr);
  pthread_mutexattr_destroy(&attr);
}

MutexLock::~MutexLock()
{
  pthread_mutex_destroy(&lock_);
}

void MutexLock::Lock()
{
  pthread_mutex_lock(&lock_);
}

bool MutexLock::TryLock()
{
  return pthread_mutex_trylock(&lock_) == 0;
}

void MutexLock::UnLock()
{
  pthread_mutex_unlock(&lock_);
}

RWLockHelper::RWLockHelper(pthread_rwlock_t *rwlock, int state)
                    : rwlock_(rwlock), state_(state), error_(0)
{
  if (kRdlock == state_)
  {
    error_ = pthread_rwlock_rdlock(rwlock_);
  }
  else if (kWrlock == state_)
  {
    error_ = pthread_rwlock_wrlock(rwlock_);
  }
}

RWLockHelper::~RWLockHelper()
{
  pthread_rwlock_unlock(rwlock_);

  state_ = kUnlock;
  error_ = 0;
}

}
