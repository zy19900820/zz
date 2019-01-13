#ifndef _TEST_BASECLASS_H
#define _TEST_BASECLASS_H

#include <gtest/gtest.h>
#include "src/zz_baseclass.h"

using namespace std;
using namespace zz;

class Test1 : public SupportErrorMsg
{
public:
  virtual void test()
  {
    SET_ERROR_MSG("base error");
  }
};

class Test2 : public SupportErrorMsg
{
public:
  virtual void test()
  {
    test_.test();
    COPY_ERROR_MSG(test_);
  }
private:
  Test1 test_; 
};

TEST(BASECLASS, baseclass)
{
  Test1 test1;
  Test2 test2;
  test1.test();
  cout << "error:" << test1.getErrorMsg() << endl;
  test2.test();
  cout << "test2 stack:" << test2.getStackTrace() << endl;
  test1.clearErrorMsg();
  cout << "  null?:" << test1.getErrorMsg() << endl;
}

#endif // _TEST_BASECLASS_H
