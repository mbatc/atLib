
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

#ifndef atAllocator_h__
#define atAllocator_h__

#include "atTypes.h"
#include "atAlloc.h"

template <typename T> class atAllocator
{
public:
  atAllocator() {}
  ~atAllocator() {}

  virtual T* alloc(const int64_t count) { atAlloc(count * sizeof(T)); }
  virtual T* realloc(T* pBlock, const int64_t newCount) { atRealloc(pBlock, newCount * sizeof(T)); }
  virtual void free(T* pBlock) { atFree(pBlock); }

protected:
  virtual void destroy();

  void add_allocation(const int64_t address);
  void remove_allocation(const int64_t address);
  int64_t get_allocation(const int64_t index);

private:
  int64_t *get_bucket(const int64_t address);
  void rehash(const int64_t bucketCount);
  static int64_t **m_ppAllocations;
  static int64_t m_nBuckets;
  static int64_t m_bucketSize;
  static int64_t m_allocCount;
};

#include "atAllocator.inl"
#endif // atAllocator_h__
