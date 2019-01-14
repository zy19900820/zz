#ifndef _ZZ_SIGNALER_H
#define _ZZ_SIGNALER_H

#include "zz_baseclass.h"

namespace zz
{

class Signaler
{
public: 
  Signaler();

  Signaler(int32_t fd);

  ~Signaler();

  void close();

  bool notify();

  // @param timeout_ms 负数 无限等待
  bool wait(int32_t timeout_ms);

  int32_t getFd();

  int32_t read();

  void setFd(int32_t fd);

  bool create();

private:
  int32_t fd_;
};

}

#endif // _ZZ_SIGNALER_H
