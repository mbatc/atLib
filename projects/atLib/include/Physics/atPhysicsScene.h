#ifndef atPhysicsScene_h__
#define atPhysicsScene_h__

#include "atRigidBody.h"

class atPhysicsScene
{
public:
  atPhysicsScene(const double timeStep = 0.016);

  void SetTimeStep(const double &timestep);
  const double& TimeStep();

  void Update();
  void AddForce();

  atRigidBody& GetBody(const int64_t &id);
  const atRigidBody& GetBody(const int64_t &id) const;
  
  int64_t AddBody(const atRigidBody &body, const atVec3D &pos = { 0, 0 ,0 }, const atVec3D &rot = { 0, 0, 0 });
  void SetBody(const int64_t &id, const atRigidBody &body);

  const atVector<atRigidBody>& Bodies() const;

protected:
  double m_timestep;
  atVector<atRigidBody> m_bodies;
};

#endif // atPhysicsScene_h__
