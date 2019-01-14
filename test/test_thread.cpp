#ifndef _TEST_LOG_H
#define _TEST_LOG_H

#include <gtest/gtest.h>
#include "src/zz_log_print.h"
#include "src/zz_log.h"
#include "src/zz_thread.h"

using namespace std;
using namespace zz;

class CTask : public Runnable
{
public:
  void run()
  {
    cout << "test" << endl;    
  }
};


TEST(THREAD, thread)
{
  CTask task;
  Thread t1(&task);
  t1.start();
  t1.join();
}

#endif // _TEST_LOG_H
