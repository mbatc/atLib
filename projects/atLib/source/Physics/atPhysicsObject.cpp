#include "atPhysicsObject.h"
#include "atLimits.h"

atPhysicsObject::atPhysicsObject(const double &mass, const double &rotationalInertia, const atVec3D &pivot)
  : mass(mass)
  , velocity(0)
  , rotationalInertia(rotationalInertia)
  , elasticity(0.5)
  , frictionalCoeff(0.5)
{
  SetPivot(pivot);
}

void atPhysicsObject::Apply(const double time)
{
  m_prevVelocity = velocity;
  m_prevAngularVelocity = angularVelocity;

  atVec3D velDir = velocity.Mag() > atLimitsSmallest<double>() ? velocity.Normalize() : atVec3D::zero();
  bool isMoving = velDir.Mag() > atLimitsSmallest<double>();
  for (NormalForce &normal : m_normalForce)
    AddCollisionForce(normal, velDir, isMoving);

  for (AppliedForce &applied : m_appliedForce)
    ApplyForce(&applied, time, true);

  // for (AppliedForce &resitive : m_resistingForce)
  //   ApplyForce(&resitive, time, false);


  // double velMag = velocity.Mag();
  // if (velMag > atLimitsSmallest<double>())
  // {
  //   atForceD velFrc(velocity.Mag(), velocity.Normalize());
  //   velFrc = AdjustForNormalForce(velFrc);
  //   velocity = velFrc.direction * velFrc.magnitude;
  // }

  // Remove expired forces
  for (int64_t i = m_appliedForce.size() - 1; i >= 0; --i)
    if (m_appliedForce[i].time <= 0)
      m_appliedForce.swap_pop_back(i);

  // Clear resiting forces
  m_resistingForce.clear();

  // Clear normal forces
  m_normalForce.clear();

  m_translation += velocity * time;
  atQuatD rot = atQuatD(m_rotation).Normalize(); // Apply inertia
  atQuatD inertia = atQuatD().Normalize();
  m_rotation = m_rotation.Slerp(m_rotation * angularVelocity, time);

  // Calculate Accelerations
  m_angularAcceleration = angularVelocity * m_prevAngularVelocity.Inverse();
  m_acceleration = velocity - m_prevVelocity;
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
  m_appliedForce.push_back(applied);
}

void atPhysicsObject::AddNormalForce(const double &force, const atVec3D &direction, const atVec3D &position, atPhysicsObject *pSrcObject)
{
  NormalForce normal;
  if (force <= atLimitsSmallest<double>() || direction.Mag() <= atLimitsSmallest<double>())
    return;

  normal.force.direction = direction.Normalize();
  normal.force.magnitude = force;
  normal.toCoM = -position;
  normal.pSrcObj = pSrcObject;
  m_normalForce.push_back(normal);
}

void atPhysicsObject::AddAcceleration(const atVec3D &acceleration, const double &time)
{
  AddAcceleration(acceleration.Mag(), acceleration, time);
}

void atPhysicsObject::AddAcceleration(const double &magnitude, const atVec3D &direction, const double &time)
{
  AddForce(mass * magnitude, direction.Normalize(), time);
}

void atPhysicsObject::Translate(const atVector3<double> &translation)
{
  velocity = translation;
  atTransformable::Translate(translation);
}

void atPhysicsObject::Rotate(const atVector3<double> &rotation)
{
  m_rotation = atQuatD(rotation.x, rotation.y, rotation.z);
  atTransformable::Rotate(rotation);
}

atVec3D atPhysicsObject::Momentum() const
{
  return velocity * mass;
}

atQuatD atPhysicsObject::AngularMomentum() const
{
  return angularVelocity * rotationalInertia;
}

void atPhysicsObject::AddResistiveForce(const AppliedForce &force)
{
  for (NormalForce &normForce : m_normalForce)
  {
    if (force.force.direction.Dot(normForce.force.direction) > atLimitsSmallest<double>())
      continue;

    if (normForce.pSrcObj)
    {
      // normForce.pSrcObj->AddResistiveForce(normForce.);
    }
    else
    {
      AppliedForce f;
      f.force.direction = normForce.force.direction;
      f.force.magnitude = normForce.force.direction.Project(force.force.direction).Mag() * force.force.magnitude;
      f.time = -1;
      f.toCoM = normForce.toCoM;
      m_resistingForce.push_back(f);
    }
  }
}

void atPhysicsObject::AddCollisionForce(const NormalForce &force, const atVec3D &velDir, const bool &isMoving)
{
  if (isMoving && force.force.direction.Dot(velDir) > 0)
    return;

  atVec3D v1 = velocity.Project(force.force.direction);
  atVec3D v2 = force.pSrcObj ? force.pSrcObj->velocity.Project(force.force.direction) : atVec3D::zero();
  AddAcceleration(atCollisionVelocity(v1, mass, v2, force.pSrcObj ? force.pSrcObj->mass : (mass * 10000), elasticity) / 0.001, 0.001);
}

void atPhysicsObject::ApplyForce(AppliedForce *pForce, const double &time, const bool &tryResist)
{
  double appliedTime = pForce->time > 0 ? atMin(time, pForce->time) : time;
  pForce->time -= time;

  double dist = pForce->toCoM.Mag();
  double forceMag = pForce->force.magnitude;

  if (dist > atLimitsSmallest<double>())
  { // If the distance is > 0, then a torque may be applied
    double appliedAngle = pForce->toCoM.Angle(pForce->force.direction);
    double torque = dist * pForce->force.magnitude * atSin(appliedAngle);

    atVec3D rotAxis = pForce->toCoM.Cross(pForce->force.direction).Normalize();
    angularVelocity *= atQuatD(rotAxis, torque * appliedTime / rotationalInertia);

    forceMag *= atCos(appliedAngle);
  }

  // if (tryResist)
  //   AddResistiveForce(*pForce);

  velocity += pForce->force.direction * forceMag * appliedTime / mass;
}
