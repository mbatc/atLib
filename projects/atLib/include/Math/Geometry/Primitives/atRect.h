#ifndef atRect_h__
#define atRect_h__

#include "atVector4.h"
#include "atIntersects.h"

template<typename T> class atRect
{
public:
  template<typename T2 = T> atRect(const atRect<T2> &o);

  atRect(const atVector2<T> &min = atLimitsMax<T>(), const atVector2<T> &max = atLimitsMin<T>());
  atRect(const T &left, const T &top, const T &right, const T &bottom);
  atRect(const atVector4<T> &rect);

  static atRect<T> Largest();
  static atRect<T> Smallest();

  T Area() const;
  atVector2<T> Dimensions() const;
  atVector4<T> ToVector4() const;

  T Width() const;
  T Height() const;

  bool Contains(const atRect &rect);
  bool Contains(const atVector2<T> &point);
  void GrowToContain(const atRect &point);
  void GrowToContain(const atVector2<T> &point);

  bool operator==(const atRect<T> &rhs) const;
  bool operator!=(const atRect<T> &rhs) const;

  atVector2<T> m_min;
  atVector2<T> m_max;
};

typedef atRect<int32_t> atRectI;
typedef atRect<int64_t> atRectI64;
typedef atRect<float> atRectF;
typedef atRect<double> atRectD;

#include "atRect.inl"
#endif // atRect_h__
