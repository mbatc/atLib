
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#ifndef atIntersects_h__
#define atIntersects_h__

#include "atMath.h"

template<typename T> class atRay;
template<typename T> class atBVH;
template<typename T> class atOBB;
template<typename T> class atAABB;
template<typename T> class atRect;
template<typename T> class atPlane;
template<typename T> class atSphere;
template<typename T> class atTriangle;
template<typename T> struct atBVHNode;

// Geometry intersections
template<typename T> bool atIntersects(const atAABB<T> &aabb, const atAABB<T> &aabb2);
template<typename T> bool atIntersects(const atSphere<T> &sphere, const atSphere<T> &sphere2, atVector3<T> *pPoint = nullptr);
template<typename T> bool atIntersects(const atTriangle<T> &tri, const atTriangle<T> &tri2, atVector3<T> *pPoint);
template<typename T> bool atIntersects(const atRect<T> &rect, const atRect<T> &ray2);
template<typename T> bool atIntersects(const atAABB<T> &aabb, const atSphere<T> &sphere);
template<typename T> bool atIntersects(const atOBB<T> &obb, const atSphere<T> &sphere);
template<typename T> bool atIntersects(const atOBB<T> &obb, const atOBB<T> &obb2);
template<typename T> bool atIntersects(const atAABB<T> &aabb, const atOBB<T> &obb);

// Ray intersections
template<typename T> bool atIntersects(const atRay<T> &ray, const atRay<T> &ray2, T *pTime);
template<typename T> bool atIntersects(const atRay<T> &ray, const atPlane<T> &tri, T* pTime = nullptr);
template<typename T> bool atIntersects(const atRay<T> &ray, const atSphere<T> &sphere, T* pTime = nullptr);
template<typename T> bool atIntersects(const atRay<T> &ray, const atAABB<T> &box, T* pTime = nullptr);
template<typename T> bool atIntersects(const atRay<T> &ray, const atOBB<T> &obb, T* pTime = nullptr);
template<typename T> bool atIntersects(const atRay<T> &ray, const atTriangle<T> &tri, T* pTime = nullptr);
template<typename T, typename T2> bool atIntersects(const atRay<T2> &ray, const atRay<T> &ray2, T2* pTime);
template<typename T, typename T2> bool atIntersects(const atRay<T2> &ray, const atBVH<T> &bvh, T2* pTime);
template<typename T, typename T2> bool atIntersects(const atRay<T2> &ray, const atBVHNode<T> &bvhNode, T2* pTime);

#include "atIntersects.inl"
#endif // atIntersects_h__

