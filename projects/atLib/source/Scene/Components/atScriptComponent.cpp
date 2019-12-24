#include "atScriptComponent.h"

atScriptComponent::atScriptComponent(const atString &name) : atSceneComponent(TypeID(), name) {}

ScriptComponent_InitCallback atScriptComponent::InitCallback = nullptr;
ScriptComponent_UpdateCallback atScriptComponent::UpdateCallback = nullptr;
ScriptComponent_RenderCallback atScriptComponent::RenderCallback = nullptr;
ScriptComponent_DestroyCallback atScriptComponent::DestroyCallback = nullptr;

bool atScriptComponent::Init()
{
  if (InitCallback)
    return InitCallback(this);
  return false;
}

bool atScriptComponent::Update(const double &deltaTime)
{
  if (UpdateCallback)
    return UpdateCallback(this, deltaTime);
  return false;
}

bool atScriptComponent::Render(const atMat4D &vp)
{
  if (RenderCallback)
    return RenderCallback(this, vp);
  return false;
}

bool atScriptComponent::Destroy()
{
  if (DestroyCallback)
    return DestroyCallback(this);
  return false;
}

atCoreComponentType atScriptComponent::TypeID() { return atCCT_Script; }

atSceneComponent* atScriptComponent::Copy() const
{
  atScriptComponent *pNew = atNew<atScriptComponent>(m_name);
  return pNew;
}
