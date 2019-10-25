
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

#ifndef atOBB_h__
#define atOBB_h__

#include "atAABB.h"

template<typename T> class atOBB
{
public:
  using Vec3 = atVector3<T>;

  atOBB();
  atOBB(const atAABB<T> &aabb, const atQuaternion<T> &orientation = atQuaternion<T>::Identity());
  atOBB(const atAABB<T> &aabb, const atMatrix4x4<T> &rotation = atMatrix4x4<T>::Identity());
  atOBB(const atAABB<T> &aabb, const atVector3<T> &rotation = { 0 });
  atOBB(const atVector3<T> &min, const atVector3<T> &max, const atQuaternion<T> &orientation = atQuaternion<T>::Identity());
  atOBB(const atVector3<T> &min, const atVector3<T> &max, const atMatrix4x4<T> &rotation = atMatrix4x4<T>::Identity());
  atOBB(const atVector3<T> &min, const atVector3<T> &max, const atVector3<T> &rotation = { 0 });

  Vec3 Center() const;
  Vec3 Dimensions() const;
  Vec3 ClosestPoint(const Vec3 &point) const;
  Vec3 ClosestPointBounds(const Vec3 &point) const;
  Vec3 ClosestFaceNormal(const Vec3 &point) const;
  bool Contains(const Vec3 &point) const;

  // Rotate a point so that it is aligned with the OBB's local axis
  Vec3 WorldToOBB(const Vec3 &point) const;
  Vec3 OBBToWorld(const Vec3 &point) const;
  
  atAABB<T> m_aabb;
  atQuaternion<T> m_orientation;
};

template<typename T> atAABB<T> atBounds(const atOBB<T> &obb);

#include "atOBB.inl"
#endif // atOBB_h__
