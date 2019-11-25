#include "atCameraComponent.h"
#include "atSceneComponent.h"
#include "atSceneNode.h"
#include "atScene.h"

atCameraComponent::atCameraComponent(const atString &name) : atSceneComponent(TypeID(), name) {}

atCoreComponentType atCameraComponent::TypeID() { return atCCT_Camera; }

atMat4D atCameraComponent::ViewMat() const { return GetNode()->GetComponent<atTransformComponent>()->GlobalTransformMat().Inverse(); }

atMat4D atCameraComponent::ViewProjMat() const { return ProjectionMat() * ViewMat(); }

atSceneComponent* atCameraComponent::Copy() const
{
  atCameraComponent *pNew = atNew<atCameraComponent>(m_name);
  pNew->m_aspect = m_aspect;
  pNew->m_depthRange = m_depthRange;
  pNew->m_fov = m_fov;
  pNew->m_farPlane = m_farPlane;
  pNew->m_nearPlane = m_nearPlane;
  pNew->m_viewport = m_viewport;
  return pNew;
}
