#ifndef _TEST_BASECLASS_H
#define _TEST_BASECLASS_H

#include <gtest/gtest.h>
#include "src/zz_baseclass.h"
#include "src/zz_atomic_pointer.h"

using namespace std;
using namespace zz;

TEST(ATOMIC_POINTER, atomic_pointer)
{
  AtomicPointer<int> pointer;
  cout << zz::AtomicPointer<int>::getTypeName() << endl;
  int a = 1;
  int b = 2;
  pointer.set(&a);
  EXPECT_EQ(1, *pointer.get());
  pointer.xchg(&b);
  EXPECT_EQ(2, *pointer.get());
  pointer.cas(&b, &a);
  EXPECT_EQ(1, *pointer.get());
}

#endif // _TEST_BASECLASS_H
