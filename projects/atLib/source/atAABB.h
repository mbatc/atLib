#ifndef atAABB_h__
#define atAABB_h__

#include "atVector.h"
#include "atVector3.h"

template <typename T> class atAABB
{
public:
  using Vec3 = atVector3<T>;

  atAABB(const Vec3 &min = std::numeric_limits<T>::max, const Vec3 &max = -std::numeric_limits<T>::max);
  atAABB(atVector<Vec3> &points);
  atAABB(const atAABB &copy);
  atAABB(atAABB &&o);

  void GrowToContain(const atVector<Vec3> &points);
  void GrowToContain(const Vec3 &point);
  void GrowToContain(const atAABB<T> &box);
  void ShrinkToFit(const atAABB<T> &box);

  bool Contains(const Vec3 &point) const;

  Vec3 Center() const;
  Vec3 Dimensions() const;

  template <typename Type> void GrowToContain(const Type &type);
  template <typename Type> void atBounds(const Type &type);

  Vec3 m_min;
  Vec3 m_max;
};

#include "atAABB.inl"
#endif // atAABB_h__
