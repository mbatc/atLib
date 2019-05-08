#ifndef atRigidBodyCube_h__
#define atRigidBodyCube_h__

#include "atPhysicsObject.h"
#include "atAABB.h"

struct atCollision
{
  double crossSectionalArea;
  atVec3D avgPosition;
  atVec3D direction;
};

class atRigidBodyCube : public atPhysicsObject
{
public:
  atRigidBodyCube(const atVec3D &dimensions, const double &mass = 1);

  bool TryCollide(atCollision *pCollision);

protected:
  atAABB<double> m_bounds;
};

#endif // atRigidBodyCube_h__
