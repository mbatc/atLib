
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

#ifndef atTransformable_h__
#define atTransformable_h__

#include "atMath.h"

template<typename T> class atTransformable
{
public:
  typedef atVector3<T> Vec;
  typedef atMatrix4x4<T> Mat;
  typedef atQuaternion<T> Quat;

  atTransformable(const Vec &trans = { 0,0,0 }, const Vec &rot = { 0,0,0 }, const Vec &scale = { 1,1,1 });

  // Get the scale matrix
  Mat ScaleMat() const;

  // Get the translation matrix
  Mat TranslationMat() const;

  // Get the transformation matrix. This matrix consists of the following
  // transformations:
  //
  //  - Translation by -pivot
  //  - Non-Uniform Scale by m_scale
  //  - Rotation by m_rotation
  //  - Translation by m_translation
  //
  //  Applied in the described order
  Mat TransformMat() const;

  // Get the rotation as euler angles (x-pitch, y-yaw, z-roll)
  Vec RotationEuler() const;

  // Get the rotation as a quaternion
  const Quat& Orientation() const;

  // Get the scale
  const Vec& Scale() const;

  // Get the translation
  const Vec& Translation() const;

  // Get the rotation matrix
  Mat RotationMat() const;

  // Get the pivot
  const Vec& Pivot() const;

  // Apply a rotation 
  virtual void Rotate(const Vec &rot);
  virtual void Rotate(const Mat &rot);
  virtual void Rotate(const Quat &rot);

  // Apply a translation
  virtual void Translate(const Vec &translation);
  
  // Apply a scale
  virtual void Scale(const Vec &scale);

  virtual void SetRotation(const Vec &rotation);
  virtual void SetRotation(const Mat &rotation);
  virtual void SetRotation(const Quat &rotation);
  virtual void SetTranslation(const Vec &translation);
  virtual void SetScale(const Vec &scale);
  virtual void SetPivot(const Vec &pivot);

  void SetTransform(const Mat &transform);

protected:
  Vec m_translation;
  Vec m_pivot;
  Vec m_scale;
  Quat m_rotation;
};

#include "atTransformable.inl"
#endif // atTransformable_h__
