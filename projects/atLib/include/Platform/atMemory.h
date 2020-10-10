
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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
template<typename T, typename... Args> T* atInternal_New(const int64_t count, Args&&... args);
template<typename T> void atDelete(T *pBlock);

#define atNew(...) new (_atAllocTrace(sizeof(__VA_ARGS__), __LINE__, __FILE__, "")) __VA_ARGS__
#define atNewArray(count, ...) new (_atAllocTrace(sizeof(__VA_ARGS__) * count, __LINE__, __FILE__, "")) __VA_ARGS__[count]
#define atDelete(pBlock) (delete pBlock, atReleaseMemRef((void*)pBlock))

// Will Construct/Destruct object of they are non primitive types
template<typename T, typename... Args> void atConstruct(T *pVal, Args&&... args);
template<typename T, typename... Args> void atConstructArray(T *pVal, const int64_t count, Args&&... args);
template<typename T> void atConstructArray(T *pVal, const int64_t count);

template<typename T> typename std::enable_if<std::is_destructible<T>::value>::type atDestruct(T *pVal);
template<typename T> typename std::enable_if<!std::is_destructible<T>::value>::type atDestruct(T *pVal) { atUnused(pVal); }
template<typename T> typename std::enable_if<std::is_destructible<T>::value>::type atDestructArray(T *pVal, const int64_t count);
template<typename T> typename std::enable_if<!std::is_destructible<T>::value>::type atDestructArray(T *pVal, const int64_t count) { atUnused(pVal, count); }

//*************************
// Function Implementations

template<typename T> typename std::enable_if<std::is_destructible<T>::value>::type atDestructArray(T *pVal, const int64_t count)
{
  if (std::is_integral<T>::value)
    return;
  for (int64_t i = 0; i < count; ++i)
    atDestruct(&pVal[i]);
}

template<typename T, typename... Args> void atConstructArray(T *pVal, const int64_t count, Args&&... args)
{
  for (int64_t i = 0; i < count; ++i)
    atConstruct<T, Args...>(pVal + i, std::forward<Args>(args)...);
}

template<typename T> inline void atConstructArray(T *pVal, const int64_t count)
{
  if (std::is_integral<T>::value)
    return;
  for (int64_t i = 0; i < count; ++i)
    atConstruct(pVal + i);
}

template<typename T> inline typename std::enable_if<std::is_destructible<T>::value>::type atDestruct(T *pVal)
{
  if (std::is_integral<T>::value)
    return;
  pVal->~T();
}

template<typename T, typename... Args> inline void atConstruct(T *pVal, Args&&... args) { new(pVal) T(std::forward<Args>(args)...); }

#endif
