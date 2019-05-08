#ifndef atPhysicsObject_h__
#define atPhysicsObject_h__

#include "atPhysicsMath.h"
#include "atTransformable.h"
#include "atVector.h"

class atPhysicsObject : public atTransformable<double>
{
public:
  atPhysicsObject(const double &mass = 1, const double &rotationalInertia = 1, const atVec3D &pivot = 0);

  // Applies all forces added to the object
  void Apply(const double time);

  // Add a force to act on the object.
  // Params:
  //   force     - Magnitude of the force to apply
  //
  //   direction - Direction that the force acts in
  //
  //   time      - Amount of time the force is applied for
  //               if time < 0 then the the force is applied
  //               for the time passed to the Apply() function
  //
  //   position  - Offset from the center of mass to apply the force
  void AddForce(const double force, const atVec3D &direction, const double &time = -1.0, const atVec3D &position = atVec3D::zero());

  atVec3D Momentum() const;
  atQuatD AngularMomentum() const;

  atVec3D m_velocity;
  atQuatD m_angularVelocity;

  double m_rotationalInertia; // Rotational equivalent of mass
  double m_mass;              // Mass of the object

protected:
  struct AppliedForce
  {
    atForceD force;   // The force to be applied
    atVec3D toCoM;    // Vector from applied position to the center of mass
    double time;      // Time for the force to be applied
  };
  
  atVector<AppliedForce> m_forces;
};

#endif // atPhysicsObject_h__
