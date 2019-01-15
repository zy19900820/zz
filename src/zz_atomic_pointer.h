#ifndef _ZZ_ATOMIC_POINTER_H
#define _ZZ_ATOMIC_POINTER_H

#include "zz_baseclass.h"

#if (defined __i386__ || defined __x86_64__) && defined __GNUC__
#define ZZ_ATOMIC_PTR_X86
#else
#define ZZ_ATOMIC_PTR_MUTEX
#endif

#if defined ZZ_ATOMIC_PTR_MUTEX
#include "zz_lock.h"
#endif

namespace zz
{

template<class T>
class AtomicPointer : public NonCopyAble
{
public:
  AtomicPointer() : ptr_(NULL)
  {}

  ~AtomicPointer()
  {}

  void set(T *ptr)
  {
    ptr_ = ptr;
  }

  /*
   * 传入新值，返回旧值
   * @param val   新值
   * @return  旧值
   */
  T *xchg(T *val)
  {
#if defined ZZ_ATOMIC_PTR_X86
    T *old;
    __asm__ volatile (
    "lock; xchg %0, %2"
    : "=r" (old), "=m" (ptr_)
    : "m" (ptr_), "0" (val));
    return old;
#else
    lock_.lock();
    T *old = reinterpret_cast<T*>(ptr_);
    ptr_ = val;
    lock_.unLock();
    return old;
#endif
  }

  /*
   * 比较cmp 相同设置val
   * @return 原值
   */
  T *cas(T *cmp, T *val)
  {
#if defined ZZ_ATOMIC_PTR_X86
    T *old;
    __asm__ volatile (
    "lock; cmpxchg %2, %3"
    : "=a" (old), "=m" (ptr_)
    : "r" (val), "m" (ptr_), "0" (cmp)
    : "cc");
    return old;
#else
    lock_.lock();
    T *old = reinterpret_cast<T*>(ptr_);
    if (ptr_ == cmp)
      ptr_ = val;
    lock_.unLock();
    return old;
#endif
  }


  volatile T *get()
  {
    return ptr_;
  }

  static const char *getTypeName()
  {
#if defined ZZ_ATOMIC_PTR_MUTEX
    return "atomic mutex";
#else 
    return "atomic x86";
#endif
  }

private:
  volatile T *ptr_;
#if defined ZZ_ATOMIC_PTR_MUTEX
  SpinLock lock_;
#endif
};

}

#endif // _ZZ_ATOMIC_POINTER_H
