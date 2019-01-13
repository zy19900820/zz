#ifndef _ZZ_THREADLOCAL_H
#define _ZZ_THREADLOCAL_H

#include "zz_basedef.h"

namespace zz
{

template<class T>
class ThreadLocal
{
/*
 * 线程局部存储
 *
 * @code
 *
 * class Test
 * {
 * public:
 *  int value;
 * };
 *
 * ThreadLocal<Test> t;
 * t->value = 1;
 *
 */
public:
  ThreadLocal()
  {
    pthread_key_create(&key_, &threadLocalDelete);
  }

  ~ThreadLocal()
  {
    pthread_key_delete(key_);
  }

  /*
   * attach detach  实现线程局部存储与对象的绑定和解除
   */
  void attach(T *pT)
  {
    pthread_setspecific(key_, pT);
  }

  T *detach()
  {
    T *p = get();
    attach(NULL);
    return p;
  }

  T *get()
  {
    T *p = Ptr();
    if (p != NULL)
    {
      return p;
    }
    p = new T();
    attach(p);
    return p;
  }

  T &operator*()
  {
    return *get();
  }

  T *operator->()
  {
    return get();
  }

private:
  static void threadLocalDelete(void *p)
  {
    delete reinterpret_cast<T *>(p);
  }

  T *Ptr() const
  {
    return reinterpret_cast<T *>(pthread_getspecific(key_));
  }
private:
  pthread_key_t key_;
};

}

#endif // _ZZ_THREADLOCAL_H
