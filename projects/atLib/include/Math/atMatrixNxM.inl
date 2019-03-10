
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

#include "atAssert.h"

template <typename T> atMatrixNxM<T> atMatrixNxM<T>::Identity()
{
  atAssert(m_rows == m_columns, "rows and col must be equal!");
  atMatrixNxM<T> ret;
  for (int64_t i = 0; i < m_rows; ++i)
    ret[i + i * m_columns] = (T)1;
  return ret;
}

template <typename T> atMatrixNxM<T> atMatrixNxM<T>::Transpose() const
{
  atMatrixNxM<T> ret;
  for (int64_t r = 0; r < m_rows; ++r)
    for (int64_t c = 0; c < m_columns; ++c)
      ret.m_data[r + c * m_rows] = m_data[c + r * m_columns];
  return ret;
}

template <typename T> T atMatrixNxM<T>::Determinate() const
{
  if (m_rows == 2)
    return m_data[0] * m_data[3] - m_data[1] * m_data[2];
  T ret = 0;
  for (int64_t c = 0; c < m_columns; ++c)
    ret += m_data[c] * LowOrderMatrix<max(2, m_rows - 1)>(c, 0).Determinate() * ((c % 2 == 0) ? 1 : -1);
  return ret;
}

template <typename T> atMatrixNxM<T> atMatrixNxM<T>::Cofactors() const
{
  atAssert(m_rows == m_columns, "rows and col must be equal!");
  atAssert(m_rows >= 2, "Matrix must be at least 2x2!");
  if (m_rows == 2)
    return atMatrixNxM<T>({ m_data[0], -m_data[1], -m_data[2], m_data[3] });
  atMatrixNxM<T> ret;
  for (int64_t r = 0; r < m_rows; ++r)
    for (int64_t c = 0; c < m_columns; ++c)
      ret[c + r * m_columns] = LowOrderMatrix<max(2, m_rows - 1)>(c, r).Determinate() * (((r + c) % 2 == 0) ? 1 : -1);
  return ret;
}

template <typename T> atMatrixNxM<T> atMatrixNxM<T>::LowOrderMatrix(const int64_t c, const int64_t r, const int64_t dim) const
{
  atMatrixNxM<T> ret(dim, dim);
  for (int64_t r2 = 0; r2 < dim; ++r2)
    for (int64_t c2 = 0; c2 < dim; ++c2)
      ret.m_data[c2 + r2 * dim] = m_data[(c2 >= c ? c2 + 1 : c2) + (r2 >= r ? r2 + 1 : r2) * m_columns];
  return ret;
}

template <typename T> template<typename T2> atMatrixNxM<T> atMatrixNxM<T>::Mul(const atMatrixNxM<T2> &rhs) const
{
  atMatrixNxM<T,> ret;
  for (int64_t r = 0; r < m_rows; ++r)
    for (int64_t c = 0; c < rhs.col; ++c)
      for (int64_t i = 0; i < rhs.row; ++i)
        ret[c + r * col2] += m_data[i + r * m_columns] * (T)rhs.m_data[c + i * col2];
  return ret;
}

template <typename T>  template<typename T2> atMatrixNxM<T> atMatrixNxM<T>::Add(const atMatrixNxM<T2> &rhs) const
{
  atMatrixNxM<T> ret = *this;
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] += (T)rhs.m_data[i];
  return ret;
}

template <typename T> template <typename T2> atMatrixNxM<T> atMatrixNxM<T>::Sub(const atMatrixNxM<T2> &rhs) const
{
  atMatrixNxM<T> ret = *this;
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] -= (T)rhs.m_data[i];
  return ret;
}

template <typename T>  atMatrixNxM<T> atMatrixNxM<T>::Mul(const T &rhs) const
{
  atMatrixNxM<T> ret;
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] = (T)m_data[i] * rhs;
  return ret;
}

