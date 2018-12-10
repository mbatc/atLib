
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

#ifndef atCamera_h__
#define atCamera_h__

#include "atWindow.h"
#include "atTransformable.h"

class atCamera : public atTransformable<double>
{
public:
  atCamera(const atWindow &wnd, const atVec3F64 &pos = { 0,0,0 }, const atVec3F64 &rot = { 0,0,0 }, const double FOV = atDegs2Rads(60), const double nearPlane = 0.1, const double farPlane = 1000.0);

  void SetProjection(const atWindow &wnd);

  atMat4D ProjectionMat() const;
  atMat4D ViewMat() const;

  void Update(const double moveSpeed, const double dt = 0.016);

  double m_fov;
  double m_nearPlane;
  double m_farPlane;
  double m_aspect;
};

#endif // atCamera_h__
