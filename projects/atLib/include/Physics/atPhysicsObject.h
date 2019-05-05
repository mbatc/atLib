#ifndef atPhysicsObject_h__
#define atPhysicsObject_h__

#include "atPhysicsMath.h"
#include "atTransformable.h"
#include "atVector.h"

class atPhysicsObject : public atTransformable<double>
{
public:
  atPhysicsObject(const double &mass = 1, const atVec3D &centerOfMass = 0);

  // Applies all forces added to the object
  void Apply(const double time);

  // Add a force to be calculated when Apply is called
  void AddForce(const double force, const atVec3D &direction, const double &time = -1.0);
  void AddMoment(const double force, const atVec3D &direction, const atVec3D &momentArm, const double &time = -1.0);

  atVec3D Momentum() const;

  atVec3D m_centerOfMass;
  atVec3D m_velocity;
  atMat4D m_momentOfIntertia;

  double m_mass;

protected:
  struct AppliedForce
  {
    atForceD force;
    double time;
  };

  struct AppliedMoment
  {
    atMomentD moment;
    double time;
  };
  
  atVector<AppliedForce> m_forces;
  atVector<AppliedMoment> m_moments;
};

#endif // atPhysicsObject_h__
