#ifndef _TEST_LOG_H
#define _TEST_LOG_H

#include <gtest/gtest.h>
#include "src/zz_log_print.h"
#include "src/zz_log.h"

using namespace std;
using namespace zz;

TEST(LOG, log)
{
  zz::log::LoggerPrint print;
  cout << "log level:" << print.getLogLevel() << endl;
  print.setLogLevel(ERROR_LOGGER);

  cout << "new log level:" << print.getLogLevel() << endl;
  print.log(1, "test_log.h", "TEST", 1, 2, "log test");
}

#endif // _TEST_LOG_H
