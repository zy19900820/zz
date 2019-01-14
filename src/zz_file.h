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

}

#endif // _ZZ_FILE_H
