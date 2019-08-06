
template<typename T> inline atRigidBody::CollidePrimitive(const atRigidBody &body, const T &primitive, atCollisionD *pThis, atCollisionD *pWith) const
{
  switch (body.m_type)
  {
  case atRBT_Cube: return atCollision(body.OBB(), primitive, pWith, pThis);
  case atRBT_Sphere: return atCollision(body.Sphere(), primitive, pWith, pThis);
  }
  return false;
}
