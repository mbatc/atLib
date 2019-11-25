#include "atTransformComponent.h"
#include "atSceneComponent.h"
#include "atSceneNode.h"
#include "atScene.h"

atTransformComponent::atTransformComponent(const atString &name) : atSceneComponent(TypeID(), name) {}

typename atTransformable<double>::Mat atTransformComponent::GlobalScaleMat() const { return atMatrixScale(GlobalScale()); }
typename atTransformable<double>::Mat atTransformComponent::GlobalTranslationMat() const { return atMatrixTranslation(GlobalTranslation()); }

typename atTransformable<double>::Mat atTransformComponent::GlobalTransformMat() const
{
  atSceneNode *pParent = GetNode()->GetParent();
  return TransformMat() * (pParent ? pParent->GetTransform()->GlobalTransformMat() : atMat4D::Identity());
}

typename atTransformable<double>::Vec atTransformComponent::GlobalRotationEuler() const
{
  atSceneNode *pParent = GetNode()->GetParent();
  return RotationEuler() * (pParent ? pParent->GetTransform()->GlobalRotationEuler() : atVec3D::zero());
}

const typename atTransformable<double>::Quat atTransformComponent::GlobalOrientation() const
{
  atSceneNode *pParent = GetNode()->GetParent();
  return Orientation() * (pParent ? pParent->GetTransform()->GlobalOrientation() : atQuatD::Identity());
}

const typename atTransformable<double>::Vec atTransformComponent::GlobalScale() const
{
  atSceneNode *pParent = GetNode()->GetParent();
  return Scale() * (pParent ? pParent->GetTransform()->GlobalScale() : atVec3D::one());
}

const typename atTransformable<double>::Vec atTransformComponent::GlobalTranslation() const
{
  atSceneNode *pParent = GetNode()->GetParent();
  return  Translation() + (pParent ? pParent->GetTransform()->GlobalTranslation() : atVec3D::zero());
}

typename atTransformable<double>::Mat atTransformComponent::GlobalRotationMat() const
{
  atSceneNode *pParent = GetNode()->GetParent();
  return RotationMat() * (pParent ? pParent->GetTransform()->GlobalRotationMat() : atMat4D::Identity());
}

atSceneComponent* atTransformComponent::Copy() const
{
  atTransformComponent *pNew = atNew<atTransformComponent>(m_name);
  pNew->SetTranslation(Translation());
  pNew->SetScale(Scale());
  pNew->SetRotation(RotationMat());
  return pNew;
}

atCoreComponentType atTransformComponent::TypeID() { return atCCT_Transform; }
