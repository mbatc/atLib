#include "atScriptComponent.h"

atScriptComponent::atScriptComponent(const atString &name) : atSceneComponent(TypeID(), name) {}

atCoreComponentType atScriptComponent::TypeID() { return atCCT_Script; }

atSceneComponent* atScriptComponent::Copy() const
{
  atScriptComponent *pNew = atNew<atScriptComponent>(m_name);
  return pNew;
}
