#include "atRect.h"

template<typename T>
template<typename T2>
inline atRect<T>::atRect(const atRect<T2> &o)
  : m_min(o.m_min)
  , m_max(o.m_max)
{}

template<typename T> inline atRect<T>::atRect(const T &left, const T &top, const T &right, const T &bottom)
  : m_min(left, top)
  , m_max(right, bottom)
{}

template<typename T> inline atRect<T>::atRect(const atVector4<T> &rect)
  : m_min(rect.xy())
  , m_max(rect.zw())
{}

template<typename T>
inline atRect<T> atRect<T>::Largest()
{
    return atRect<T>(atLimitsMin<T>(), atLimitsMax<T>());
}

template<typename T>
inline atRect<T> atRect<T>::Smallest()
{
  return atRect<T>(atLimitsMax<T>(), atLimitsMin<T>());
}

template<typename T> inline atRect<T>::atRect(const atVector2<T> &min, const atVector2<T> &max)
  : m_min(min)
  , m_max(max)
{}

template<typename T> inline T atRect<T>::Area() const
{
  atVector2<T> dims = Dimensions();
  return dims.x * dims.y;
}

template<typename T> inline atVector2<T> atRect<T>::Dimensions() const { return m_max - m_min; }
template<typename T> inline atVector4<T> atRect<T>::ToVector4() const { return atVector4<T>(m_min, m_max); }

template<typename T> inline T atRect<T>::Width() const { return m_max.x - m_min.x; }
template<typename T> inline T atRect<T>::Height() const { return m_max.y - m_min.y; }

template<typename T> inline bool atRect<T>::Contains(const atRect &rect) { return Contains(rect.m_min) && Contains(rect.m_max); }
template<typename T> inline bool atRect<T>::Contains(const atVector2<T> &point) { return point.Clamp(m_min, m_max) == point; }

template<typename T> inline void atRect<T>::GrowToContain(const atRect<T> &point)
{
  GrowToContain(point.m_min);
  GrowToContain(point.m_max);
}

template<typename T> inline void atRect<T>::GrowToContain(const atVector2<T> &point)
{
  m_min = m_min.Min(point);
  m_max = m_max.Max(point);
}

template<typename T> inline bool atRect<T>::operator==(const atRect<T> &rhs) const { return m_max == rhs.m_max && m_min == rhs.m_min; }
template<typename T> inline bool atRect<T>::operator!=(const atRect<T> &rhs) const { return !(*this == rhs); }
