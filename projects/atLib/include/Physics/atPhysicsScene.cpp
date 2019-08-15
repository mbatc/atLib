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
  for (int64_t i = 0; i < m_bodies.size(); ++i)
    for (int64_t j = i + 1; j < m_bodies.size(); ++j)
    {
      atRigidBody &bodyA = m_bodies[i];
      atRigidBody &bodyB = m_bodies[j];
      atCollisionD collisionA;
      atCollisionD collisionB;
      if (!bodyA.TestCollision(bodyB, &collisionA, &collisionB))
        continue;
      bodyA.ApplyCollision(&bodyB, collisionA, collisionB);
      bodyB.ApplyCollision(&bodyA, collisionB, collisionA);
    }

  for (atRigidBody &body : m_bodies)
    body.Apply(m_timestep);
}

void atPhysicsScene::AddForce(const double &magnitude, const atVec3D &direction, const double &time)
{
  for (atRigidBody &body : m_bodies)
    if (!body.IsStatic())
      body.AddForce(magnitude, direction, time);
}

void atPhysicsScene::AddAcceleration(const double &magnitude, const atVec3D &direction, const double &time)
{
  for (atRigidBody &body : m_bodies)
    if (!body.IsStatic())
      body.AddAcceleration(magnitude, direction, time);
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
