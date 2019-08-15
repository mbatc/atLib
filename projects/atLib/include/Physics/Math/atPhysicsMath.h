#ifndef atPhysicsMath_h__
#define atPhysicsMath_h__

#include "atMath.h"

// some helpful equations
//
//  Equations of Motion
//
//  S = displacement
//  Vf = Final Velocity
//  Vi = Initial Velocity
//  A = Acceleration
//  dT = Delta Time
//
//    Vf = Vi + A * dT
//
//    S = 0.5 * (Vi + Vf) * dT
//
//    S = (Vf^2 - Vi^2) / 2A
//
//    A = (Vf^2 - Vi^2) / 2S
//
//    S = Vi*dT + A*dT^2
//
//
//  Gravity
//
//    G = m1 * m2 / distance^2
//  
// 

template <typename T> struct atForce
{
  atForce(const T &magnitude = 0, const atVector3<T> &direction = 0);
  atForce(const atVector3<T> &acceleration, const T &mass = 1);
  atForce(const atVector3<T> &initialVelocity, const atVector3<T> &finalVelocity, const T &time = 1, const T &mass = 1);
  atForce(const atVector3<T> &initialVelocity, const T &displacement, const atVector3<T> &finalVelocity, const T &mass = 1);

  atVector3<T> direction;
  T magnitude;
};

template<typename T> atVector3<T> atAcceleration(const atForce<T> &force, const T &mass);
template<typename T> atVector3<T> atAcceleration(const atVector3<T> &initialVelocity, const T &displacement, const atVector3<T> &finalVelocity);
template<typename T> atVector3<T> atAcceleration(const atVector3<T> &initialVelocity, const atVector3<T> &finalVelocity, const T &time);

template<typename T> atVector3<T> atVelocityDelta(const atForce<T> &force, const T &mass, const T &time);
template<typename T> atVector3<T> atVelocity(const atForce<T> &force, const T &mass, const T &time, const atVector3<T> &initialVelocity);

template<typename T> atVector3<T> atDisplacementA(const atVector3<T> &initialVelocity, const atVector3<T> &acceleration, const T &time);
template<typename T> atVector3<T> atDisplacementV(const atVector3<T> &initialVelocity, const atVector3<T> &finalVelocity, const T &time);
template<typename T> atVector3<T> atDisplacementA(const atVector3<T> &initialVelocity, const atVector3<T> &finalVelocity, const atVector3<T> &acceleration);

template<typename T> atVector3<T> atMomentum(const atVector3<T> &velocity, const T &mass);

template<typename T> atVector3<T> atCollisionVelocity(const atVector3<T> &v1, const T &m1, const atVector3<T> &v2, const T &m2,const T &elasticity = 1);

typedef atForce<double> atForceD;
typedef atForce<float> atForceF;

#include "atPhysicsMath.inl"
#endif // atPhysicsMath_h__
