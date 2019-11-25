#include "atTransformable.h"

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

template<typename T> atTransformable<T>::atTransformable(const Vec& trans, const Vec& rot, const Vec& scale)
  : m_rotation(atMatrixYawPitchRoll(rot.y, rot.x, rot.z))
  , m_translation(trans)
  , m_scale(scale) 
{}

template<typename T> inline typename atTransformable<T>::Mat atTransformable<T>::RotationMat() const
{
  return atMatrixRotation(m_rotation);
}

template<typename T> inline typename const atTransformable<T>::Vec& atTransformable<T>::Pivot() const
{
  return m_pivot;
}

template<typename T> inline void atTransformable<T>::Rotate(const Vec &rot)
{
  Rotate(Quat(rot.x, rot.y, rot.z));
}

template<typename T> inline void atTransformable<T>::Rotate(const Mat &rot)
{
  Rotate(Quat(rot));
}

template<typename T> inline void atTransformable<T>::Rotate(const Quat &rot)
{
  m_rotation *= rot;
}

template<typename T> inline void atTransformable<T>::Translate(const Vec &translation)
{
  m_translation += translation;
}

template<typename T> inline void atTransformable<T>::Scale(const Vec &scale)
{
  m_scale *= scale;
}


template<typename T> inline void atTransformable<T>::SetTransform(const Mat &transform)
{
  Vec rotation = atMatrixExtractRotation(transform);
  SetRotation(rotation);
  m_scale = atMatrixExtractScale(transform);
  m_translation = atMatrixExtractTranslation(transform);
}

template<typename T> inline void atTransformable<T>::SetRotation(const Vec &rotation)
{
  m_rotation.Set(rotation.x, rotation.y, rotation.z);
}

template<typename T> inline void atTransformable<T>::SetRotation(const Mat &rotation)
{
  m_rotation.Set(rotation);
}

template<typename T> inline void atTransformable<T>::SetRotation(const Quat &rotation)
{
  m_rotation = rotation;
}

template<typename T> inline void atTransformable<T>::SetTranslation(const Vec &translation)
{
  m_translation = translation;
}

template<typename T> inline void atTransformable<T>::SetScale(const Vec &scale)
{
  m_scale = scale;
}

template<typename T> inline void atTransformable<T>::SetPivot(const Vec &pivot)
{
  m_pivot = pivot;
}

template<typename T> inline typename atTransformable<T>::Mat atTransformable<T>::ScaleMat() const
{
  return atMatrixScale(m_scale);
}

template<typename T> inline typename atTransformable<T>::Mat atTransformable<T>::TranslationMat() const
{
  return atMatrixTranslation(m_translation);
}

template<typename T> inline typename atTransformable<T>::Mat atTransformable<T>::TransformMat() const
{
  return TranslationMat() * RotationMat() * ScaleMat() * atMatrixTranslation(-m_pivot);
}

template<typename T> inline typename atTransformable<T>::Vec atTransformable<T>::RotationEuler() const
{
  return m_rotation.EulerAngles();
}

template<typename T> inline const typename atTransformable<T>::Quat& atTransformable<T>::Orientation() const
{
  return m_rotation;
}

template<typename T> inline const typename atTransformable<T>::Vec& atTransformable<T>::Scale() const
{
  return m_scale;
}

template<typename T> inline const typename atTransformable<T>::Vec& atTransformable<T>::Translation() const
{
  return m_translation;
}
