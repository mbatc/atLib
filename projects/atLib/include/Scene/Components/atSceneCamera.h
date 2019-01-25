
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

#ifndef atSceneCamera_h__
#define atSceneCamera_h__

#include "atWindow.h"
#include "atTransformable.h"
#include "atSceneComponent.h"

class atSceneCamera : public atSceneComponent
{
public:
  atSceneCamera(const double aspect = 1.0, const double FOV = atDegs2Rads(60), const double nearPlane = 0.1, const double farPlane = 1000.0);

  void SetViewport(const atVec4I viewport);
  void SetViewport(atWindow &wnd);
  atVec4I Viewport() const;
  atMat4D ProjectionMat() const;

  double m_fov;
  double m_aspect;
  double m_farPlane;
  double m_nearPlane;
  atVec2F m_depthRange = atVec2I(0, 1);
 
  int64_t TypeID() const override;
  static const int64_t typeID;

protected:
  atVec4I m_viewport = -1;
};

class atSimpleCamera : public atSceneCamera, public atTransformable<double>
{
public:
  atSimpleCamera(atWindow &wnd, const atVec3D &pos = { 0,0,0 }, const atVec3D &rot = { 0,0,0 }, const double FOV = atDegs2Rads(60), const double nearPlane = 0.1, const double farPlane = 1000.0);

  atMat4D ViewMat() const;
  bool OnUpdate(const double dt) override;

  double m_moveSpeed = 1.0;
};

#endif // atCamera_h__
