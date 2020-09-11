
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

#ifndef atSceneCamera_h__
#define atSceneCamera_h__

#include "atMath.h"
#include "atWindow.h"
#include "atTimespan.h"
#include "atTransformable.h"

class atProjection
{
public:
  atProjection(const atVec2I &viewDims, const double &FOV = atDegs2Rads(60), const double &nearPlane = 0.1, const double &farPlane = 1000.0);
  atProjection(const double &aspect = 1.0, const double &FOV = atDegs2Rads(60), const double &nearPlane = 0.1, const double &farPlane = 1000.0);

  atMat4D ProjectionMat(const double &clipNearZ = atClipNearZ<double>(), const double &clipFarNear = atClipFarZ<double>()) const;

  atVec4I viewport = -1;

  double fov;
  double aspect;
  double farPlane;
  double nearPlane;
};

class atCamera : public atProjection, public atTransformable<double>
{
public:
  atCamera(const atVec3D &pos = { 0, 0, 0 }, const atVec3D &rotation = { 0, 0, 0 }, const double &aspect = 1.0, const double &FOV = atDegs2Rads(60), const double &nearPlane = 0.1, const double &farPlane = 1000.0);

  void SetViewport(const atVec4I viewport);
  void SetViewport(const atWindow *pWindow);

  atMat4D ViewMat() const;
};

class atFPSCamera : public atCamera
{
public:
  atFPSCamera(double aspect = 1.0, const atVec3D &pos = { 0,0,0 }, const atVec3D &rot = { 0,0,0 }, const double FOV = atDegs2Rads(60), const double nearPlane = 0.1, const double farPlane = 1000.0);
  atFPSCamera(const atWindow *pWindow, const atVec3D &pos = { 0,0,0 }, const atVec3D &rot = { 0,0,0 }, const double FOV = atDegs2Rads(60), const double nearPlane = 0.1, const double farPlane = 1000.0);

  bool Update(const atNanoSeconds &dt);

  double m_moveSpeed = 1.0;
  bool m_lockMouse = false;
  atVec2F m_sensitivity = { 1, 1 };

protected:
  atQuatD m_yaw;
  atQuatD m_pitch;
};

#endif // atCamera_h__
