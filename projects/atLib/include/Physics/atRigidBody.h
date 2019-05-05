#ifndef atRigidBody_h__
#define atRigidBody_h__

#include "atMesh.h"

class atRigidBody
{
public:
  atRigidBody(const atMesh &bounds);         // Construct mesh
  atRigidBody(const atAABB<double> &bounds); // Construct rectangular prism
  atRigidBody(const double &radius);         // Construct sphere
  atRigidBody(const double &height, const double &radius, const bool &isCapsule = false); // Construct cylinder or capsule

  void SetCube(const atAABB<double> &bounds);
  void SetSphere(const double &radius);
  void SetCylinder(const double &height, const double &radius);
  void SetCapsule(const double &height, const double &radius);
  void SetMesh(const atMesh &mesh);

protected:

};

#endif // atRigidBody_h__
