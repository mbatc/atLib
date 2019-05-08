#include "atPhysicsObject.h"
#include "atLimits.h"

atPhysicsObject::atPhysicsObject(const double &mass, const double &rotationalInertia, const atVec3D &pivot)
  : m_mass(mass)
  , m_velocity(0)
  , m_rotationalInertia(rotationalInertia)
{
  SetPivot(pivot);
}

void atPhysicsObject::Apply(const double time)
{
  for (AppliedForce &applied : m_forces)
  {
    double appliedTime = applied.time > 0 ? atMin(time, applied.time) : time;
    applied.time -= time;

    double dist = applied.toCoM.Mag();
    double force = applied.force.magnitude;

    if (dist > atLimitsSmallest<double>())
    { // If the distance is > 0, then a torque may be applied
      double appliedAngle = applied.toCoM.Angle(applied.force.direction);
      double torque = dist * applied.force.magnitude * atSin(appliedAngle);
      
      atVec3D rotAxis = applied.toCoM.Cross(applied.force.direction).Normalize();
      m_angularVelocity *= atQuatD(rotAxis, torque * appliedTime / m_rotationalInertia);

      force *= atCos(appliedAngle);
    }

    m_velocity += applied.force.direction * force * appliedTime / m_mass;
  }

  // Remove expired forces
  for (int64_t i = m_forces.size() - 1; i >= 0; --i)
    if (m_forces[i].time <= 0)
      m_forces.swap_pop_back(i);

  m_translation += m_velocity * time;
  atQuatD rot = atQuatD(m_rotation).Normalize(); // Apply inertia
  atQuatD inertia = atQuatD().Normalize();
  m_rotation = m_rotation.Slerp(m_rotation * m_angularVelocity, time);
}

void atPhysicsObject::AddForce(const double force, const atVec3D &direction, const double &time, const atVec3D &position)
{
  AppliedForce applied;
  if (force <= atLimitsSmallest<double>() || abs(time) <= atLimitsSmallest<double>() || direction.Mag() <= atLimitsSmallest<double>())
    return;

  applied.force.direction = direction.Normalize();
  applied.force.magnitude = force;
  applied.toCoM = -position;
  applied.time = time;
  m_forces.push_back(applied);
}

atVec3D atPhysicsObject::Momentum() const
{
  return m_velocity * m_mass;
}

atQuatD atPhysicsObject::AngularMomentum() const
{
  return m_angularVelocity * m_rotationalInertia;
}
