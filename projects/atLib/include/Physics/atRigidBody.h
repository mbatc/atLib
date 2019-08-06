#ifndef atRigidBody_h__
#define atRigidBody_h__

#include "atMesh.h"
#include "atCollision.h"
#include "atPhysicsObject.h"

enum atRigidBodyType
{
  atRBT_Cube,
  atRBT_Sphere,
  atRBT_Capsule,
  atRBT_Cylinder,
  atRBT_Mesh,
  atRBT_None
};

class atRigidBody : public atPhysicsObject
{

public:
  atRigidBody(const atMesh &bounds, const double &mass = 1);         // Construct mesh
  atRigidBody(const atVec3D &dims, const double &mass = 1);          // Construct rectangular prism
  atRigidBody(const double &radius, const double &mass = 1);         // Construct sphere
  atRigidBody(const double &height, const double &radius, const bool &isCapsule = false, const double &mass = 1); // Construct cylinder or capsule

  atRigidBodyType Type() const;

  void SetCube(const atVec3D &dims);
  void SetSphere(const double &radius);
  void SetCylinder(const double &height, const double &radius);
  void SetCapsule(const double &height, const double &radius);
  void SetMesh(const atMesh &mesh);

  void ApplyCollision(const atRigidBody &body, const atCollisionD &colData);

  bool TestCollision(const atRigidBody &body, atCollisionD *pThis = nullptr, atCollisionD *pWith = nullptr) const;

  atOBB<double> AsOBB() const;
  atSphere<double> AsSphere() const;

protected:
  template<typename T> bool CollidePrimitive(const atRigidBody &body, const T &primitive, atCollisionD *pThis = nullptr, atCollisionD *pWith = nullptr) const;

  void Clear();

  atVec3D m_size = 0;  
  double m_height = 0;
  double m_radius = 0;
  atMesh *m_pMesh = 0;

  atRigidBodyType m_type = atRBT_None;
};

#include "atRigidBody.inl"

#endif // atRigidBody_h__
