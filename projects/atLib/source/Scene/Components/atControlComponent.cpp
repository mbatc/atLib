#include "atControlComponent.h"
#include "atSceneNode.h"
#include "atInput.h"

atControlComponent::atControlComponent(const atString &name) : atSceneComponent(TypeID(), name) {}

bool atControlComponent::Update(const double &deltaTime)
{
  atVec2D dMouse = atInput::MouseDelta();
  atVec2D rot;

  double speed = deltaTime * (atInput::ButtonDown(atKC_Shift) ? 2 : 1) * (atInput::ButtonDown(atKC_Control) ? 0.5 : 1);
  double rotSpeed = 0.01;

  if (!atInput::RightMouseDown())
  {
    atInput::LockMouse(false);
  }
  else
  {
    rot = { -dMouse.y * rotSpeed, -dMouse.x * rotSpeed };
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

  atTransformComponent *pTransform = GetNode()->GetTransform();
  pTransform->Translate(pTransform->Orientation().Rotate(move));

  m_yaw *= atQuatD(atVec3D(0, 1, 0), rot.y);
  m_pitch *= atQuatD(atVec3D(1, 0, 0), rot.x);

  pTransform->SetRotation(pTransform->Orientation().Slerp(m_yaw * m_pitch, 0.3));
  return true;
}

atCoreComponentType atControlComponent::TypeID() { return atCCT_Control; }

atSceneComponent* atControlComponent::Copy() const
{
  atControlComponent *pNew = atNew<atControlComponent>(m_name);
  pNew->m_yaw = m_yaw;
  pNew->m_pitch = m_pitch;
  return pNew;
}
