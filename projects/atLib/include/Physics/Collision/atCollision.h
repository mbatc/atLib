#ifndef atCollisionData_h__
#define atCollisionData_h__

#include "atMath.h"
#include "atRay.h"
#include "atOBB.h"
#include "atAABB.h"
#include "atSphere.h"

template <typename T> class atCollisionData
{
public:
  atCollisionData();
  atCollisionData(const atVec3D &point, const atVec3D &normal, const atVec3D &midPoint);
  
  bool Collided() const;
  const atVec3D& Point() const;
  const atVec3D& Normal() const;
  const atVec3D& MidPoint() const;

protected:
  bool m_collided;
  atVec3D m_point;
  atVec3D m_normal;
  atVec3D m_midPoint;
};

template<typename T> bool atCollision(const atOBB<T> &obb, const atOBB<T> &obb2, atCollisionData<T> *pOBBCol, atCollisionData<T> *pOBB2Col);
template<typename T> bool atCollision(const atAABB<T> &aabb, const atOBB<T> &obb, atCollisionData<T> *pAABBCol, atCollisionData<T> *pOBBCol);
template<typename T> bool atCollision(const atAABB<T> &aabb, const atAABB<T> &aabb2, atCollisionData<T> *pAABBCol, atCollisionData<T> *pAABB2Col);
template<typename T> bool atCollision(const atSphere<T> &sphere, const atOBB<T> &obb, atCollisionData<T> *pSphereCol, atCollisionData<T> *pOBBCol);
template<typename T> bool atCollision(const atSphere<T> &sphere, const atAABB<T> &aabb, atCollisionData<T> *pSphereCol, atCollisionData<T> *pAABBCol);
template<typename T> bool atCollision(const atSphere<T> &sphere, const atSphere<T> &sphere2, atCollisionData<T> *pSphereCol, atCollisionData<T> *pSphere2Col);

// Flip inputs for completeness sake
template<typename T> bool atCollision(const atOBB<T> &obb, const atAABB<T> &aabb, atCollisionData<T> *pOBBCol, atCollisionData<T> *pAABBCol);
template<typename T> bool atCollision(const atOBB<T> &obb, const atSphere<T> &sphere, atCollisionData<T> *pOBBCol, atCollisionData<T> *pSphereCol);
template<typename T> bool atCollision(const atAABB<T> &aabb, const atSphere<T> &sphere, atCollisionData<T> *pAABBCol, atCollisionData<T> *pSphereCol);

typedef atCollisionData<int32_t> atCollisionI;
typedef atCollisionData<int64_t> atCollisionI64;
typedef atCollisionData<float> atCollisionF;
typedef atCollisionData<double> atCollisionD;

#include "atCollision.inl"
#endif // atCollisionData_h__
