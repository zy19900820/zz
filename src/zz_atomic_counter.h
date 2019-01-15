#ifndef _ZZ_ATOMIC_COUNTER_H
#define _ZZ_ATOMIC_COUNTER_H

#include "zz_basedef.h"

template<class T>
class AtomicCounter
{
public:
  AtomicCounter() : counter_(0)
  {}

  AtomicCounter(T value) : counter_(value)
  {}

  AtomicCounter(const AtomicCounter& atomicCounter) : counter_(atomicCounter.getValue())
  {}

  AtomicCounter& operator=(const AtomicCounter& atomicCounter)
  {
    if (this != &atomicCounter)
    {
      __sync_lock_test_and_set(&counter_, atomicCounter.getValue());
    }
    return *this;
  }

  AtomicCounter& operator=(T value)
  {
    __sync_lock_test_and_set(&counter_, value);
    return *this;
  }

  T getValue() const 
  {
    return counter_;
  }

  T operator++()
  {
    // ++T
    return __sync_add_and_fetch(&counter_, 1);
  }

  T operator++(int)
  {
    // T++
    return __sync_fetch_and_add(&counter_, 1);
  }

  T operator--()
  {
    return __sync_sub_and_fetch(&counter_, 1);
  }

  T operator--()
  {
    return __sync_fetch_and_sub(&counter_, 1);
  }

  T operator+=(T value)
  {
    return __sync_add_and_fetch(&counter_, value);
  }

  T operator-=(T value)
  {
    return __sync_sub_and_fetch(&counter_, value);
  }

  // counter == oldvalue  counter = newvalue
  // @return 旧值
  T cas(T oldValue, T newValue)
  {
    return __sync_val_compare_and_swap(&counter_, oldvalue, newvalue); 
  }

private:
  T counter_;
};

#endif // _ZZ_ATOMIC_COUNTER_H
