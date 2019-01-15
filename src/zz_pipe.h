#ifndef _ZZ_PIPE_H
#define _ZZ_PIPE_H

#include "zz_queue.h"

namespace zz
{

template<class T, int N=16>
class ZZPipe : public NonCopyAble
{
public:
  ZZPipe()
  {
    queue_.push();

    r = w = end_ = &queue_.back();
    readPre.set(&queue_.back());
  }

  void write(const T& value)
  {
    queue_.back() = value;
    queue_.push();

    end_ = &queue_.back();
  }

  /*
   * 将预读 未刷新元素 刷新到队列末端
   * @return false表示读停止
   */
  bool flush()
  {
    if (w == end_)
      return true;

    if (readPre.cas(w, end_) != w)
    {
      // readPre == NULL
      // 表示读取休眠 返回false
      readPre.set(end_);
      w = end_;
      return false;
    }

    w = end_;
    return true;
  }

  /*
   * @return 是否可读
   */
  bool checkRead()
  {
    // 期望最后读取的位置不在队列前端
    if (&queue_.front() != r && r)
      return true;
  
    // 获取预读元素
    // 如果预读在队列最前 则下次预读为NULL 读取休眠
    r = readPre.cas(&queue_.Front(), NULL);
 
    // 预读元素为NULL或者在最前端
    if (&queue_.Front() == r || !r)
      return false;

    //最后要读取的元素在其他位置,可以放心的去读取了
    return true;
  }

  bool read(T& value)
  {
    if (checkRead())
    {
      //预读成功就去读取
      value = queue_.Front();
      queue_.pop();
      return true;
    }

    return false;
  }

private:
  ZZQueue<T, N> queue_;     // 数据存储队列

  T *w;                     // 指向第一个未刷新元素

  T *end_;                  // 指向队列末端

  T *r;                     //指向一个未读取元素

  AtomicPointer<T>     readPre;  //指向一个预读元素 为NULL时读取休眠     
};


}

#endif // _ZZ_PIPE_H
