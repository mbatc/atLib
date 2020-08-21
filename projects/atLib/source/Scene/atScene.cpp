#include "atScene.h"

atScene::atScene(const atString &name)
  : name(name)
{
  m_hierarchy.Root() = new atSceneNode(atScene_RootNodeID, "Root");
}

atSceneNode* atScene::GetRoot() const { return m_hierarchy.Root(); }

atSceneNode* atScene::GetNode(const int64_t &nodeID) const { return m_hierarchy.Contains(nodeID) ? m_hierarchy.Get(nodeID) : nullptr; }

atSceneNode* atScene::AddNode(const atString &name, const int64_t &parentID)
{
  atSceneNode *pNode = new atSceneNode(m_hierarchy.Add(nullptr, parentID), name);
  m_hierarchy.Get(pNode->id) = pNode;
  return pNode;
}

bool atScene::RemoveNode(const int64_t &nodeID)
{
  atUnused(nodeID);
  m_hierarchy.Get();
  return false;
}

atSceneNode* atScene::FindByTag(const atString &tag, const int64_t &root) const
{
  int64_t id = m_hierarchy.Find([tag](int64_t nodeID, const atHierarchy<atSceneNode*> *pTree) {
    return pTree->Get(nodeID)->tags.Contains(tag);
  });

  return id == atScene_InvalidNodeID ? nullptr : m_hierarchy.Get(id);
}

atSceneNode* atScene::FindByName(const atString &name, const int64_t &root) const
{
  int64_t id = m_hierarchy.Find([name](int64_t nodeID, const atHierarchy<atSceneNode*> *pTree) {
    return pTree->Get(nodeID)->name == name;
  });

  return id == atScene_InvalidNodeID ? nullptr : m_hierarchy.Get(id);
}
