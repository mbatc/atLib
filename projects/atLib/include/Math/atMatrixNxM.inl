
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

#include <algorithm>

template<typename T> atMatrixNxM<T> atMatrixNxM<T>::Identity(const int64_t &cols, const int64_t &rows)
{
  atAssert(rows == cols, "rows and cols must be equal!");
  atMatrixNxM<T> ret(cols, rows);
  for (int64_t i = 0; i < rows; ++i)
    at(i, i) = (T)1;
  return ret;
}

template<typename T> atMatrixNxM<T> atMatrixNxM<T>::Transpose() const
{
  atMatrixNxM<T> ret(m_rows, m_columns);
  for (int64_t r = 0; r < m_rows; ++r)
    for (int64_t c = 0; c < m_columns; ++c)
      ret.at(c, r) = at(r, c);
  return ret;
}

template<typename T> T atMatrixNxM<T>::Determinate() const
{
  if (m_rows == 2)
    return m_data[0] * m_data[3] - m_data[1] * m_data[2];
  T ret = 0;
  for (int64_t c = 0; c < m_columns; ++c)
    ret += m_data[c] * LowOrderMatrix(c, 0, std::max(int64_t(2), m_rows - 1)).Determinate() * ((c % 2 == 0) ? 1 : -1);
  return ret;
}

template<typename T> atMatrixNxM<T> atMatrixNxM<T>::Cofactors() const
{
  atAssert(m_rows == m_columns, "rows and col must be equal!");
  atAssert(m_rows >= 2, "Matrix must be at least 2x2!");
  if (m_rows == 2)
    return atMatrixNxM<T>({ m_data[0], -m_data[1], -m_data[2], m_data[3] });
  atMatrixNxM<T> ret;
  for (int64_t r = 0; r < m_rows; ++r)
    for (int64_t c = 0; c < m_columns; ++c)
      ret.at(r, c) = LowOrderMatrix(c, r, std::max(int64_t(2), m_rows - 1)).Determinate() * (((r + c) % 2 == 0) ? 1 : -1);
  return ret;
}

template<typename T> atMatrixNxM<T> atMatrixNxM<T>::LowOrderMatrix(const int64_t c, const int64_t r, const int64_t dim) const
{
  atMatrixNxM<T> ret(dim, dim);
  for (int64_t r2 = 0; r2 < dim; ++r2)
    for (int64_t c2 = 0; c2 < dim; ++c2)
      ret.at(r2, c2) = m_data[(c2 >= c ? c2 + 1 : c2) + (r2 >= r ? r2 + 1 : r2) * m_columns];
  return ret;
}

template<typename T> template<typename T2> atMatrixNxM<T> atMatrixNxM<T>::Mul(const atMatrixNxM<T2> &rhs) const
{
  atMatrixNxM<T> ret(rhs.m_columns, m_rows);
  for (int64_t r = 0; r < m_rows; ++r)
    for (int64_t c = 0; c < rhs.m_columns; ++c)
      for (int64_t i = 0; i < rhs.m_rows; ++i)
        ret.at(r, c) += at(r, i) * (T)rhs.at(i, c);
  return ret;
}

template<typename T> template<typename T2> atMatrixNxM<T> atMatrixNxM<T>::Add(const atMatrixNxM<T2> &rhs) const
{
  atMatrixNxM<T> ret = *this;
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] += (T)rhs.m_data[i];
  return ret;
}

template<typename T> template<typename T2> atMatrixNxM<T> atMatrixNxM<T>::Sub(const atMatrixNxM<T2> &rhs) const
{
  atMatrixNxM<T> ret = *this;
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] -= (T)rhs.m_data[i];
  return ret;
}

template<typename T>  atMatrixNxM<T> atMatrixNxM<T>::Mul(const T &rhs) const
{
  atMatrixNxM<T> ret(m_columns, m_rows);
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] = (T)m_data[i] * rhs;
  return ret;
}

template<typename T> atMatrixNxM<T> atMatrixNxM<T>::Add(const T &rhs) const
{
  atMatrixNxM<T> ret(m_columns, m_rows);
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] = (T)m_data[i] + rhs;
  return ret;
}

template<typename T> inline atMatrixNxM<T> atMatrixNxM<T>::Apply(T(*func)(const T &))
{
  atMatrixNxM<T> ret = *this;
  for (T &val : ret.m_data)
    val = func(val);
  return ret;
}

template<typename T> inline atMatrixNxM<T>::atMatrixNxM(int64_t _col, int64_t _row)
{
  m_columns = _col;
  m_rows = _row;
  m_data.reserve(_col * _row);
  m_data.resize(_col * _row);
}

template<typename T> atMatrixNxM<T>::atMatrixNxM(int64_t _col, int64_t _row, const std::initializer_list<T> &list)
  : atMatrixNxM<T>(_col, _row)
{
  for (int64_t i = 0; i < (int64_t)list.size() && i < m_rows * m_columns; ++i)
    m_data[i] = (T)(*(list.begin() + i));
}

