#ifndef _TEST_LOG_H
#define _TEST_LOG_H

#include <gtest/gtest.h>
#include "src/zz_log_print.h"
#include "src/zz_log.h"
#include "src/zz_thread.h"
#include "src/zz_signaler.h"
#include "src/zz_threadpool.h"

using namespace std;
using namespace zz;

int num = 0;

class CTask : public Runnable
{
public:
  void run()
  {
    cout << num++ << endl;
  }
};


TEST(THREADPOOL, threadpool)
{
  ThreadPool pool(10);
  pool.start();
  for (int i = 0; i < 100; i++)
  {
    CTask *task = new CTask();
    pool.processRequest(task);
  }
  pool.join();
  cout << num << endl;

}

#endif // _TEST_LOG_H
