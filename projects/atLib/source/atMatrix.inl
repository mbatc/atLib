
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

template <typename T, int64_t col, int64_t row> atMatrix<T, col, row> atMatrix<T, col, row>::Identity()
{
  atAssert(row == col, "rows and col must be equal!");
  atMatrix<T, col, row> ret;
  for (int64_t i = 0; i < row; ++i)
    ret[i + i * col] = (T)1;
  return ret;
}

template <typename T, int64_t col, int64_t row> atMatrix<T, row, col> atMatrix<T, col, row>::Transpose() const
{
  atMatrix<T, col, row> ret;
  for (int64_t r = 0; r < row; ++r)
    for (int64_t c = 0; c < col; ++c)
      ret.m_data[r + c * row] = m_data[c + r * col];
  return ret;
}

template <typename T, int64_t col, int64_t row> T atMatrix<T, col, row>::Determinate() const
{
  if (row == 2)
    return m_data[0] * m_data[3] - m_data[1] * m_data[2];
  T ret = 0;
  for (int64_t c = 0; c < col; ++c)
    ret += m_data[c] * LowOrderMatrix<max(2, row - 1)>(c, 0).Determinate() * ((c % 2 == 0) ? 1 : -1);
  return ret;
}

template <typename T, int64_t col, int64_t row> atMatrix<T, col, row> atMatrix<T, col, row>::Cofactors() const
{
  atAssert(row == col, "rows and col must be equal!");
  atAssert(row >= 2, "Matrix must be at least 2x2!");
  if(row == 2)
    return atMatrix<T, row, col>({ m_data[0], -m_data[1], -m_data[2], m_data[3] });
  atMatrix<T, row, col> ret;
  for (int64_t r = 0; r < row; ++r)
    for (int64_t c = 0; c < col; ++c)
      ret[c + r * col] = LowOrderMatrix<max(2, row - 1)>(c, r).Determinate() * (((r + c) % 2 == 0) ? 1 : -1);
  return ret;
}

template <typename T, int64_t col, int64_t row> template<int64_t dim> atMatrix<T, dim, dim> atMatrix<T, col, row>::LowOrderMatrix(const int64_t c, const int64_t r) const
{
  atMatrix<T, dim, dim> ret;
  for (int64_t r2 = 0; r2 < dim; ++r2)
    for (int64_t c2 = 0; c2 < dim; ++c2)
      ret.m_data[c2 + r2 * dim] = m_data[(c2 >= c ? c2 + 1 : c2) + (r2 >= r ? r2 + 1 : r2) * col];
  return ret;
}

template <typename T, int64_t col, int64_t row> template<typename T2, int64_t col2, int64_t row2> atMatrix<T, row, col2> atMatrix<T, col, row>::Mult(const atMatrix<T2, col2, row2> &rhs) const
{
  atMatrix<T, row, col2> ret;
  for (int64_t r = 0; r < row; ++r)
    for (int64_t c = 0; c < col2; ++c)
      for (int64_t i = 0; i < row2; ++i)
        ret[c + r * col2] += m_data[i + r * col] * (T)rhs.m_data[c + i * col2];
  return ret;
}

template <typename T, int64_t col, int64_t row>  template<typename T2> atMatrix<T, col, row> atMatrix<T, col, row>::Add(const atMatrix<T2, col, row> &rhs) const
{
  atMatrix<T, col, row> ret = *this;
  for (int64_t i = 0; i < col * row; ++i)
      ret[i] += (T)rhs.m_data[i];
  return ret;
}

template <typename T, int64_t col, int64_t row> template <typename T2> atMatrix<T, col, row> atMatrix<T, col, row>::Subtract(const atMatrix<T2, col, row> &rhs) const
{
  atMatrix<T, col, row> ret = *this;
  for (int64_t i = 0; i < col * row; ++i) 
    ret[i] -= (T)rhs.m_data[i];
  return ret;
}

template <typename T, int64_t col, int64_t row>  atMatrix<T, col, row> atMatrix<T, col, row>::Mult(const T &rhs) const
{
  atMatrix<T, col, row> ret;
  for (int64_t i = 0; i < col * row; ++i) 
    ret[i] = (T)m_data[i] * rhs;
  return ret;
}

template <typename T, int64_t col, int64_t row> atMatrix<T, col, row> atMatrix<T, col, row>::Add(const T &rhs) const
{
  atMatrix<T, col, row> ret;
  for (int64_t i = 0; i < col * row; ++i)
      ret[i] = (T)m_data[i] + rhs;
  return ret;
}

