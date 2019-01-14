#include <string.h>
#include "zz_config_read.h"
#include "zz_file.h"
#include "zz_string.h"
#include "zz_singleton.h"

#define MULTI_LINE_FLAG     "'''"

namespace zz
{

bool ConfigRead::open(const string& config_file)
{
  FileRead file(config_file);
  if (!file.open())
  {
    COPY_ERROR_MSG(file);
    return false;
  }
  string config = file.readAll();
  config = zz::replaceAll(config, "\r\n", "\n");
  config.append("\n");
  file.close();

  return parseConfigData(config);
}

bool ConfigRead::parseConfigData(string& config)
{
  int32_t line_start_pos = 0;
  int32_t line_end_pos = config.find("\n");
  string section_name = "_";
  while (line_end_pos >= line_start_pos)
  {
    // 本行内容
    string line = config.substr(line_start_pos, line_end_pos - line_start_pos);
    
    // 下一行设置
    line_start_pos = line_end_pos + 1;
    line_end_pos = config.find("\n", line_start_pos);

    line = zz::trim(line);

    // 去掉注释
    if (line.length() >= 1 && line[0] == '#')
      continue;

    if (line[0] == '[' && line[line.length() - 1] == ']')
    {
      // session_name
      section_name = line.substr(1, line.length() - 2);
    }
    else
    {
      size_t equal_pos = line.find("=");
      // 配置文件格式不正确
      if (equal_pos == string::npos)
        continue;
      string key = zz::trim(line.substr(0, equal_pos));
      string value = zz::trim(line.substr(equal_pos + 1, -1));
      if (strncmp(value.c_str(), MULTI_LINE_FLAG, strlen(MULTI_LINE_FLAG)) == 0)
      {
        // 查找下个标记位置
        size_t multi_end_flag_pos = config.find(MULTI_LINE_FLAG, line_start_pos);
        if (multi_end_flag_pos == string::npos)
        {
          SET_ERROR_MSG("find multi line flag fail!");
          return false;
        }
        string remain = config.substr(line_start_pos, multi_end_flag_pos - line_start_pos);
        value = value.substr(strlen(MULTI_LINE_FLAG));
        value.append("\n");
        value.append(remain);

        // 计算下一行位置
        line_start_pos = multi_end_flag_pos + 1;
        line_end_pos = config.find("\n", line_start_pos);
      }
      {
        ScopeLock scop_lock(lock_);
        map<string, map<string, string> >::iterator itr = config_data_.find(section_name);
        if (itr == config_data_.end())
        {
          map<string, string> mapTemp;
          config_data_[section_name] = mapTemp;
          itr = config_data_.find(section_name);
        }
        itr->second[key] = value;
        LOG_INFO("parse config![session:%s, key:%s, value:%s]",
                section_name.c_str(), key.c_str(), value.c_str());
      }
    }
  }
  return true;
}

void ConfigRead::read(const string& section_name, const string& name, int32_t& value, int32_t default_value)
{
  if (!read(section_name, name, value))
  {
    value = default_value;
    LOG_INFO("config read by default![section_name:%s, name:%s, value:%d]",
            section_name.c_str(), name.c_str(), value);
  }
}

void ConfigRead::read(const string& section_name, const string& name, string& value, const string& default_value)
{
  if (!read(section_name, name, value))
  {
    value = default_value;
    LOG_INFO("config read by default![section_name:%s, name:%s, value:%s]",
            section_name.c_str(), name.c_str(), value.c_str());
  }
}

vector<string> ConfigRead::getAllSection()
{
  vector<string> all_section;
  map<string, map<string, string> >::iterator itr = config_data_.begin();
  for (; itr != config_data_.end(); itr++)
  {
    if (itr->first != "_")
    {
      all_section.push_back(itr->first);
    }
  }
  return all_section;
}

bool ConfigRead::read(const string& section_name, const string& name, int32_t& value)
{
  string tmp_value;
  bool bRet = read(section_name, name, tmp_value);
  if (bRet)
  {
    value = atoi(tmp_value.c_str());
  }
  return bRet;
}

bool ConfigRead::readByDefaultSession(const string& name, string &value)
{
  map<string, map<string, string> >::iterator itr = config_data_.find("_");
  if (itr == config_data_.end()) 
  {
    return false;
  }

  map<string, string>::iterator itr_key = itr->second.find(name);
  if (itr_key == itr->second.end())
  {
    return false;
  }

  value = itr_key->second;
  LOG_INFO("config read sucessful![name:%s, value:%s]", name.c_str(), value.c_str());
  return true;
}

map<string, string> ConfigRead::getSection(const string& section_name)
{
  map<string, string> tmp_section;
  map<string, map<string, string> >::iterator itr = config_data_.find(section_name);
  if (itr == config_data_.end())
  {
    return tmp_section;
  }
  return itr->second;
}

bool ConfigRead::read(const string& section_name, const string& name, string& value)
{
  map<string, map<string, string> >::iterator itr = config_data_.find(section_name);
  if (itr == config_data_.end())
  {
    return false;
  }

  map<string, string>::iterator itr_key = itr->second.find(name);
  if (itr_key == itr->second.end())
  {
    return readByDefaultSession(name, value);
  }

  value = itr_key->second;
  LOG_INFO("config read sucessful![section_name:%s, name:%s, value:%s]",
          section_name.c_str(), name.c_str(), value.c_str());
  return true;
}

ConfigRead *ConfigRead::getDefault()
{
  static SingletonHolder<ConfigRead> inst(false);
  return inst.get();
}

bool initConfig(const string& config_path)
{
  if (!ConfigRead::getDefault()->open(config_path))
  {
    printf("init config fail!%s\n", ConfigRead::getDefault()->getErrorMsg().c_str());
    return false;
  }
  return true;
}

}
