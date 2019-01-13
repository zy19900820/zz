#include "zz_string.h"

namespace zz
{

string itoa(int32_t i)
{
  char buf[255];
  sprintf(buf, "%d", i);
  return string(buf);
}

string itoa(int64_t i)
{
  char buf[255];
  sprintf(buf, "%lld", i);
  return string(buf);
}

string trim(const string& src)
{
  if (src.length() == 0)
  {
    return src;
  }

  string ret = src;
  size_t start_pos = 0;
  while (start_pos < ret.length())
  {
    char tmp = ret[start_pos];
    if (tmp == '\r' || tmp == '\n' || tmp == '\t' || tmp == ' ')
    {
      start_pos++;
      continue;
    }
    break;
  }

  size_t end_pos = ret.length() - 1;
  while (end_pos > 0)
  {
    char tmp = ret[end_pos];
    if (tmp == '\r' || tmp == '\n' || tmp == '\t' || tmp == ' ')
    {
      end_pos--;
      continue;
    }
    break;
  }

  return ret;
}

string toUpperCase(const string& src)
{
  string ret = src;
  string::iterator it = ret.begin();
  while (it != ret.end())
  {
    (*it) = (unsigned char)toupper((unsigned char)(*it));
    it++;
  }
  return ret;
}

string toLowerCase(const string& src)
{
  string ret = src;
  string::iterator it = ret.begin();
  while (it != ret.end())
  {
    (*it) = (unsigned char)tolower((unsigned char)(*it));
    it++;
  }
  return ret;
}

string format(const char *format, ...)
{
  char buf[4096];
  string ret;
  if (format != NULL)
  {
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    buf[sizeof(buf) - 1] = 0;
    va_end(args);
    ret = buf;
  }
  return ret;
}

string replaceAll(const string& src, const string& target, const string& replacement)
{
  string ret = src;
  size_t pos = 0;
  for (; (pos = ret.find(target, pos)) != string::npos;) 
  {
    ret.erase(pos, target.length());
    ret.insert(pos, replacement);
    pos += replacement.length();
  }
  return ret;
}

}
