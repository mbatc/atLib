#include "atSceneNode.h"
#include "atScene.h"

atSceneNode::atSceneNode(const atString &name)
  : m_name(name)
{
  AddComponent<atTransformComponent>();
}

atSceneNode::atSceneNode(atSceneNode &&o)
{
  m_components = std::move(o.m_components);
  m_pScene = o.m_pScene;
}

atSceneNode::atSceneNode(const atSceneNode &o)
{
  for (atSceneComponent *pComponent : o.m_components)
  {
    m_components.push_back(pComponent->Copy());
    m_components.back()->SetNode(nullptr, this);
  }
  m_pScene = nullptr;
}

atSceneNode::~atSceneNode()
{
  for (atSceneComponent *pComponent : m_components)
    atDelete(pComponent);
  m_components.clear();
}

const atString& atSceneNode::GetName() const { return m_name; }

void atSceneNode::SetName(const atString &name) { m_name = name; }

bool atSceneNode::Init()
{
  for (atSceneComponent *pComponent : m_components)
    if (!pComponent->Init())
      return false;
  return true;
}

bool atSceneNode::Update(const double &deltaTime)
{
  for (atSceneComponent *pComponent : m_components)
    if (!pComponent->Update(deltaTime))
      return false;
  return true;
}

bool atSceneNode::Render(const atMat4D &vp)
{
  for (atSceneComponent *pComponent : m_components)
    if (!pComponent->Render(vp))
      return false;
  return true;
}

bool atSceneNode::Destroy()
{
  for (atSceneComponent *pComponent : m_components)
    if (!pComponent->Destroy())
      return false;
  return true;
}

void atSceneNode::AddComponent_Internal(atSceneComponent *pComponent)
{
  m_components.push_back(pComponent);
  m_components.back()->SetNode(m_pScene, this);
}

atSceneComponent* atSceneNode::GetComponent(const atCoreComponentType &type, const int64_t &index) const
{
  int64_t curIdx = 0;
  for (atSceneComponent *pComp : m_components)
    if (pComp->typeID == type && curIdx++ == index)
      return pComp;
  return nullptr;
}

atVector<atSceneComponent*> atSceneNode::GetComponents(const atCoreComponentType &type) const
{
  atVector<atSceneComponent*> components;
  for (atSceneComponent *pComp : m_components)
    if (pComp->typeID == type)
      components.push_back(pComp);
  return components;
}

void atSceneNode::SetScene(atScene *pScene)
{
  m_pScene = pScene;

  for (atSceneComponent *pComponent : m_components)
    pComponent->SetNode(m_pScene, this);
}

int64_t atSceneNode::ComponentCount(const atCoreComponentType &type) const
{
  int64_t count = 0;
  for (atSceneComponent *pComponent : m_components)
    count += pComponent->TypeID() == type;
  return count;
}

atScene* atSceneNode::GetScene() const { return m_pScene; }
int64_t atSceneNode::ComponentCount() const { return m_components.size(); }

int64_t atSceneNode::ChildCount() const { return GetScene()->ChildCount(GetID()); }
int64_t atSceneNode::GetID() const { return GetScene()->GetNodeID(this); }

atSceneNode* atSceneNode::GetParent() const { return GetScene()->GetParent(GetID()); }
atSceneNode* atSceneNode::GetChild(const int64_t &index) const { return GetScene()->GetChild(GetID(), index); }

atTransformComponent* atSceneNode::GetTransform() const { return GetComponent<atTransformComponent>(); }
atSceneComponent* atSceneNode::GetComponent(const int64_t &index) const { return index < m_components.size() ? m_components[index] : nullptr; }
