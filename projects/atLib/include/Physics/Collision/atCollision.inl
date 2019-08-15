
template<typename T> inline atCollisionData<T>::atCollisionData()
  : m_collided(false)
{}

template<typename T> inline atCollisionData<T>::atCollisionData(const atVec3D &point, const atVec3D &normal, const atVec3D &midPoint)
  : m_collided(true)
  , m_normal(normal.Normalize())
  , m_point(point)
  , m_midPoint(midPoint)
{}

template<typename T> inline bool atCollisionData<T>::Collided() const { return m_collided; }

template<typename T> inline const atVec3D& atCollisionData<T>::Point() const { return m_point; }

template<typename T> inline const atVec3D& atCollisionData<T>::Normal() const { return m_normal; }

template<typename T> inline const atVec3D& atCollisionData<T>::MidPoint() const { return m_midPoint; }

template<typename T> inline bool atCollision(const atOBB<T> &obb, const atOBB<T> &obb2, atCollisionData<T> *pOBBCol, atCollisionData<T> *pOBB2Col)
{
  if (atIntersects(obb, obb2))
  {
    // Calculate the intersection point
    atVector3<T> testPoint = atBounds(obb).OverlappingBox(atBounds(obb2)).Center();

    // Find the closest points that lie on the surface of each bounding box
    atVector3<T> obb1Point = obb.ClosestPointBounds(testPoint);
    atVector3<T> obb2Point = obb2.ClosestPointBounds(testPoint);

    // Get the normals of the colliding planes
    atVector3<T> p1 = obb.ClosestFaceNormal(testPoint);
    atVector3<T> p2 = obb2.ClosestFaceNormal(testPoint);

    if (1.0 - abs(p1.Dot(p2)) > 0.001)
    {
      // Get the direction of the intersection line of the two planes
      atVector3<T> intersectionNormal = p1.Cross(p2).Normalize();

      // Get the direction of the closest intersecting lines for each bounding box
      atRay<T> ray1(obb1Point, intersectionNormal.Cross(p1));
      atRay<T> ray2(obb2Point, intersectionNormal.Cross(p2));

      ray2.ClosestPoint(ray1, &testPoint);
    }
    
    atVec3D midPoint = (p2 + p1) / 2;
    if (pOBBCol) *pOBBCol = atCollisionData<T>(testPoint, p2, testPoint);
    if (pOBB2Col) *pOBB2Col = atCollisionData<T>(testPoint, p1, testPoint);

    return true;
  }

  return false;
}

template<typename T> inline bool atCollision(const atAABB<T> &aabb, const atOBB<T> &obb, atCollisionData<T> *pAABBCol, atCollisionData<T> *pOBBCol)
{
  return atCollision(atOBB(aabb.m_min, aabb.m_max), obb, pAABBCol, pOBBCol);
}

template<typename T> inline bool atCollision(const atAABB<T> &aabb, const atAABB<T> &aabb2, atCollisionData<T> *pAABBCol, atCollisionData<T> *pAABB2Col)
{
  if (atIntersects(aabb, aabb2))
  {
    atVector3<T> midPoint = aabb.OverlappingBox(aabb2).Center();
    if (pAABBCol)
    {
      atVector3<T> colPoint = aabb.ClosestPointBounds(midPoint);
      *pAABBCol = atCollisionData<T>(colPoint, aabb2.GetClosestFaceNormal(colPoint), midPoint);
    }
    if (pAABB2Col)
    {
      atVector3<T> colPoint = aabb2.ClosestPointBounds(midPoint);
      *pAABB2Col = atCollisionData<T>(colPoint, aabb.GetClosestFaceNormal(colPoint), midPoint);
    }
    return true;
  }

  return false;
}

template<typename T> inline bool atCollision(const atSphere<T> &sphere, const atOBB<T> &obb, atCollisionData<T> *pSphereCol, atCollisionData<T> *pOBBCol)
{
  atSphere<T> obbSphere = sphere;
  obbSphere.m_position = obb.WorldToOBB(obbSphere.m_position);

  if (atCollision(sphere, obb.m_aabb, pSphereCol, pOBBCol))
  {
    if (pOBBCol)
    {
      atVector3<T> point = obb.OBBToWorld(pOBBCol->Point());
      atVector3<T> midPoint = obb.OBBToWorld(pOBBCol->MidPoint());
      atVector3<T> normal = obb.m_orientation.Rotate(pOBBCol->Normal());
      *pOBBCol = atCollisionData<T>(point, normal, midPoint);
    }

    if (pSphereCol)
    {
      atVector3<T> point = obb.OBBToWorld(pSphereCol->Point());
      atVector3<T> midPoint = obb.OBBToWorld(pSphereCol->MidPoint());
      atVector3<T> normal = obb.m_orientation.Rotate(pSphereCol->Normal());
      *pSphereCol = atCollisionData<T>(point, normal, midPoint);
    }
    return true;
  }

  return false;
}

template<typename T> inline bool atCollision(const atSphere<T> &sphere, const atAABB<T> &aabb, atCollisionData<T> *pSphereCol, atCollisionData<T> *pAABBCol)
{
  if (atIntersects(aabb, sphere))
  {
    atVector3<T> aabbColPoint = aabb.ClosestPointBounds(sphere.m_position);
    atVector3<T> sphereColPoint = sphere.ClosestPointBounds(aabbColPoint);
    atVector3<T> midPoint = (sphereColPoint + aabbColPoint) / 2;
    if (pSphereCol) *pSphereCol = atCollisionData<T>(sphereColPoint, aabb.GetClosestFaceNormal(sphere.m_position), midPoint);
    if (pAABBCol) *pAABBCol = atCollisionData<T>(aabbColPoint, aabbColPoint - sphere.m_position, midPoint);
    return true;
  }

  return false;
}

template<typename T> inline bool atCollision(const atSphere<T> &sphere, const atSphere<T> &sphere2, atCollisionData<T> *pSphereCol, atCollisionData<T> *pSphere2Col)
{
  atVector3<T> colPoint = 0;
  if (atIntersects(sphere, sphere2, &colPoint))
  {
    if (pSphereCol) *pSphereCol = atCollisionData<T>(sphere.ClosestPointBounds(colPoint), sphere.m_position - sphere2.m_position, colPoint);
    if (pSphere2Col) *pSphere2Col = atCollisionData<T>(sphere2.ClosestPointBounds(colPoint), sphere2.m_position - sphere.m_position, colPoint);
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
