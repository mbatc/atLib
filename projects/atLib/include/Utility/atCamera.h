
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

#include "atMath.h"
#include "atWindow.h"
#include "atTransformable.h"

// Simple view projection

class atProjection
{
public:
  atProjection(const double &aspect = 1.0, const double &fov = atDegs2Rads(60), const double &nearPlane = 0.1, const double &farPlane = 1000);
  atProjection(const atVec2F &dims, const double &fov = atDegs2Rads(60), const double &nearPlane = 0.1, const double &farPlane = 1000);

  atMat4D ProjectionMat(const double &clipNearZ = atClipNearZ<double>(), const double &clipFarZ = atClipFarZ<double>()) const;

  void SetProjection(const double &aspect, const double &fov, const double &nearPlane, const double &farPlane);
  void SetProjection(const atVec2F &dims, const double &fov, const double &nearPlane, const double &farPlane);

  void SetViewport(const atWindow *pWindow);
  void SetViewport(const atVec4I &viewport);
  
  void SetFOV(const double &fov);
  void SetAspect(const double &aspect);
  void SetAspect(const atVec2F &dims);
  void SetNearPlane(const double &nearPlane);
  void SetFarPlane(const double &farPlane);

  const double& Aspect() const;
  const double& FOV() const;
  const double& NearPlane() const;
  const double& FarPlane() const;

  const atVec4I& Viewport() const;

protected:
  double m_fov;
  double m_aspect;
  double m_farPlane;
  double m_nearPlane;
  atVec2F m_depthRange = atVec2I(0, 1);
  atVec4I m_viewport = -1;
};

// Static camera (Projection with a transform)

class atCamera : public atTransformable<double>, public atProjection
{
public:
  atCamera(const double aspect = 1.0, const double FOV = atDegs2Rads(60), const double nearPlane = 0.1, const double farPlane = 1000.0);
  atMat4D ViewMat() const;
};

// Camera with simple FPS controls

class atFPSCamera : public atCamera
{
public:
  atFPSCamera(double aspect = 1.0, const atVec3D &pos = { 0,0,0 }, const atVec3D &rot = { 0,0,0 }, const double FOV = atDegs2Rads(60), const double nearPlane = 0.1, const double farPlane = 1000.0);
  atFPSCamera(const atWindow *pWindow, const atVec3D &pos = { 0,0,0 }, const atVec3D &rot = { 0,0,0 }, const double FOV = atDegs2Rads(60), const double nearPlane = 0.1, const double farPlane = 1000.0);

  bool Update(const double dt);

  double m_moveSpeed = 1.0;
  bool m_lockMouse = false;
  atVec2F m_sensitivity = { 1, 1 };

protected:
  atQuatD m_yaw;
  atQuatD m_pitch;
};

#endif // atCamera_h__
