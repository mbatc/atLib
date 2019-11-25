#include "atSceneComponent.h"
#include "atSceneNode.h"
#include "atScene.h"

void atSceneComponent::SetNode(atScene *pScene, atSceneNode *pNode)
{
  m_pScene = pScene;
  m_pNode = pNode;
}

atSceneComponent::atSceneComponent(const atCoreComponentType &type, const atString &name) : m_name(name), typeID(type) {}
atSceneComponent::atSceneComponent(const atString &typeName, const atString &name) : atSceneComponent(atCoreComponentType_FromString(typeName), name) {}

bool atSceneComponent::Init() { return true; }
bool atSceneComponent::Update(const double &deltaTime) { return true; }
bool atSceneComponent::Render(const atMat4D &vp) { atUnused(vp); return true; }
bool atSceneComponent::Destroy() { return true; }

atCoreComponentType atSceneComponent::TypeID() { return atCCT_Unknown; }

void atSceneComponent::SetName(const atString &name) { m_name = name; }
const atString& atSceneComponent::GetName() const { return m_name; }

atScene* atSceneComponent::GetScene() const { return m_pScene; }
atSceneNode* atSceneComponent::GetNode() const { return m_pNode; }
