
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

#ifndef atIterator_h__
#define atIterator_h__

#include "atTypes.h"

template<typename T> class atIterator
{
public:
  atIterator(T *pStart, const int64_t len);

  T* begin();
  T* end();

  T* m_pBegin;
  T* m_pEnd;
};


template<typename T> class atIteratorConst
{
public:
  atIteratorConst(const T *pStart, const int64_t len);

  const T* begin() const;
  const T* end() const;

  const T* m_pBegin;
  const T* m_pEnd;
};

template<typename T> atIterator<T> atIterate(T *pStart, int64_t len);
template<typename T> atIteratorConst<T> atIterate(const T *pStart, int64_t len);

#include "atIterator.inl"

#endif // atIterator_h__
