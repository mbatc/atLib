
template<typename T> inline atCollisionData<T>::atCollisionData()
  : m_collided(false)
{}

template<typename T> inline atCollisionData<T>::atCollisionData(const atVec3D &point, const atVec3D &normal)
  : m_collided(true)
  , m_normal(normal.Normalize())
  , m_point(point)
{}

template<typename T> inline bool atCollisionData<T>::Collided() const { return m_collided; }

template<typename T> inline const atVec3D& atCollisionData<T>::Point() const { return m_point; }

template<typename T> inline const atVec3D& atCollisionData<T>::Normal() const { return m_normal; }

template<typename T> inline bool atCollision(const atOBB<T> &obb, const atOBB<T> &obb2, atCollisionData *pOBBCol, atCollisionData *pOBB2Col)
{
  if (atIntersects(obb, obb2))
  {
    // Calculate the intersection point
    atVector3<T> testPoint = atBounds(obb).OverlappingBox(atBounds(obb2)).Center();

    // Find the closest points that lie on the surface of each bounding box
    atVector3<T> obb1Point = obb.ClosestPointBounds(testPoint);
    atVector3<T> obb2Point = obb2.ClosestPointBounds(testPoint);

    // Get the normals of the colliding planes
    atVector3<T> p1 = (obb1Point - testPoint).Normalize();
    atVector3<T> p2 = (obb2Point - testPoint).Normalize();

    if (1.0 - abs(p1.Dot(p2)) > 0.001)
    {
      // Get the direction of the intersection line of the two planes
      atVector3<T> intersectionNormal = p1.Cross(p2).Normalize();

      // Get the direction of the closest intersecting lines for each bounding box
      atRay<T> ray1(obb1Point, intersectionNormal.Cross(p1));
      atRay<T> ray2(obb2Point, intersectionNormal.Cross(p2));

      ray2.ClosestPoint(ray1, &testPoint);
    }

    if (pOBBCol) *pOBBCol = atCollisionData(testPoint, p2);
    if (pOBB2Col) *pOBB2Col = atCollisionData(testPoint, p1);

    return true;
  }

  return false;
}

template<typename T> inline bool atCollision(const atAABB<T> &aabb, const atOBB<T> &obb, atCollisionData *pAABBCol, atCollisionData *pOBBCol)
{
  return Collision(atOBB(aabb.m_min, aabb.m_max), obb, pAABBCol, pOBBCol);
}

template<typename T> inline bool atCollision(const atAABB<T> &aabb, const atAABB<T> &aabb2, atCollisionData *pAABBCol, atCollisionData *pAABB2Col)
{
  if (atIntersects(aabb, aabb2))
  {
    atVector3<T> colPoint = aabb.OverlappingBox(aabb2).Center();
    if (pAABBCol) *pAABBCol = atCollisionData(colPoint, aabb2.ClosestPointBounds(colPoint) - colPoint);
    if (pAABB2Col) *pAABB2Col = atCollisionData(colPoint, aabb.ClosestPointBounds(colPoint) - colPoint);
    return true;
  }

  return false;
}

template<typename T> inline bool atCollision(const atSphere<T> &sphere, const atOBB<T> &obb, atCollisionData *pSphereCol, atCollisionData *pOBBCol)
{
  if (Collision(sphere, obb.m_aabb, pSphereCol, pOBBCol))
  {
    if (pOBBCol)
    {
      atVector3<T> point = obb.OBBToWorld(pOBBCol->Point());
      atVector3<T> normal = obb.m_orientation.Rotate(pOBBCol->Normal());
      if (pOBBCol)
        *pOBBCol = atCollisionData(point, normal);
    }
    return true;
  }

  return false;
}

template<typename T> inline bool atCollision(const atSphere<T> &sphere, const atAABB<T> &aabb, atCollisionData *pSphereCol, atCollisionData *pAABBCol)
{
  if (atIntersects(sphere, aabb))
  {
    atVector3<T> sphereColPoint = sphere.ClosestPointBounds(aabb.Center());
    atVector3<T> aabbColPoint = aabb.ClosestPointBounds(sphereColPoint);
    if (pSphereCol) *pSphereCol = atCollisionData(sphereColPoint, sphere.m_position - sphereColPoint);
    if (pAABBCol) *pAABBCol = atCollisionData(aabbColPoint)
      return true;
  }

  return false;
}

template<typename T> inline bool atCollision(const atSphere<T> &sphere, const atSphere<T> &sphere2, atCollisionData *pSphereCol, atCollisionData *pSphere2Col)
{
  atVector3<T> colPoint = 0;
  if (atIntersects(sphere, sphere2, &colPoint))
  {
    if (pSphereCol) *pSphereCol = atCollisionData(colPoint, sphere2.m_position - sphere.m_position);
    if (pSphere2Col) *pSphere2Col = atCollisionData(colPoint, sphere.m_position - sphere2.m_position);
    return true;
  }

  return false;
}

template<typename T> inline bool atCollision(const atOBB<T> &obb, const atAABB<T> &aabb, atCollisionData<T> *pOBBCol, atCollisionData<T> *pAABBCol)
{
  return atCollision(aabb, obb, pAABBCol, pOBBCol);
}

template<typename T> inline bool atCollision(const atOBB<T> &obb, const atSphere<T> &sphere, atCollisionData<T> *pOBBCol, atCollisionData<T> *pSphereCol)
{
  return atCollision(sphere, obb, pSphereCol, pOBBCol);
}

template<typename T> inline bool atCollision(const atAABB<T> &aabb, const atSphere<T> &sphere, atCollisionData<T> *pAABBCol, atCollisionData<T> *pSphereCol)
{
  return atCollision(sphere, aabb, pSphereCol, pAABBCol);
}
