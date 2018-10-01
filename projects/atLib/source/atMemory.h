
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

#ifndef _atMemory_h__
#define _atMemory_h__

#include "atAlloc.h"
#include <type_traits>
#include <forward_list>

//*********************
// Function Definitions

// Internal Functions
template <class T, typename... Args> T* atInternal_New(const int64_t count, Args&&... args);
template <class T> void atDelete(T* pBlock);

// External Functions
template <class T, typename... Args> T* atNew(Args&&... args);
template <class T, typename... Args> T* atNewArray(const int64_t size);
template <class T, typename... Args> T* atNewArray(const int64_t size, const T &copy);

// Will Construct/Destruct object of they are non primitive types
template <class T, typename... Args> typename std::enable_if<std::is_constructible<T, Args...>::value>::type atConstruct(T *pVal, Args... args);
template <class T, typename... Args> typename std::enable_if<!std::is_constructible<T, Args...>::value>::type atConstruct(T *pVal, Args... args) { atUnused(pVal, args...); }

template <class T, typename... Args> typename std::enable_if<std::is_constructible<T, Args...>::value>::type atConstructArray(T *pVal, const int64_t count, Args... args);
template <class T, typename... Args> typename std::enable_if<!std::is_constructible<T, Args...>::value>::type atConstructArray(T *pVal, const int64_t count, Args... args) { atUnused(pVal, count, args...); }

template <class T> typename std::enable_if<std::is_destructible<T>::value>::type atDestruct(T *pVal);
template <class T> typename std::enable_if<!std::is_destructible<T>::value>::type atDestruct(T *pVal) { atUnused(pVal); }
template <class T> typename std::enable_if<std::is_destructible<T>::value>::type atDestructArray(T *pVal, const int64_t count);
template <class T> typename std::enable_if<!std::is_destructible<T>::value>::type atDestructArray(T *pVal, const int64_t count) { atUnused(pVal, count); }

//*************************
// Function Implementations

template<class T, typename... Args>
inline T* atInternal_New(const int64_t count, Args&&... args)
{
  const int64_t size = count * sizeof(T);
  void *pBlock = atAlloc(size + sizeof(int64_t));
  memcpy(pBlock, &count, sizeof(int64_t));
  T* pPos = (T*)((int64_t)pBlock + sizeof(int64_t));
  T* pStart = pPos;

  // construct new objects
  while(pPos - pStart != count)
    atConstruct(pPos++, std::forward<Args>(args)...);

  return pStart;
}

template <class T> 
inline void atDelete(T* pBlock)
{
  void *pActualAlloc = (int64_t*)pBlock - 1;
  const int64_t count = *(int64_t*)pActualAlloc;
  const T* pStart = pBlock;

  // destruct objects
  while (pBlock - pStart != count)
    atDestruct(pBlock++);
  atFree(pActualAlloc);
}

template <class T> 
inline typename std::enable_if<std::is_destructible<T>::value>::type atDestructArray(T *pVal, const int64_t count)
{
  for (int64_t i = 0; i < count; ++i)
    atDestruct(&pVal[i]);
}

template <class T, typename... Args> 
inline typename std::enable_if<std::is_constructible<T, Args...>::value>::type atConstructArray(T *pVal, const int64_t count, Args... args)
{
  for (int64_t i = 0; i < count; ++i)
    atConstruct<T, Args...>(pVal + i, std::forward<Args>(args)...);
}

template <class T, typename... Args> inline T* atNew(Args&&... args) { return atInternal_New<T>(1, std::forward<Args>(args)...);  }
template <class T> inline T* atNewArray(const int64_t size) { return atInternal_New(size); }
template <class T> inline T* atNewArray(const int64_t size, const T &copy) { return atInternal_New(size, copy); }
template <class T, typename... Args> inline typename std::enable_if<std::is_constructible<T, Args...>::value>::type atConstruct(T *pVal, Args... args) { new(pVal) T(std::forward<Args>(args)...); }
template <class T> inline typename std::enable_if<std::is_destructible<T>::value>::type atDestruct(T *pVal) { pVal->~T(); }

#endif

