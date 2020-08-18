
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

#include "atCamera.h"
#include "atInput.h"

// Base camera - only provides a projection matrix - static camera
atCamera::atCamera(const double aspect, const double FOV, const double nearPlane, const double farPlane)
  : m_fov(FOV)
  , m_nearPlane(nearPlane)
  , m_farPlane(farPlane)
  , m_aspect(aspect)
{}

void atCamera::SetViewport(const atVec4I viewport)
{
  m_viewport = viewport;
  m_aspect = (double)(viewport.z) / (double)(viewport.w);
}

atMat4D atCamera::ProjectionMat(const double &clipNearZ, const double &clipFarZ) const
{
  return atMatrixProjection(m_aspect, m_fov, m_nearPlane, m_farPlane, clipNearZ, clipFarZ);
}

void atCamera::SetViewport(const atWindow *pWnd)
{
  SetViewport(atVec4I(0, 0, pWnd->Width(), pWnd->Height()));
}

atMat4D atCamera::ViewMat() const
{
  return TransformMat().Inverse();
}

atVec4I atCamera::Viewport() const
{
  return m_viewport;
}

bool atFPSCamera::Update(const atNanoSeconds &dt)
{
  atVec2D dMouse = atInput::MouseDelta();
  atVec2D rot;

  double speedMult = (atInput::ButtonDown(atKC_Shift) ? 2 : 1) * (atInput::ButtonDown(atKC_Control) ? 0.5 : 1);
  double speed = m_moveSpeed * dt.ToSecondsf() * speedMult;
  double kbRotSpeed = dt.ToSecondsf() * speedMult * 2;
  double msRotSpeed = 0.01;
  if (!atInput::RightMouseDown() && !m_lockMouse)
  {
    atInput::LockMouse(false);
  }
  else
  {
    rot = { -dMouse.y * msRotSpeed * m_aspect * m_sensitivity.x, -dMouse.x * msRotSpeed * m_sensitivity.y };
    atInput::LockMouse(true);
  }
  
  atVec3D move;
  if (atInput::ButtonDown(atKC_W)) move.z -= speed;
  if (atInput::ButtonDown(atKC_S)) move.z += speed;
  if (atInput::ButtonDown(atKC_D)) move.x += speed;
  if (atInput::ButtonDown(atKC_A)) move.x -= speed;
  if (atInput::ButtonDown(atKC_E)) move.y += speed;
  if (atInput::ButtonDown(atKC_X)) move.y -= speed;

  if (atInput::ButtonDown(atKC_Left)) rot.y += kbRotSpeed;
  if (atInput::ButtonDown(atKC_Right)) rot.y -= kbRotSpeed;
  if (atInput::ButtonDown(atKC_Up)) rot.x += kbRotSpeed;
  if (atInput::ButtonDown(atKC_Down)) rot.x -= kbRotSpeed;

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
  : atCamera((double)pWnd->Size().x / (double)pWnd->Size().y, FOV, nearPlane, farPlane)
{
  m_translation = pos;
  SetRotation(rot); 
}
