#include "atRigidBody.h"

atRigidBody::atRigidBody(const atMesh &bounds, const double &mass) { SetMesh(bounds); }

atRigidBody::atRigidBody(const atVec3D &dims, const double &mass)
  : m_size(dims)
  , m_type(atRBT_Cube)
  , atPhysicsObject(mass)
{}

atRigidBody::atRigidBody(const double &radius, const double &mass)
  : m_radius(radius)
  , m_type(atRBT_Sphere)
  , atPhysicsObject(mass)
{}

atRigidBody::atRigidBody(const double &height, const double &radius, const bool &isCapsule, const double &mass)
  : m_height(height)
  , m_radius(radius)
  , m_type(isCapsule ? atRBT_Capsule : atRBT_Cylinder)
  , atPhysicsObject(mass)
{}

atRigidBodyType atRigidBody::Type() const { return m_type; }

void atRigidBody::SetCube(const atVec3D &dims)
{
  Clear();
  m_size = dims;
  m_type = atRBT_Cube;
}

void atRigidBody::SetSphere(const double &radius)
{
  Clear();
  m_radius = radius;
  m_type = atRBT_Sphere;
}

void atRigidBody::SetCylinder(const double &height, const double &radius)
{
  Clear();
  m_height = height;
  m_radius = radius;
  m_type = atRBT_Cylinder;
}

void atRigidBody::SetCapsule(const double &height, const double &radius)
{
  Clear();
  m_height = height;
  m_radius = radius;
  m_type = atRBT_Capsule;
}

void atRigidBody::SetMesh(const atMesh &mesh)
{
  Clear();
  m_pMesh = atNew<atMesh>(mesh);
  m_type = atRBT_Mesh;
}

void atRigidBody::ApplyCollision(const atRigidBody &body, const atCollisionD &colData)
{

}

bool atRigidBody::TestCollision(const atRigidBody &body, atCollisionD *pThis, atCollisionD *pWith) const
{
  switch (body.m_type)
  {
  case atRBT_Cube: return CollidePrimitive(*this, body.AsOBB(), pThis, pWith);
  case atRBT_Sphere: return CollidePrimitive(*this, body.AsSphere(), pThis, pWith);
  }
  return true;
}

atOBB<double> atRigidBody::AsOBB() const { return atOBB<double>(Translation() - m_size / 2, Translation() + m_size / 2, Orientation()); }

atSphere<double> atRigidBody::AsSphere() const { return atSphere<double>(m_radius); }

void atRigidBody::Clear()
{
  if (m_pMesh)
    atDelete(m_pMesh);
  m_pMesh = nullptr;
  m_type = atRBT_None;
}
