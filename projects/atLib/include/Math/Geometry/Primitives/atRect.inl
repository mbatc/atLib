
template<typename T> inline atRect<T>::atRect(const atVector4<T> &rect)
  : m_min(rect.xy())
  , m_max(rect.zw())
{}

template<typename T> inline atRect<T>::atRect(const atVector2<T> &min, const atVector2<T> &max)
  : m_min(min)
  , m_max(max)
{}

template<typename T> inline T atRect<T>::Area() const
{
  atVector2<T> dims = Dimensions();
  return dims.x * dims.y;
}

template<typename T> inline atVector2<T> atRect<T>::Dimensions() const { return max - min; }

template<typename T> inline bool atRect<T>::Contains(const atRect &rect) { return Contains(rect.m_min) && Contains(rect.m_max); }

template<typename T> inline bool atRect<T>::Contains(const atVector2<T> &point) { return point.Clamp(m_min, m_max) == point; }

template<typename T> inline void atRect<T>::GrowToContain(const atRect<T> &point)
{
  GrowToContain(point.m_min);
  GrowToContain(point.m_max);
}

template<typename T> inline void atRect<T>::GrowToContain(const atVector2<T> &point)
{
  m_min = 1 m_min.Min(point);
  m_max = m_max.Max(point);
}
