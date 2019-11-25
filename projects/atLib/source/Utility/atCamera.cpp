
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

#include "atCamera.h"
#include "atInput.h"

atProjection::atProjection(const double &aspect, const double &fov, const double &nearPlane, const double &farPlane)
{
  SetProjection(aspect, fov, nearPlane, farPlane);
}

atProjection::atProjection(const atVec2F &dims, const double &fov, const double &nearPlane, const double &farPlane)
{
  SetProjection(dims, fov, nearPlane, farPlane);
}

atMat4D atProjection::ProjectionMat(const double &clipNearZ, const double &clipFarZ) const
{
  atUnused(clipNearZ, clipFarZ);
  return atMatrixProjection(m_aspect, m_fov, m_nearPlane, m_farPlane);
}

void atProjection::SetProjection(const double &aspect, const double &fov, const double &nearPlane, const double &farPlane)
{
  SetAspect(aspect);
  SetFOV(fov);
  SetNearPlane(nearPlane);
  SetFarPlane(farPlane);
}

void atProjection::SetProjection(const atVec2F &dims, const double &fov, const double &nearPlane, const double &farPlane)
{
  SetProjection(1, fov, nearPlane, farPlane);
  SetAspect(dims);
}

void atProjection::SetViewport(const atVec4I &viewport)
{
  m_viewport = viewport;
  SetAspect(atVec2F(m_viewport.z - m_viewport.x, m_viewport.w - m_viewport.y));
}

void atProjection::SetViewport(const atWindow *pWindow) { SetViewport(atVec4I(0, 0, pWindow->Size())); }
void atProjection::SetFOV(const double &fov) { m_fov = fov; }
void atProjection::SetAspect(const double &aspect) { m_aspect = aspect; }
void atProjection::SetAspect(const atVec2F &dims) { SetAspect(dims.x / dims.y); }
void atProjection::SetNearPlane(const double &nearPlane) { m_nearPlane = nearPlane; }
void atProjection::SetFarPlane(const double &farPlane) { m_farPlane = farPlane; }

const double& atProjection::Aspect() const { return m_aspect; }
const double& atProjection::FOV() const { return m_fov; }
const double& atProjection::NearPlane() const { return m_nearPlane; }
const double& atProjection::FarPlane() const { return m_farPlane; }
const atVec4I& atProjection::Viewport() const { return m_viewport; }

// Base camera - only provides a projection matrix - static camera

atCamera::atCamera(const double aspect, const double FOV, const double nearPlane, const double farPlane) : atProjection(aspect, FOV, nearPlane, farPlane) {}
atMat4D atCamera::ViewMat() const { return TransformMat().Inverse(); }

// Simple dynamic camera providing first person style controls

bool atFPSCamera::Update(const double dt)
{
  atVec2D dMouse = atInput::MouseDelta();
  atVec2D rot;

  double speed = m_moveSpeed * dt * (atInput::ButtonDown(atKC_Shift) ? 2 : 1) * (atInput::ButtonDown(atKC_Control) ? 0.5 : 1);
  double rotSpeed = 0.01;

  if (!atInput::RightMouseDown() && !m_lockMouse)
  {
    atInput::LockMouse(false);
  }
  else
  {
    rot = { -dMouse.y * rotSpeed * m_aspect * m_sensitivity.x, -dMouse.x * rotSpeed * m_sensitivity.y };
    atInput::LockMouse(true);
  }
  
  atVec3D move;
  if (atInput::ButtonDown(atKC_W)) move.z -= speed;
  if (atInput::ButtonDown(atKC_S)) move.z += speed;
  if (atInput::ButtonDown(atKC_D)) move.x += speed;
  if (atInput::ButtonDown(atKC_A)) move.x -= speed;
  if (atInput::ButtonDown(atKC_E)) move.y += speed;
  if (atInput::ButtonDown(atKC_X)) move.y -= speed;

  if (atInput::ButtonDown(atKC_Left)) rot.y += speed * 2;
  if (atInput::ButtonDown(atKC_Right)) rot.y -= speed * 2;
  if (atInput::ButtonDown(atKC_Up)) rot.x += speed * 2;
  if (atInput::ButtonDown(atKC_Down)) rot.x -= speed * 2;

  double curPitch = m_pitch.Angle();
  if (curPitch > atPi_2 && curPitch < (at2Pi - atPi_2))
    rot.y = -rot.y;

  Translate(Orientation().Rotate(move));
  m_yaw *= atQuatD(atVec3D(0, 1, 0), rot.y);
  m_pitch *= atQuatD(atVec3D(1, 0, 0), rot.x);
  SetRotation(Orientation().Slerp(m_yaw * m_pitch, 0.3));
  return true;
}

atFPSCamera::atFPSCamera(double aspect, const atVec3D &pos, const atVec3D &rot, const double FOV, const double nearPlane, const double farPlane)
  : atCamera(aspect, FOV, nearPlane, farPlane)
{
  m_translation = pos;
  SetRotation(rot);
}

atFPSCamera::atFPSCamera(const atWindow *pWnd, const atVec3D &pos, const atVec3D &rot, const double FOV, const double nearPlane, const double farPlane)
  : atCamera(1, FOV, nearPlane, farPlane)
{
  m_translation = pos;
  SetRotation(rot); 
  SetAspect(pWnd->Size());
}
