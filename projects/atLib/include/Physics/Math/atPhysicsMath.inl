
template<typename T> inline atForce<T>::atForce(const T &magnitude, const atVector3<T> &direction)
  : magnitude(magnitude)
  , direction(direction)
{}

template<typename T> inline atForce<T>::atForce(const atVector3<T> &acceleration, const T &mass)
  : direction(acceleration)
{
  magnitude = direction.Mag();
  if(abs(magnitude) > atLimitsSmallest<T>())
    direction /= magnitude;
  magnitude *= mass;
}

template<typename T> inline atForce<T>::atForce(const atVector3<T> &initialVelocity, const atVector3<T> &finalVelocity, const T &time, const T &mass)
  : atForce(atAcceleration(initialVelocity, finalVelocity, time), mass)
{}

template<typename T> inline atForce<T>::atForce(const atVector3<T> &initialVelocity, const T &displacement, const atVector3<T> &finalVelocity, const T &mass)
  : atForce(atAcceleration(initialVelocity, finalVelocity, displacement), mass)
{}

template<typename T> inline atVector3<T> atAcceleration(const atForce<T> &force, const T &mass)
{
  return force.direction * force.magnitude / mass;
}

template<typename T> inline atVector3<T> atAcceleration(const atVector3<T> &initialVelocity, const T &displacement, const atVector3<T> &finalVelocity)
{
  return (atSquare(finalVelocity) - atSquare(initialVelocity)) / (2 * displacement);
}

template<typename T> inline atVector3<T> atAcceleration(const atVector3<T> &initialVelocity, const atVector3<T> &finalVelocity, const T &time)
{
  return (finalVelocity - initialVelocity) / time;
}

template<typename T> inline atVector3<T> atVelocityDelta(const atForce<T> &force, const T &mass, const T &time)
{
  return atAcceleration(force, mass) * time;
}

template<typename T> inline atVector3<T> atVelocity(const atForce<T> &force, const T &mass, const T &time, const atVector3<T> &initialVelocity)
{
  return initialVelocity + atVelocityDelta(force, mass, time);
}

template<typename T> inline atVector3<T> atDisplacementA(const atVector3<T> &initialVelocity, const atVector3<T> &acceleration, const T &time)
{
  return initialVelocity * time + acceleration * time * time;
}

template<typename T> inline atVector3<T> atDisplacementV(const atVector3<T> &initialVelocity, const atVector3<T> &finalVelocity, const T &time)
{
  return 0.5 * (initialVelocity + finalVelocity) * time;
}

template<typename T> inline atVector3<T> atDisplacementA(const atVector3<T> &initialVelocity, const atVector3<T> &finalVelocity, const atVector3<T> &acceleration)
{
  return (atSquare(finalVelocity) - atSquare(initialVelocity)) / (2 * acceleration);
}

template<typename T> inline atVector3<T> atMomentum(const atVector3<T> &velocity, const T &mass)
{
  return velocity * mass;
}

template<typename T> inline atVector3<T> atCollisionVelocity(const atVector3<T> &v1, const T &m1, const atVector3<T> &v2, const T &m2, const T &elasticity)
{
  return ((m1 - m2) * v1 / (m1 + m2) + (2 * m2) * v2 / (m1 + m2)) * elasticity + (m1 * v1 + m2 * v2) / (m1 + m2) * (T(1) - elasticity);
}
