#ifndef _ZZ_SINGLETON_H
#define _ZZ_SINGLETON_H

#include "zz_lock.h"

namespace zz
{

/*
 * 单例模式
 * 保证多线程安全
 *
 * @code
 * class Test
 * {
 * public:
 *  void foo();
 *  static Test *getInstance()
 *  {
 *    static zz::SingletonHolder<Test> testManager;
 *    return testManager.get();
 *  }
 * };
 *
 * Test::getInstance()->foo();
 */
template<class T>
class SingletonHolder : public NonCopyAble
{
public:
  SingletonHolder() : instance_(NULL), exit_release_(true)
  {}

  explicit SingletonHolder(bool exit_release) : instance_(NULL), exit_release_(exit_release)
  {}

  ~SingletonHolder()
  {
    if (exit_release_)
    {
      if (instance_)
      {
        delete instance_;
      }
    }
  }

  T *get()
  {
    if (instance_ == NULL)
    {
      ScopeLock scope_lock(lock_);
      if (instance_ == NULL)
      {
        T *temp = new T();
        instance_ = temp;
      }
    }
    return instance_;
  }

  T& operator*()
  {
    return *get();
  }

  T *operator->()
  {
    return get();
  }
private:
  SpinLock lock_;
  T *volatile instance_;
  bool exit_release_;     // 退出是否销毁
};

}

#endif // _ZZ_SINGLETON_H
