
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

template <typename T> atAABB<T>::atAABB(const atAABB &copy)
  : m_min(copy.m_min)
  , m_max(copy.m_max)
{}

template <typename T> atAABB<T>::atAABB(atAABB &&o)
  : m_min(std::move(copy.m_min))
  , m_max(std::move(copy.m_max))
{}

template <typename T> void atAABB<T>::GrowToContain(const Vec3 &point)
{
  for (int64_t dim = 0; dim < 3; ++dim)
  {
    m_min[dim] = atMin(point[dim], m_min[dim]);
    m_max[dim] = atMax(point[dim], m_max[dim]);
  }
}

template <typename T> void atAABB<T>::GrowToContain(const atAABB<T> &box)
{
  for (int64_t dim = 0; dim < 3; ++dim)
  {
    m_min[dim] = atMin(box.m_min[dim], m_min[dim]);
    m_max[dim] = atMax(box.m_max[dim], m_max[dim]);
  }
}

template <typename T> void atAABB<T>::ShrinkToFit(const atAABB<T> &box)
{
  for (int64_t dim = 0; dim < 3; ++dim)
  {
    m_min[dim] = atMax(box.m_min[dim], m_min[dim]);
    m_max[dim] = atMin(box.m_max[dim], m_max[dim]);
  }
}

template <typename T> template<typename Type> void atAABB<T>::atBounds(const Type &type) { atAssert(false, "Bounds are not defined for this type"); return atAABB<T>(); }
template <typename T> template<typename Type> void atAABB<T>::GrowToContain(const Type &type) { GrowToContain(atBounds(type)); }
template <typename T> atAABB<T>::atAABB(const Vec3 &min, const Vec3 &max) : m_min(min), m_max(max) {}
template <typename T> atAABB<T>::atAABB(atVector<Vec3> &points) { GrowToContain(points); }
template <typename T> void atAABB<T>::GrowToContain(const atVector<Vec3> &points) { for (const Vec3 p : points) GrowToContain(points); }
