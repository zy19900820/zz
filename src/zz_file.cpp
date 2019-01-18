#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include "zz_file.h"
#include "zz_string.h"
#include "zz_os.h"

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

bool File::getRealPath(const string& path, string& real_path)
{
  string new_path;
  string undefine_env_name;
  if (!os::replaceEnv(path, new_path, undefine_env_name))
  {
    SET_ERROR_MSG(
            "translate to real path fail![org_path:%s, undefine_env_name:%s]",
            path.c_str(), undefine_env_name.c_str());
    return false;
  }
  real_path = new_path;
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

FileRead::FileRead() : fd_(-1), opened_(false), file_content_(NULL),
                cur_pos_(0), file_size_(0)
{
  reset();
}

FileRead::FileRead(const string& path) : File(path), fd_(-1), opened_(false),
                file_content_(NULL), cur_pos_(0), file_size_(0)
{
  reset();
}

FileRead::~FileRead()
{
  close();
}

bool FileRead::open()
{
  if (opened_)
  {
    SET_ERROR_MSG("file is opened![file_path:%s]", file_path_.c_str());
    return false;
  }
  fd_ = ::open(file_path_.c_str(), O_RDONLY);
  if (fd_ == -1)
  {
    SET_ERROR_MSG("open file fail![file_path:%s, reason:%s, errno:%d",
                file_path_.c_str(), strerror(errno), errno);
    return false;
  }
  struct stat sb;
  if (fstat(fd_, &sb) == -1)
  {
    SET_ERROR_MSG("stat file fail![file_path:%s, reason:%s, errno:%d",
                file_path_.c_str(), strerror(errno), errno);
    return false;
  }
  file_size_ = sb.st_size;
  file_content_ = reinterpret_cast<char *>(mmap(NULL, sb.st_size, PROT_READ,
                                        MAP_PRIVATE, fd_, 0));
  if (file_content_ == NULL)
  {
    SET_ERROR_MSG("mmap file fail![file_path:%s, reason:%s, errno:%d",
                file_path_.c_str(), strerror(errno), errno);
    return false;
  }
  opened_ = true;
  LOG_INFO("open file success for read![file_path:%s, file_content:%p, size:%d]",
          file_path_.c_str(), file_content_, file_size_);
  return opened_;
}

int64_t FileRead::countLine(const char *line_end_flag)
{
  if (eof())
  {
    return 0;
  }
  int64_t count = 0;
  int line_flag_len = strlen(line_end_flag);
  for (int64_t i = 0; i < file_size_; i += line_flag_len)
  {
    if (memcmp((const void *)(file_content_ + i),
                (const void *)line_end_flag, line_flag_len) == 0)
    {
      count++;
    }
  }
  return count;
}

bool FileRead::eof()
{
  if (!opened_)
  {
    return true;
  }
  if (cur_pos_ >= file_size_)
  {
    return true;
  }
  return false;
}

const char *FileRead::readLine(int64_t& line_size, const char *line_end_flag)
{
  if (eof())
  {
    return NULL;
  }
  int line_flag_len = strlen(line_end_flag);
  const char *start_addr = file_content_ + cur_pos_;
  int64_t start_pos = cur_pos_;
  for (; start_pos < file_size_; start_pos += line_flag_len)
  {
    if (memcmp((const void *) (file_content_ + start_pos),
          (const void *) line_end_flag, line_flag_len) == 0)
    {
      break;
    }
  }

  line_size = start_pos - cur_pos_;
  cur_pos_ = start_pos + line_flag_len;
  return start_addr;
}

string FileRead::readLine(const char *line_end_flag)
{
  int64_t line_size = 0;
  const char *content = readLine(line_size, line_end_flag);
  if (content == NULL)
  {
    return "";
  }
  return string(content, line_size);

}

const char *FileRead::readAll()
{
  if (!opened_)
  {
    return NULL;
  }
  return file_content_;
}

const char *FileRead::readRemain()
{
  if (!opened_)
  {
    return NULL;
  }
  return file_content_ + cur_pos_;
}

void FileRead::reset()
{
  opened_ = false;
  fd_ = -1;
  file_size_ = 0;
  cur_pos_ = 0;
  file_content_ = NULL;
}

void FileRead::close()
{
  if (opened_)
  {
    munmap(file_content_, file_size_);
    ::close(fd_);
    reset();
  }
}

bool FileWrite::open()
{
  return open(O_WRONLY | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
}

bool FileWrite::open(int32_t flag, mode_t mode)
{
  if (fd_ != -1)
  {
    SET_ERROR_MSG("file is opened![file_path:%d, fd:%p]", file_path_.c_str(),fd_);
    return false;
  }
  fd_ = ::open(file_path_.c_str(), flag, mode);
  if (fd_ == -1)
  {
    SET_ERROR_MSG("open file fail![file_path:%s, flag:%d, mode:%d, reason:%s, errno:%d",
      file_path_.c_str(), flag, mode, strerror(errno), errno);
    return false;
  }
  LOG_INFO("open file sucessful for write![fd:%d, file_path:%s]", fd_,
      file_path_.c_str());
  return false;
}

bool FileWrite::write(const string& content)
{
  return write(content.c_str(), content.length());
}

bool FileWrite::write(const char *content)
{
  return write(content, strlen(content));
}

bool FileWrite::write(const char *content, int32_t len)
{
  int32_t nwrite = 0;
  while (nwrite < len)
  {
    int32_t ret = ::write(fd_, content + nwrite, len - nwrite);
    if (ret <= 0)
    {
      SET_ERROR_MSG("write file fail![file_path:%s, reason:%s, errno:%d",
                file_path_.c_str(), strerror(errno), errno);
      return false;
    }
    nwrite += ret;
  }
  return true;
}

bool FileWrite::writeLine(const string& content, const char *line_end_flag)
{
  return writeLine(content.c_str(), line_end_flag);
}

bool FileWrite::writeLine(const char *content, const char *line_end_flag)
{
  if (!write(content))
  {
    return false;
  }
  if (!write(line_end_flag))
  {
    return false;
  }
  return true;
}

void FileWrite::close()
{
  if (fd_ > 0)
  {
    ::close(fd_);
    LOG_INFO("close file sucessful for write ![fd:%d, file_path:%s]", fd_,
            file_path_.c_str());
    fd_ = -1;
  }
}

}
