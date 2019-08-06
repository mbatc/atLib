#ifndef atRigidBody_h__
#define atRigidBody_h__

#include "atMesh.h"
#include "atCollision.h"
#include "atPhysicsObject.h"

class atRigidBody : public atPhysicsObject
{
  enum atRigidBodyType
  {
    atRBT_Cube,
    atRBT_Sphere,
    atRBT_Capsule,
    atRBT_Cylinder,
    atRBT_Mesh,
    atRBT_None
  };

public:
  atRigidBody(const atMesh &bounds);         // Construct mesh
  atRigidBody(const atVec3D &dims);          // Construct rectangular prism
  atRigidBody(const double &radius);         // Construct sphere
  atRigidBody(const double &height, const double &radius, const bool &isCapsule = false); // Construct cylinder or capsule

  void SetCube(const atVec3D &dims);
  void SetSphere(const double &radius);
  void SetCylinder(const double &height, const double &radius);
  void SetCapsule(const double &height, const double &radius);
  void SetMesh(const atMesh &mesh);

  bool Collide(const atRigidBody &body, atCollisionD *pThis = nullptr, atCollisionD *pWith = nullptr) const;

protected:
  template<typename T> bool CollidePrimitive(const atRigidBody &body, const T &primitive, atCollisionD *pThis = nullptr, atCollisionD *pWith = nullptr) const;

  atOBB<double> OBB() const;
  atSphere<double> Sphere() const;

  void Clear();

  atVec3D m_size = 0;  
  double m_height = 0;
  double m_radius = 0;
  atMesh *m_pMesh = 0;

  atRigidBodyType m_type = atRBT_None;
};

#include "atRigidBody.inl"

#endif // atRigidBody_h__
