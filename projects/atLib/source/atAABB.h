#ifndef atAABB_h__
#define atAABB_h__

#include "atVector.h"
#include "atVector3.h"

template <typename T> class atAABB
{
public:
  using Vec3 = atVector3<T>;

  atAABB();
  atAABB(const Vec3 &min, const Vec3 &max);
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

  Vec3 m_min;
  Vec3 m_max;
  
  const atAABB<T> &operator=(atAABB<T> &&rhs);
  const atAABB<T> &operator=(const atAABB<T> &rhs);
  bool operator==(const atAABB<T> &rhs) const;
  bool operator!=(const atAABB<T> &rhs) const;
};

template <typename T, typename Type> atAABB<T> atBounds(const Type &type) { atAssert(false, "Bounds are not defined for this type"); return atAABB<T>(); }

#include "atAABB.inl"
#endif // atAABB_h__
