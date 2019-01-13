#include "zz_os.h"
#include "zz_lock.h"
#include "zz_string.h"

namespace zz
{

namespace os
{

const char *getEnv(const string& name, const string& default_value)
{
  static ThreadLocal<EnvTlsCache> cache_inst;
  static MutexLock lock(true);

  const char *value = cache_inst->getEnv(name);
  if (value != NULL)
  {
    return value;
  }

  {
    ScopeLock scope_lock(lock);
    value = getenv(name.c_str());
  }

  if (value == NULL)
  {
    return default_value.c_str();
  }

  cache_inst->addEnv(name, value);
  return value;
}

bool replaceEnv(const string& org, string& result)
{
  string undefined_envname;
  return replaceEnv(org, result, undefined_envname);
}

bool replaceEnv(const string& org, string& result, string& undefined_envname)
{
  string result_temp = org;
  size_t start_pos = result_temp.find("$(");
  while (start_pos != string::npos) 
  {
    size_t end_pos = result_temp.find(")", start_pos);
    if (end_pos != string::npos)
    {
      string env_name = result_temp.substr(start_pos + 2, end_pos - start_pos - 2);
      const char *env_value = getEnv(env_name.c_str(), NULL);
      if (env_value == NULL)
      {
        undefined_envname = env_name;
        return false;
      }
      result_temp = zz::replaceAll(result_temp, 
                                  string("$(") + env_name + string(")"),
                                  env_value == NULL ? "" : env_value);
    }
    start_pos = result_temp.find("$(");
  }
  result = result_temp;
  return true;
}

}

}
