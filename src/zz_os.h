#ifndef _ZZ_OS_H
#define _ZZ_OS_H

#include "zz_basedef.h"

namespace zz
{

namespace os
{

class EnvTlsCache
{
public:
  const char *getEnv(const string& name)
  {
    map<string, string>::iterator it = env_map_.find(name);
    if (it != env_map_.end())
    {
      return it->second.c_str();
    }
    return NULL;
  }

  void addEnv(const string& name, const string& value)
  {
    env_map_[name] = value;
  }
private:
  map<string, string> env_map_;
};

const char *getEnv(const string& name, const string& value="");

/*
 * @param org 相对路径
 * @param result 替换后的字符串
 */
bool replaceEnv(const string& org, string& result);

// @param undefined_env_name 出错时未定义变量名
bool replaceEnv(const string& org, string& result, string& undefined_env_name);

}

}

#endif // _ZZ_OS_H
