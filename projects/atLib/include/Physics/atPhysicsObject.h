#ifndef atPhysicsObject_h__
#define atPhysicsObject_h__

#include "atPhysicsMath.h"
#include "atTransformable.h"
#include "atVector.h"

class atPhysicsObject : public atTransformable<double>
{
  struct AppliedForce
  {
    atForceD force;   // The force to be applied
    atVec3D toCoM;    // Vector from applied position to the center of mass
    double time;      // Time for the force to be applied
  };

  struct NormalForce
  {
    atVec3D toCoM;
    atForceD force;
    atPhysicsObject *pSrcObj;
  };

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
  void AddNormalForce(const double &force, const atVec3D &direction, const atVec3D &position = atVec3D::zero(), atPhysicsObject *pSrcObject = nullptr);
  void AddAcceleration(const atVec3D &acceleration, const double &time = -1.0);
  void AddAcceleration(const double &magnitude, const atVec3D &direction, const double &time = -1.0);

  void Translate(const atVector3<double> &translation);
  void Rotate(const atVector3<double> &rotation);

  atVec3D Momentum() const;
  atQuatD AngularMomentum() const;

  atVec3D velocity;
  atQuatD angularVelocity;

  double elasticity;        // Amount of force that is not absorbed during a collision
  double frictionalCoeff;   // Coefficient of friction. How much colliding objects resist motion.
  double rotationalInertia; // Rotational equivalent of mass
  double mass;              // Mass of the object

protected:
  // Try find a force that can resist this force
  void AddResistiveForce(const AppliedForce &force);
  void AddCollisionForce(const NormalForce &force, const atVec3D &velDir, const bool &isMoving);
  void ApplyForce(AppliedForce *pForce, const double &time, const bool &tryResist);

  atVec3D m_acceleration;
  atQuatD m_angularAcceleration;

  atVec3D m_prevVelocity;
  atQuatD m_prevAngularVelocity;
    
  atVector<AppliedForce> m_appliedForce;
  atVector<AppliedForce> m_resistingForce;

  atVector<NormalForce> m_normalForce;
};

#endif // atPhysicsObject_h__
