#include "atPhysicsObject.h"
#include "atLimits.h"

atPhysicsObject::atPhysicsObject(const double &mass, const atVec3D &centerOfMass)
  : m_mass(mass)
  , m_velocity(0)
  , m_centerOfMass(centerOfMass)
{}

void atPhysicsObject::Apply(const double time)
{
  for (AppliedForce &applied : m_forces)
  {
    double appliedTime = applied.time > 0 ? atMin(time, applied.time) : time;
    m_velocity = atVelocity(applied.force, m_mass, appliedTime, m_velocity);
    applied.time -= time;
  }

  for (AppliedMoment &applied : m_moments)
  {
    atVec3D toArm = applied.moment.arm - m_centerOfMass;
    double forceAngle = toArm.Angle(applied.moment.force.direction);
    double torque = (applied.moment.arm - m_centerOfMass).Mag() * applied.moment.force.magnitude * atSin(forceAngle);
    atVec3D rotAxis = toArm.Cross(applied.moment.force.direction).Normalize();

    // Increase moment of inertia around moment 
    m_momentOfIntertia = atMatrixRotation(rotAxis, torque / m_mass) * m_momentOfIntertia;
  }

  // Remove expired forces
  for (int64_t i = m_forces.size() - 1; i >= 0; --i)
    if (m_forces[i].time <= 0)
      m_forces.swap_pop_back(i);

  for (int64_t i = m_moments.size() - 1; i >= 0; --i)
    if (m_moments[i].time <= 0)
      m_moments.swap_pop_back(i);

  m_translation += m_velocity * time;
  m_rotation = m_momentOfIntertia * m_rotation;
}

void atPhysicsObject::AddForce(const double force, const atVec3D &direction, const double &time)
{
  AppliedForce applied;
  if (force <= atLimitsSmallest<double>() || abs(time) <= atLimitsSmallest<double>()|| direction.Mag() <= atLimitsSmallest<double>())
    return;

  applied.force.direction = direction.Normalize();
  applied.force.magnitude = force;
  applied.time = time;
  m_forces.push_back(applied);
}

void atPhysicsObject::AddMoment(const double force, const atVec3D &direction, const atVec3D &momentArm, const double &time)
{
  AppliedMoment applied;
  applied.moment.force.direction = direction.Normalize();
  applied.moment.force.magnitude = force;
  applied.moment.arm = momentArm;
  applied.time = time;
  m_moments.push_back(applied);
}

atVec3D atPhysicsObject::Momentum() const
{
  return m_velocity * m_mass;
}
