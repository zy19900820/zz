#ifndef _ZZ_QUEUE_H
#define _ZZ_QUEUE_H

#include "zz_atomic_pointer.h"

namespace zz
{

// 数组链表 简单模仿stl alloc的内存分配
template<class T, int N=16>
class ZZQueue : public NonCopyAble
{
public:
  ZZQueue()
  {
    begin_chunk_ = reinterpret_cast<Chunk *>(malloc(sizeof(Chunk)));
    begin_pos_ = 0;
    end_chunk_ = begin_chunk_;
    end_pos_ = begin_pos_;
    insert_chunk_ = NULL;
    insert_pos_ = 0;
  }

  ~ZZQueue()
  {
    while (1)
    {
      if (begin_chunk_ == end_chunk_)
      {
        free(begin_chunk_);
        break;
      }
      Chunk *o = begin_chunk_;
      begin_chunk_ = begin_chunk_->next;
      free(o);
    }
    // 释放缓冲 没有则free(NULL)
    Chunk *sc = spare_chunk_.xchg(NULL);
    free(sc);
  }

  T& front()
  {
    return begin_chunk_->values[begin_pos_];
  }

  T& back()
  {
    return insert_chunk_->values[insert_pos_];
  }

  void push()
  {
    insert_chunk_ = end_chunk_;
    insert_pos_ = end_pos_;

    if (++end_pos_ != N)
      return;

    Chunk *sc = spare_chunk_.xchg(NULL);
    if (sc)
    {
      end_chunk_->next = sc;
      sc->pre = end_chunk_;
    }
    else
    {
      end_chunk_->next = reinterpret_cast<Chunk *>(malloc(sizeof(Chunk)));
      end_chunk_->next->pre = end_chunk_;
    }
    end_chunk_ = end_chunk_->next;
    end_pos_ = 0;
  }

  void pop()
  {
    if (++begin_pos_ == N)
    {
      Chunk *o = begin_chunk_;
      begin_chunk_ = begin_chunk_->next;
      begin_chunk_->pre = NULL;
      begin_pos_ = 0;

      // 缓存被释放的内存
      Chunk *cs = spare_chunk_.xchg(o);
      free(cs);
    }
  }

private:
  struct Chunk
  {
    T value[N];
    Chunk *pre;
    Chunk *next;
  };

  Chunk *begin_chunk_;
  int32_t begin_pos_;

  Chunk *end_chunk_;
  int32_t end_pos_;

  // 插入的区块
  Chunk *insert_chunk_;
  int32_t insert_pos_;

  // 缓存最近释放的区块 提高性能
  AtomicPointer<Chunk> spare_chunk_;
};

}

#endif // _ZZ_QUEUE_H
