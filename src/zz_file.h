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

private:
  string file_path_;
};

}

#endif // _ZZ_FILE_H
