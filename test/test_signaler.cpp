#ifndef _TEST_LOG_H
#define _TEST_LOG_H

#include <gtest/gtest.h>
#include "src/zz_log_print.h"
#include "src/zz_log.h"
#include "src/zz_thread.h"
#include "src/zz_signaler.h"

using namespace std;
using namespace zz;

Signaler sig;

class CTask : public Runnable
{
public:
  void run()
  {
    cout << "enter run" << endl;
    sig.wait(-1);
    cout << "recv event" << endl;    
  }
};


TEST(THREAD, thread)
{

  CTask task;
  Thread t1(&task);
  t1.start();

  sig.create();
  sig.notify();
  sleep(2);
  
  t1.join();
  
}

#endif // _TEST_LOG_H
