#ifndef _TEST_LOG_H
#define _TEST_LOG_H

#include <gtest/gtest.h>
#include "src/zz_log_print.h"
#include "src/zz_log.h"
#include "src/zz_file.h"

using namespace std;
using namespace zz;

TEST(File, file)
{
  FileRead test_read;
  EXPECT_TRUE(test_read.setFilePath("./test.txt"));
  EXPECT_TRUE(test_read.open());
  if (!test_read.eof())
  {
    string tmpRead = test_read.readLine();
    printf("Readline = %s\n", tmpRead.c_str());
  }
  const char *ptrReadRemain = test_read.readRemain();
  printf("ReadRemain = %s\n", ptrReadRemain);
  const char *ptrReadAll = test_read.readAll();
  printf("ReadAll = %s\n", ptrReadAll);
  EXPECT_EQ(6, test_read.countLine());
  test_read.close();
}

#endif // _TEST_LOG_H
