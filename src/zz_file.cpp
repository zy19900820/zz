#include <sys/stat.h>
#include <sys/file.h>
#include "zz_file.h"
#include "zz_string.h"

namespace zz
{

File::File() : SupportErrorMsg(false), file_path_("")
{}

File::File(const string& path) : SupportErrorMsg(false)
{
  setFilePath(path);
}

int64_t File::getSize()
{
  int64_t file_size = -1;
  struct stat statbuff;
  if (stat(file_path_.c_str(), &statbuff) < 0)
  {
    return file_size;
  }
  return statbuff.st_size;
}

bool File::setFilePath(const string& path)
{
  return getRealPath(path, file_path_);
}

bool File::isExist()
{
  struct stat statbuff;
  if (stat(file_path_.c_str(), &statbuff) < 0)
  {
    return false;
  }
  return true;
}

bool File::deleteFile()
{
  close();
  return unlink(file_path_.c_str()) == 0;
}

bool File::moveToDir(const string& dir_path)
{
  string new_dir_path = dir_path;
  if (new_dir_path[new_dir_path.length() - 1] != '/')
  {
    new_dir_path += '/';
  }
  string new_path = zz::format("%s%s", new_dir_path.c_str(),
                            getFileName().c_str());
  return rename(new_path.c_str());
}

string File::getFileDirectory()
{
  size_t pos = file_path_.rfind("/");
  if (pos == string::npos)
  {
    return "";
  }
  return file_path_.substr(0, pos + 1);
}

string File::getFileName()
{
  size_t pos = file_path_.rfind("/");
  if (pos == string::npos)
  {
    return file_path_.c_str();
  }
  return file_path_.c_str() + pos + 1;
}

//FIXME
bool File::getRealPath(const string& path, string& real_path)
{
  real_path = path;
  return true;
}

bool File::rename(const string& new_path)
{
  close();
  string new_real_path;
  if (!getRealPath(new_path, new_real_path))
  {
    return false;
  }

  int ret = ::rename(file_path_.c_str(), new_real_path.c_str());
  if (ret != 0)
  {
    SET_ERROR_MSG("rename file fail![old_name:%s, new_name:%s]",
                file_path_.c_str(), new_real_path.c_str());
    return false;
  }
  LOG_INFO("file rename sucessful![old_name:%s, new_name:%s]",
          file_path_.c_str(), new_real_path.c_str());
  file_path_ = new_real_path;
  return true;
}

string File::getFilePath()
{
  return file_path_.c_str();
}

bool File::gzip()
{
  if (!isExist())
  {
    return false;
  }

  string cmd = zz::format("gzip %s", file_path_.c_str());
  int iRet = system(cmd.c_str());
  file_path_.append(".gz");
  if (iRet != 0)
  {
    SET_ERROR_MSG("exec system fail![ret:%d, cmd:%s]", iRet, cmd.c_str());
    return false;
  }
  return true;
}

}
