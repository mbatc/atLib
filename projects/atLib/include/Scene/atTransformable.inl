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
  : m_rotation(rot)
  , m_translation(trans)
  , m_scale(scale) 
{}

template<typename T> inline typename atTransformable<T>::Mat atTransformable<T>::RotationMat() const
{
  return atMatrixYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
}

template<typename T> inline typename atTransformable<T>::Mat atTransformable<T>::WorldMat() const
{
  return TranslationMat() * RotationMat() * ScaleMat();
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
  return TranslationMat() * RotationMat() * ScaleMat();
}
