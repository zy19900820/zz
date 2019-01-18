#ifndef _ZZ_FILE_H
#define _ZZ_FILE_H

#include "zz_baseclass.h"

namespace zz
{

class File : public SupportErrorMsg
{
public:
  File();

  explicit File(const string& path);

  virtual ~File()
  {}

  virtual bool open()=0;

  virtual void close()=0;

  bool setFilePath(const string& path);

  int64_t getSize();

  bool isExist();

  bool deleteFile();

  bool rename(const string& new_path);

  bool moveToDir(const string& dir_path);

  string getFileName();

  // 路径加文件名
  string getFilePath();

  string getFileDirectory();

  // 压缩文件
  bool gzip();

private:
  bool getRealPath(const string& org_path, string& real_path);

protected:
  string file_path_;
};

class FileRead : public File
{
public:
  FileRead();

  // 支持环境变量的路径
  explicit FileRead(const string& path);

  virtual ~FileRead();

  bool open();

  bool eof();

  int64_t countLine(const char *line_end_flag = "\n");

  const char *readLine(int64_t& line_size, const char *line_end_flag = "\n");

  string readLine(const char *line_end_flag = "\n");

  const char *readAll();

  // 获取剩余内容
  const char *readRemain();

  void close();

private:
  void reset();

private:
  int32_t fd_;
  bool opened_;
  char *file_content_;
  int64_t cur_pos_;
  int64_t file_size_;
};

class FileWrite : public File
{
public:
  FileWrite();

  explicit FileWrite(const string& file_path);

  virtual ~FileWrite();
  
  // 权限 O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
  bool open();

  bool open(int32_t flag, mode_t mode);

  bool writeLine(const string& content, const char *line_end_flag="\n");

  bool writeLine(const char *content, const char *line_end_flag="\n");

  bool write(const string& content);

  bool write(const char *content);

  bool write(const char *content, int32_t len);

  void close();

protected:
  int32_t fd_;
};

}

#endif // _ZZ_FILE_H
