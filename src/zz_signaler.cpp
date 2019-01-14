#include <sys/eventfd.h>
#include <poll.h>
#include <string.h>
#include <errno.h>
#include "zz_signaler.h"

namespace zz
{

Signaler::Signaler() : fd_(-1)
{
  create();
}

Signaler::Signaler(int32_t fd)
{
  fd_ = fd;
}

Signaler::~Signaler()
{
  close();
}

void Signaler::close()
{
  if (fd_ != -1)
  {
    ::close(fd_);
    fd_ = -1;
  }
}

bool Signaler::create()
{
  fd_ = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (fd_ == -1)
  {
    LOG_FATAL(-1, "create signaler fail!");
    return false;
  }
  return true;
}

bool Signaler::notify()
{
  const int64_t cmd = 1;
  ssize_t sz = write(fd_, &cmd, sizeof(cmd));
  bool ok = (sz == sizeof(cmd));
  if (!ok)
  {
    LOG_FATAL(-1, "signaler notify fail![fd:%d, ret:%d]", fd_, sz);
    return false;
  }
  return true;
}

bool Signaler::wait(int32_t timeout_ms)
{
  struct pollfd pfd;
  pfd.fd = fd_;
  pfd.events = POLLIN;
  while (1)
  {
    int ret = poll(&pfd, 1, timeout_ms);
    if (ret < 0)
    {
      if (errno == EINTR || errno == EAGAIN)
        continue;
      return false;
    }
    else if (ret == 0)
    {
      //超时
      return false;
    }
    else if (ret == 1 && pfd.revents & POLLIN)
    {
      // 收到事件
      break;
    }
    else
    {
      // 事件个数不为1
      return false;
    }
  }

  int64_t cmd = read();
  if (cmd == -1)
  {
    LOG_FATAL(-1, "signaler wait fail![fd:%d, ret:%lld]", fd_, cmd);
    return false;
  }
  if (cmd > 1)
  {
    LOG_INFO("get event more than 1");
  }
  return true;
}

int32_t Signaler::read()
{
  int64_t cmd = 0;
  ssize_t sz = ::read(fd_, &cmd, sizeof(cmd));
  if (sz != sizeof(cmd))
  {
    return -1;
  }
  return cmd;
}

int32_t Signaler::getFd()
{
  return fd_;
}

void Signaler::setFd(int32_t fd)
{
  fd_ = fd;
}

}