template <typename T> atMatrixNxM<T> atMatrixNxM<T>::Add(const T &rhs) const
{
  atMatrixNxM<T> ret;
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] = (T)m_data[i] + rhs;
  return ret;
}

template <typename T> atMatrixNxM<T>::atMatrixNxM(const std::initializer_list<T> &list)
{
  for (int64_t i = 0; i < (int64_t)list.size() && i < m_rows * m_columns; ++i)
    m_data[i] = (T)(*(list.begin() + i));
}

template <typename T> template <typename T2> const atMatrixNxM<T>& atMatrixNxM<T>::operator=(const atMatrixNxM<T2> &rhs)
{
  for (int64_t i = 0; i < m_rows * m_columns; ++i)
    m_data[i] = (T)rhs.m_data[i];
  return *this;
}

template <typename T> template <typename T2> atMatrixNxM<T>::atMatrixNxM(const atMatrixNxM<T2> &copy)
{
  for (int64_t i = 0; i < m_rows * m_columns; ++i)
    m_data[i] = (T)copy.m_data[i];
}

template <typename T> atMatrixNxM<T> atMatrixNxM<T>::Mul(const atMatrixNxM<T> &rhs) const { return Mul<T>(rhs); }
template <typename T> atMatrixNxM<T> atMatrixNxM<T>::operator*(const atMatrixNxM<T>& rhs) const { return Mul<T>(rhs); }
template <typename T> template <typename T2> atMatrixNxM<T> atMatrixNxM<T>::operator*(const atMatrixNxM<T2>& rhs) const { return Mul<T2>(rhs); }

template <typename T> template <typename T2> atMatrixNxM<T> atMatrixNxM<T>::operator/(const T2 &rhs) const { return Mul<T2>((T)1 / rhs); }
template <typename T> template <typename T2> atMatrixNxM<T> atMatrixNxM<T>::operator+(const atMatrixNxM<T2>& rhs) const { return Add(rhs); }
template <typename T> template <typename T2> atMatrixNxM<T> atMatrixNxM<T>::operator-(const atMatrixNxM<T2>& rhs) const { return Sub(rhs); }

template <typename T> atMatrixNxM<T> atMatrixNxM<T>::Inverse() const { return ((m_rows == 2 && m_columns == 2) ? atMatrixNxM<T>({ m_data[3], -m_data[1], -m_data[2], m_data[0] }) : Cofactors().Transpose()).Mul((T)1 / Determinate()); }
template <typename T> const atMatrixNxM<T>& atMatrixNxM<T>::operator=(const atMatrixNxM<T> &rhs) { memcpy(m_data, rhs.m_data, (int64_t)sizeof(T) * m_rows * m_columns); return *this; }

template <typename T> atMatrixNxM<T>::atMatrixNxM(atMatrixNxM<T> &&move) { memcpy(m_data, move.m_data, (int64_t)sizeof(T) * m_rows * m_columns); move = atMatrixNxM(); }
template <typename T> atMatrixNxM<T>::atMatrixNxM(const atMatrixNxM<T> &copy) { memcpy(m_data, copy.m_data, (int64_t)sizeof(T) * m_rows * m_columns); }

template <typename T> bool atMatrixNxM<T>::operator==(const atMatrixNxM<T> &rhs) const { return memcmp(m_data, rhs.m_data, m_rows * m_columns) == 0; }
template <typename T> bool atMatrixNxM<T>::operator!=(const atMatrixNxM<T>& rhs) const { return !(*this == rhs); }
template <typename T> const T & atMatrixNxM<T>::operator[](const int64_t index) const { return m_data[index]; }
template <typename T> atMatrixNxM<T> atMatrixNxM<T>::Sub(const T &rhs) const { return Add(-rhs); }
template <typename T> T& atMatrixNxM<T>::operator[](const int64_t index) { return m_data[index]; }
template <typename T> atMatrixNxM<T>::atMatrixNxM() { memset(m_data, 0, sizeof(T) * m_rows * m_columns); }
