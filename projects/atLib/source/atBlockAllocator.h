
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#ifndef atBlockAllocator_h__
#define atBlockAllocator_h__

#include "atAllocator.h"

template <typename T> struct atBlock
{
  int64_t size;
  int64_t offsets[10];
  T *pData;
  T data[block_size];
};

template <typename T, int64_t block_size> class atBlockAllocator : atAllocator<atBlock<T>*>
{
public:
  atBlockAllocator();
  ~atBlockAllocator();

  virtual T* alloc(const int64_t count) override;
  virtual T* realloc(T* pBlock, const int64_t count) override;
  virtual void free(T*pBlock) override;

protected:
  virtual alloc_blocks(const int64_t count);
  virtual free_block(const int64_t index);

  static int64_t m_nBlocks;
  static atBlock<T> **m_ppBlocks;

  static atAllocator<T> m_allocator;
};

#include "atBlockAllocator.inl"
#endif // atBlockAllocator_h__