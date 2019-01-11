#include <gtest/gtest.h>
#include <iostream>
#include "zz/zz_time.h"

using namespace std;
using namespace zz;

TEST(Time, time)
{
  cout << "begin sleepBySec" << endl;
  zz::time::sleepBySec(1);
  cout << "begin sleepByMs" << endl;
  zz::time::sleepByMs(456);
  cout << "begin sleepByUs" << endl;
  zz::time::sleepByUs(456789);
}
