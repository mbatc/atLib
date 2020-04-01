
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

#ifndef atMatrixNxM_h__
#define atMatrixNxM_h__

#include "atMinMax.h"
#include "atTypes.h"
#include "atVector.h"

template <typename T> class atMatrixNxM
{
public:
  atMatrixNxM(int64_t _col = 0, int64_t _row = 0);
  atMatrixNxM(int64_t _col, int64_t _row, const std::initializer_list<T> &list);
  atMatrixNxM(const atMatrixNxM<T> &copy);
  atMatrixNxM(atMatrixNxM<T> &&move);
  template <typename T2> atMatrixNxM(const atMatrixNxM<T2> &copy);

  static atMatrixNxM<T> Identity(const int64_t &cols, const int64_t &rows);
  atMatrixNxM<T> Transpose() const;
  atMatrixNxM<T> Cofactors() const;
  atMatrixNxM<T> Inverse() const;

  T Determinate() const;

  template <typename T2> atMatrixNxM<T> Mul(const atMatrixNxM<T2> &rhs) const;
  template <typename T2> atMatrixNxM<T> operator*(const atMatrixNxM<T2> &rhs) const;

  atMatrixNxM<T> Mul(const atMatrixNxM<T> &rhs) const;
  atMatrixNxM<T> operator*(const atMatrixNxM<T> &rhs) const;

  template <typename T2> atMatrixNxM<T> Add(const atMatrixNxM<T2> &rhs) const;
  template <typename T2> atMatrixNxM<T> Sub(const atMatrixNxM<T2> &rhs) const;

  atMatrixNxM<T> Mul(const T &rhs) const;
  atMatrixNxM<T> Sub(const T &rhs) const;
  atMatrixNxM<T> Add(const T &rhs) const;
  atMatrixNxM<T> Apply(T (*func)(const T &));

  atMatrixNxM<T> LowOrderMatrix(const int64_t x, const int64_t y, const int64_t dim) const;

  template <typename T2> atMatrixNxM<T> operator/(const T2 &rhs) const;
  template <typename T2> atMatrixNxM<T> operator+(const atMatrixNxM<T2> &rhs) const;
  template <typename T2> atMatrixNxM<T> operator-(const atMatrixNxM<T2> &rhs) const;

  bool operator==(const atMatrixNxM<T> &rhs) const;
  bool operator!=(const atMatrixNxM<T> &rhs) const;
  const atMatrixNxM<T> &operator=(const atMatrixNxM<T> &copy);
  template <typename T2> const atMatrixNxM<T>& operator=(const atMatrixNxM<T2> &copy);


  T& operator[](const int64_t index);
  const T& operator[](const int64_t index) const;

  atVector<T> m_data;
  int64_t m_rows;
  int64_t m_columns;
};

template <typename T> int64_t atStreamRead(atReadStream *pStream, atMatrixNxM<T> *pData, const int64_t count);
template <typename T> int64_t atStreamWrite(atWriteStream *pStream, const atMatrixNxM<T> *pData, const int64_t count);

#include "atMatrixNxM.inl"

#endif // atMatrixNxM_h__
