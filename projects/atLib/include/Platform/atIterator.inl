
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

template<typename T> atIterator<T>::atIterator(T *pStart, const int64_t len)
  : m_pBegin(pStart)
{
  m_pEnd = pStart + atMax(len, 0);
}

template<typename T> atIteratorConst<T>::atIteratorConst(const T *pStart, const int64_t len)
  : m_pBegin(pStart)
{
  m_pEnd = pStart + atMax(len, 0);
}

template<typename T> T* atIterator<T>::begin() { return m_pBegin; }
template<typename T> T* atIterator<T>::end() { return m_pEnd; }
template<typename T> const T* atIteratorConst<T>::begin() const { return m_pBegin; }
template<typename T> const T* atIteratorConst<T>::end() const { return m_pEnd; }

template<typename T> atIterator<T> atIterate(T *pStart, int64_t len) { return atIterator<T>(pStart, len); }
template<typename T> atIteratorConst<T> atIterate(const T *pStart, int64_t len) { return atIteratorConst<T>(pStart, len); }
