
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

atCamera::atCamera(const atWindow &wnd, const atVec3F64 &pos, const atVec3F64 &rot, const double FOV, const double nearPlane, const double farPlane)
  : atTransformable(pos, rot)
  , m_fov(FOV)
  , m_nearPlane(nearPlane)
  , m_farPlane(farPlane)
  , m_aspect((double)wnd.Size().x / (double)wnd.Size().y)
{}

void atCamera::Update(const double moveSpeed, const double dt)
{
  atVec2F64 dMouse = atInput::MouseDelta();
  atVec3F64 rot;
  const double speed = moveSpeed * dt * (atInput::KeyDown(atKC_Shift) ? 2 : 1) * (atInput::KeyDown(atKC_Control) ? 0.5 : 1);
  const double rotSpeed = 0.4 * dt;
  if (!atInput::RightMouseDown())
    atInput::LockMouse(false);
  else
    rot = { -dMouse.y * rotSpeed, -dMouse.x * rotSpeed * m_aspect, 0 };
  atVec3F64 move;
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
}

atMat4D atCamera::ProjectionMat() const { return atMatrixProjection(m_aspect, m_fov, m_nearPlane, m_farPlane); }
atMat4D atCamera::ViewMat() const { return TransformMat().Inverse(); }
