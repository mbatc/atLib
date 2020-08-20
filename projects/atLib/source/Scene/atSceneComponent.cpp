#include "atSceneComponent.h"
#include "atScene.h"

atSceneComponent::atSceneComponent(const int64_t &typeID, const atString &typeString)
  : typeID(typeID)
  , typeString(typeString)
{}

bool atSceneComponent::OnInit() { return true; }
bool atSceneComponent::OnUpdate() { return true; }
bool atSceneComponent::OnRender() { return true; }
bool atSceneComponent::OnDestroy() { return true; }

atSceneNode* atSceneComponent::GetNode() const { return m_pNode; }

atScene* atSceneComponent::GetScene() const { return GetNode()->GetScene(); }

atTransformable<double> atSceneComponent::GlobalTransform(const bool &reEvaluate)
{
  return GetNode()->GlobalTransform(reEvaluate);
}

atTransformable<double>& atSceneComponent::Transform() const { return *(atTransformable<double>*)GetNode(); }
