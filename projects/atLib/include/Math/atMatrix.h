
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

#ifndef atMatrix_h__
#define atMatrix_h__

#include <minmax.h>
#include "atTypes.h"

template <typename T, int64_t col, int64_t row> class atMatrix
{
public:
  atMatrix();
  atMatrix(const std::initializer_list<T> &list);
  atMatrix(const atMatrix<T, col, row> &copy);
  atMatrix(atMatrix<T, col, row> &&move);
  template <typename T2> atMatrix(const atMatrix<T2, col, row> &copy);

  static atMatrix<T, col, row> Identity();
  atMatrix<T, row, col> Transpose() const;
  atMatrix<T, col, row> Cofactors() const;
  atMatrix<T, col, row> Inverse() const;

  T Determinate() const;

  template <typename T2, int64_t col2, int64_t row2> atMatrix<T, row, col2> Mul(const atMatrix<T2, col2, row2> &rhs) const;
  template <typename T2, int64_t col2, int64_t row2> atMatrix<T, row, col2> operator*(const atMatrix<T2, col2, row2> &rhs) const;

  template <int64_t col2, int64_t row2> atMatrix<T, row, col2> Mul(const atMatrix<T, col2, row2> &rhs) const;
  template <int64_t col2, int64_t row2> atMatrix<T, row, col2> operator*(const atMatrix<T, col2, row2> &rhs) const;

  template <typename T2> atMatrix<T, col, row> Add(const atMatrix<T2, col, row> &rhs) const;
  template <typename T2> atMatrix<T, col, row> Sub(const atMatrix<T2, col, row> &rhs) const;

  atMatrix<T, col, row> Mul(const T &rhs) const;
  atMatrix<T, col, row> Sub(const T &rhs) const;
  atMatrix<T, col, row> Add(const T &rhs) const;

  template <typename T2> atMatrix<T, col, row> operator/(const T2 &rhs) const;
  template <typename T2> atMatrix<T, col, row> operator+(const atMatrix<T2, col, row> &rhs) const;
  template <typename T2> atMatrix<T, col, row> operator-(const atMatrix<T2, col, row> &rhs) const;

  bool operator==(const atMatrix<T, col, row> &rhs) const;
  bool operator!=(const atMatrix<T, col, row> &rhs) const;
  const atMatrix<T, col, row> &operator=(const atMatrix<T, col, row> &copy);
  template <typename T2> const atMatrix<T, col, row>& operator=(const atMatrix<T2, col, row> &copy);


  T& operator[](const int64_t index);
  const T& operator[](const int64_t index) const;

  T m_data[row * col];

  const int64_t m_rows = row;
  const int64_t m_columns = col;

protected:
  template <int64_t dim> atMatrix<T, dim, dim> LowOrderMatrix(const int64_t x, const int64_t y) const;
};

#include "atMatrix.inl"
#endif // atMatrix_h__
