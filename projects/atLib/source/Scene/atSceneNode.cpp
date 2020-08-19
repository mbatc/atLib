#include "atSceneNode.h"
#include "atScene.h"

atSceneNode::atSceneNode(const int64_t &id, const atString &name)
  : id(id)
  , name(name)
{}

atScene* atSceneNode::GetScene() const { return m_pScene; }

atSceneNode* atSceneNode::GetParent() const
{
  atScene *pScene = GetScene();
  int64_t id = pScene->m_hierarchy.GetParent(id);
  return pScene->GetNode(id);
}

bool atSceneNode::SetParent(const int64_t &parentID) { return GetScene()->m_hierarchy.Move(id, parentID); }
bool atSceneNode::SetParent(atSceneNode *pNode) { return pNode && SetParent(pNode->id); }
bool atSceneNode::AddChild(const int64_t &childID) { return GetScene()->m_hierarchy.Move(childID, id); }
bool atSceneNode::AddChild(atSceneNode *pNode) { return pNode && AddChild(pNode->id); }
bool atSceneNode::RemoveChild(const int64_t &childID, const int64_t &moveTo) { return GetScene()->m_hierarchy.Move(childID, moveTo); }
bool atSceneNode::RemoveChild(atSceneNode *pNode, atSceneNode *pMoveTo) { return pNode && RemoveChild(pNode->id, pMoveTo ? pMoveTo->id : atScene_RootNodeID); }

atSceneComponent* atSceneNode::GetComponent(const int64_t &index) const { return index >= 0 && index < m_components.size() ? m_components[index] : nullptr; }
atSceneComponent* atSceneNode::GetComponent(const atString &type) const { return GetComponent(FindComponent(type)); }

int64_t atSceneNode::GetComponentCount() const { return m_components.size(); }

int64_t atSceneNode::FindComponent(const atString &type) const
{
  for (int64_t i = 0; i < m_components.size(); ++i)
    if (m_components[i]->typeString == type)
      return i;
  return -1;
}

atSceneComponent* atSceneNode::AddComponent(const atString &type) { return HasComponent(type) ? nullptr : AddComponent(atSceneComponentFactory::Create(type)); }
bool atSceneNode::HasComponent(const atString &type) const { return GetComponent(type) != nullptr; }

bool atSceneNode::RemoveComponent(const int64_t &index)
{
  if (index >= 0 || index < m_components.size())
    return false;
  delete m_components[index];
  m_components.erase(index);
  return true;
}

bool atSceneNode::RemoveComponent(const atString &type) { return RemoveComponent(FindComponent(type)); }
bool atSceneNode::IsRoot() const { return GetScene()->GetRoot() == this; }

atTransformable<double> atSceneNode::GlobalTransform(const bool &reEvaluate)
{
  // TODO: Re-evaluate transform support
  return m_globalTransform;
}

bool atSceneNode::Init()
{
  bool success = true;
  for (atSceneComponent *pComponent : m_components)
    success &= pComponent->OnInit();
  return success;
}

bool atSceneNode::Update()
{
  bool success = true;
  for (atSceneComponent *pComponent : m_components)
    success &= pComponent->OnUpdate();
  return success;
}

bool atSceneNode::Render()
{
  bool success = true;
  for (atSceneComponent *pComponent : m_components)
    success &= pComponent->OnRender();
  return success;
}

bool atSceneNode::Destroy()
{
  bool success = true;
  for (atSceneComponent *pComponent : m_components)
    success &= pComponent->OnDestroy();
  return success;
}

atSceneComponent* atSceneNode::AddComponent(atSceneComponent *pComponent)
{
  if (!pComponent)
    return nullptr;

  pComponent->m_pNode = this;
  m_components.push_back(pComponent);
  return pComponent;
}
