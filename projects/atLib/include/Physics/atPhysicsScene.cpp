#include "atPhysicsScene.h"

atPhysicsScene::atPhysicsScene(const double timeStep)
{
  SetTimeStep(timeStep);
}

void atPhysicsScene::SetTimeStep(const double &timestep)
{
  m_timestep = timestep;
}

const double& atPhysicsScene::TimeStep() {  return m_timestep; }

void atPhysicsScene::Update()
{
}

void atPhysicsScene::AddForce()
{
}

atRigidBody& atPhysicsScene::GetBody(const int64_t &id) { return m_bodies[id]; }

const atRigidBody& atPhysicsScene::GetBody(const int64_t &id) const { return m_bodies[id]; }

int64_t atPhysicsScene::AddBody(const atRigidBody &body, const atVec3D &pos, const atVec3D &rot)
{
  m_bodies.push_back(body);
  m_bodies.back().SetTranslation(pos);
  m_bodies.back().SetRotation(rot);
  return m_bodies.size() - 1;
}

const atVector<atRigidBody>& atPhysicsScene::Bodies() const { return m_bodies; }

void atPhysicsScene::SetBody(const int64_t &id, const atRigidBody &body)
{
  m_bodies[id] = body;
}
