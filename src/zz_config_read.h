#ifndef _ZZ_CONFIG_READ_H
#define _ZZ_CONFIG_READ_H

#include "zz_baseclass.h"
#include "zz_lock.h"

namespace zz
{

class ConfigRead : public SupportErrorMsg
{
public:
  bool open(const string& config_file);

  bool read(const string& section_name, const string& name, string& value);

  bool read(const string& section_name, const string& name, int32_t& value);

  void read(const string& section_name, const string& name, int32_t& value, int32_t default_value);

  void read(const string& section_name, const string& name, string& value, const string& default_value);

  vector<string> getAllSection();

  map<string, string> getSection(const string& section_name);

  static ConfigRead *getDefault();

private:
  bool parseConfigData(string& config);

  bool readByDefaultSession(const string& name, string& value);

private:
  SpinLock lock_;
  map<string, map<string, string> > config_data_;
};

bool initConfig(const string& config_path);

}

#endif // _ZZ_CONFIG_READ_H