template<typename T> template <typename T2> const atMatrixNxM<T>& atMatrixNxM<T>::operator=(const atMatrixNxM<T2> &rhs)
{
  for (int64_t i = 0; i < m_rows * m_columns; ++i)
    m_data[i] = (T)rhs.m_data[i];
  return *this;
}

template<typename T> template <typename T2> atMatrixNxM<T>::atMatrixNxM(const atMatrixNxM<T2> &copy)
{
  for (int64_t i = 0; i < m_rows * m_columns; ++i)
    m_data[i] = (T)copy.m_data[i];
}

template<typename T> atMatrixNxM<T> atMatrixNxM<T>::Mul(const atMatrixNxM<T> &rhs) const { return Mul<T>(rhs); }
template<typename T> atMatrixNxM<T> atMatrixNxM<T>::operator*(const atMatrixNxM<T>& rhs) const { return Mul<T>(rhs); }
template<typename T> template <typename T2> atMatrixNxM<T> atMatrixNxM<T>::operator*(const atMatrixNxM<T2>& rhs) const { return Mul<T2>(rhs); }

template<typename T> template <typename T2> atMatrixNxM<T> atMatrixNxM<T>::operator/(const T2 &rhs) const { return Mul<T2>((T)1 / rhs); }
template<typename T> template <typename T2> atMatrixNxM<T> atMatrixNxM<T>::operator+(const atMatrixNxM<T2>& rhs) const { return Add(rhs); }
template<typename T> template <typename T2> atMatrixNxM<T> atMatrixNxM<T>::operator-(const atMatrixNxM<T2>& rhs) const { return Sub(rhs); }

template<typename T> atMatrixNxM<T> atMatrixNxM<T>::Inverse() const { return ((m_rows == 2 && m_columns == 2) ? atMatrixNxM<T>({ m_data[3], -m_data[1], -m_data[2], m_data[0] }) : Cofactors().Transpose()).Mul((T)1 / Determinate()); }
template<typename T> const atMatrixNxM<T>& atMatrixNxM<T>::operator=(const atMatrixNxM<T> &rhs) { m_data = rhs.m_data; m_rows = rhs.m_rows; m_columns = rhs.m_columns; return *this; }

template<typename T> atMatrixNxM<T>::atMatrixNxM(atMatrixNxM<T> &&move) { m_data = move.m_data, m_columns = move.m_columns; m_rows = move.m_rows; }
template<typename T> atMatrixNxM<T>::atMatrixNxM(const atMatrixNxM<T> &copy) { m_data = copy.m_data; m_rows = copy.m_rows; m_columns = copy.m_columns; }

template<typename T> bool atMatrixNxM<T>::operator==(const atMatrixNxM<T> &rhs) const { return memcmp(m_data, rhs.m_data, m_rows * m_columns) == 0; }
template<typename T> bool atMatrixNxM<T>::operator!=(const atMatrixNxM<T>& rhs) const { return !(*this == rhs); }
template<typename T> const T& atMatrixNxM<T>::operator[](const int64_t index) const { return m_data[index]; }
template<typename T> T& atMatrixNxM<T>::operator()(const int64_t row, const int64_t col) { return at(row, col); }
template<typename T> const T& atMatrixNxM<T>::operator()(const int64_t row, const int64_t col) const { return at(row, col); }
template<typename T> int64_t atMatrixNxM<T>::Rows() const { return m_rows; }
template<typename T> int64_t atMatrixNxM<T>::Columns() const { return m_columns; }
template<typename T> atMatrixNxM<T> atMatrixNxM<T>::Sub(const T &rhs) const { return Add(-rhs); }
template<typename T> T &atMatrixNxM<T>::at(const int64_t row, const int64_t col) { return m_data[col + row * m_columns]; }
template<typename T> const T &atMatrixNxM<T>::at(const int64_t row, const int64_t col) const { return m_data[col + row * m_columns]; }
template<typename T> T& atMatrixNxM<T>::operator[](const int64_t index) { return m_data[index]; }

template<typename T> inline int64_t atStreamRead(atReadStream *pStream, atMatrixNxM<T> *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    ret += atStreamRead(pStream, &pData[i].m_rows, 1);
    ret += atStreamRead(pStream, &pData[i].m_columns, 1);
    ret += atStreamRead(pStream, &pData[i].m_data, 1);
  }
  return ret;
}

template<typename T> inline int64_t atStreamWrite(atWriteStream *pStream, const atMatrixNxM<T> *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    ret += atStreamWrite(pStream, &pData[i].m_rows, 1);
    ret += atStreamWrite(pStream, &pData[i].m_columns, 1);
    ret += atStreamWrite(pStream, &pData[i].m_data, 1);
  }
  return ret;
}