template <typename T, int64_t col, int64_t row> atMatrix<T, col, row>::atMatrix(const std::initializer_list<T> &list)
{
  for (int64_t i = 0; i < (int64_t)list.size() && i < row * col; ++i)
    m_data[i] = (T)(*(list.begin() + i));
}

template <typename T, int64_t col, int64_t row> template <typename T2> const atMatrix<T, col, row>& atMatrix<T, col, row>::operator=(const atMatrix<T2, col, row> &rhs)
{
  for (int64_t i = 0; i < row * col; ++i)
    m_data[i] = (T)rhs.m_data[i];
  return *this; 
}

template <typename T, int64_t col, int64_t row> template <typename T2> atMatrix<T, col, row>::atMatrix(const atMatrix<T2, col, row> &copy)
{
  for (int64_t i = 0; i < row * col; ++i)
    m_data[i] = (T)copy.m_data[i];
}

template <typename T, int64_t col, int64_t row> template <int64_t col2, int64_t row2> atMatrix<T, row, col2> atMatrix<T, col, row>::Mult(const atMatrix<T, col2, row2> &rhs) const { return Mult<T, col2, row2>(rhs); }
template <typename T, int64_t col, int64_t row> template <int64_t col2, int64_t row2> atMatrix<T, row, col2> atMatrix<T, col, row>::operator*(const atMatrix<T, col2, row2>& rhs) const { return Mult<T, col, row2>(rhs); }
template <typename T, int64_t col, int64_t row> template <typename T2, int64_t col2, int64_t row2> atMatrix<T, row, col2> atMatrix<T, col, row>::operator*(const atMatrix<T2, col2, row2>& rhs) const { return Mult<T2, col2, row2>(rhs); }
         
template <typename T, int64_t col, int64_t row> template <typename T2> atMatrix<T, col, row> atMatrix<T, col, row>::operator/(const T2 &rhs) const { return Mult<T2>((T)1 / rhs); }
template <typename T, int64_t col, int64_t row> template <typename T2> atMatrix<T, col, row> atMatrix<T, col, row>::operator+(const atMatrix<T2, col, row>& rhs) const { return Add(rhs); }
template <typename T, int64_t col, int64_t row> template <typename T2> atMatrix<T, col, row> atMatrix<T, col, row>::operator-(const atMatrix<T2, col, row>& rhs) const { return Subtract(rhs); }

template <typename T, int64_t col, int64_t row> atMatrix<T, col, row> atMatrix<T, col, row>::Inverse() const { return ((row == 2 && col == 2) ? atMatrix<T, col, row>({ m_data[3], -m_data[1], -m_data[2], m_data[0] }) : Cofactors().Transpose()).Mult((T)1 / Determinate()); }
template <typename T, int64_t col, int64_t row> const atMatrix<T, col, row>& atMatrix<T, col, row>::operator=(const atMatrix<T, col, row> &rhs) { memcpy(m_data, rhs.m_data, (int64_t)sizeof(T) * row * col); return *this; }

template <typename T, int64_t col, int64_t row> atMatrix<T, col, row>::atMatrix(atMatrix<T, col, row> &&move) { memcpy(m_data, move.m_data, (int64_t)sizeof(T) * row * col); move = atMatrix(); }
template <typename T, int64_t col, int64_t row> atMatrix<T, col, row>::atMatrix(const atMatrix<T, col, row> &copy) { memcpy(m_data, copy.m_data, (int64_t)sizeof(T) * row * col); }
         
template <typename T, int64_t col, int64_t row> bool atMatrix<T, col, row>::operator==(const atMatrix<T, col, row> &rhs) const { return memcmp(m_data, rhs.m_data, row * col) == 0; }
template <typename T, int64_t col, int64_t row> bool atMatrix<T, col, row>::operator!=(const atMatrix<T, col, row>& rhs) const { return !(*this == rhs); }
template <typename T, int64_t col, int64_t row> const T & atMatrix<T, col, row>::operator[](const int64_t index) const { return m_data[index]; }
template <typename T, int64_t col, int64_t row> atMatrix<T, col, row> atMatrix<T, col, row>::Subtract(const T &rhs) const { return Add(-rhs); }
template <typename T, int64_t col, int64_t row> T& atMatrix<T, col, row>::operator[](const int64_t index) { return m_data[index]; }
template <typename T, int64_t col, int64_t row> atMatrix<T, col, row>::atMatrix() { memset(m_data, 0, sizeof(T) * row * col); }
