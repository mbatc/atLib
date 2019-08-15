
template<typename T> inline bool atRigidBody::CollidePrimitive(const atRigidBody &body, const T &primitive, atCollisionD *pThis, atCollisionD *pWith) const
{
  switch (body.m_type)
  {
  case atRBT_Cube: return atCollision(body.AsOBB(), primitive, pThis, pWith);
  case atRBT_Sphere: return atCollision(body.AsSphere(), primitive, pThis, pWith);
  }
  return false;
}
