#ifndef atRect_h__
#define atRect_h__

#include "atVector4.h"
#include "atIntersects.h"

template<typename T> class atRect
{
public:
  atRect(const atVector4<T> &rect);
  atRect(const atVector2<T> &min, const atVector2<T> &max);

  T Area() const;
  atVector2<T> Dimensions() const;

  bool Contains(const atRect &rect);
  bool Contains(const atVector2<T> &point);
  void GrowToContain(const atRect &point);
  void GrowToContain(const atVector2<T> &point);

  atVector2<T> m_min;
  atVector2<T> m_max;
};

typedef atRect<int32_t> atRectI;
typedef atRect<int64_t> atRectI64;
typedef atRect<float> atRectF;
typedef atRect<double> atRectD;

#include "atRect.inl"
#endif // atRect_h__
