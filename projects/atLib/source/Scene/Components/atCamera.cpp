
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

const int64_t atCamera::typeID = atSCT_Camera;
int64_t atCamera::TypeID() const { return typeID; }

atCamera::atCamera(const double aspect, const double FOV, const double nearPlane, const double farPlane)
  : m_fov(FOV)
  , m_nearPlane(nearPlane)
  , m_farPlane(farPlane)
  , m_aspect(aspect)
{}

bool atSimpleCamera::Update(const double dt)
{
  atVec2D dMouse = atInput::MouseDelta();
  atVec3D rot;
  const double speed = m_moveSpeed * dt * (atInput::KeyDown(atKC_Shift) ? 2 : 1) * (atInput::KeyDown(atKC_Control) ? 0.5 : 1);
  const double rotSpeed = 0.4 * dt;
  if (!atInput::RightMouseDown())
    atInput::LockMouse(false);
  else
    rot = { -dMouse.y * rotSpeed, -dMouse.x * rotSpeed * m_aspect, 0 };
  atVec3D move;
  if (atInput::KeyDown(atKC_W)) move.z -= speed;
  if (atInput::KeyDown(atKC_S)) move.z += speed;
  if (atInput::KeyDown(atKC_D)) move.x += speed;
  if (atInput::KeyDown(atKC_A)) move.x -= speed;
  if (atInput::KeyDown(atKC_E)) move.y += speed;
  if (atInput::KeyDown(atKC_X)) move.y -= speed;
  if (atInput::KeyDown(atKC_Left)) rot.y += speed;
  if (atInput::KeyDown(atKC_Right)) rot.y -= speed;
  if (atInput::KeyDown(atKC_Up)) rot.x += speed;
  if (atInput::KeyDown(atKC_Down)) rot.x -= speed;
  m_translation += (RotationMat().Inverse() * move);
  m_rotation += rot;
  return true;
}

void atCamera::SetViewport(const atVec4I viewport)
{
  m_viewport = viewport;
  m_aspect = (double)(viewport.z) / (double)(viewport.w);
}

atSimpleCamera::atSimpleCamera(atWindow &wnd, const atVec3D &pos, const atVec3D &rot, const double FOV, const double nearPlane, const double farPlane) : atCamera((double)wnd.Size().x / (double)wnd.Size().y, FOV, nearPlane, farPlane) { m_translation = pos; m_rotation = rot; }
atMat4D atCamera::ProjectionMat() const { return atMatrixProjection(m_aspect, m_fov, m_nearPlane, m_farPlane); }
void atCamera::SetViewport(atWindow &wnd) { SetViewport(atVec4I(0, 0, wnd.Width(), wnd.Height())); }
atMat4D atSimpleCamera::ViewMat() const { return TransformMat().Inverse(); }
atVec4I atCamera::Viewport() const { return m_viewport; }
