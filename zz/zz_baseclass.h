#ifndef _ZZ_BASECLASS_H
#define _ZZ_BASECLASS_H

namespace zz
{

class NonCopyAble
{
protected:
  NonCopyAble()
  {}

  virtual ~NonCopyAble()
  {}

private:
  NonCopyAble(const NonCopyAble&)
  {}

  const NonCopyAble& operator=(const NonCopyAble&)
  {}
};

}

#endif // _ZZ_BASECLASS_H
