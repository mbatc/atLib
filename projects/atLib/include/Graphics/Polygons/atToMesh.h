#ifndef atToMesh_h__
#define atToMesh_h__

#include "atMesh.h"
#include "atOBB.h"
#include "atAABB.h"
#include "atRect.h"
#include "atPlane.h"
#include "atSphere.h"
#include "atTriangle.h"

template<typename T> atMesh atToMesh(const atTriangle<T> &tri);
template<typename T> atMesh atToMesh(const atRect<T> &tri);
template<typename T> atMesh atToMesh(const atPlane<T> &tri);
template<typename T> atMesh atToMesh(const atSphere<T> &tri);
template<typename T> atMesh atToMesh(const atAABB<T> &tri);
template<typename T> atMesh atToMesh(const atOBB<T> &tri);

#include "atToMesh.inl"

#endif // atToMesh_h__
