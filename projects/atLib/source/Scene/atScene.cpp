#include "atScene.h"
#include <time.h>

class atSceneInitializer
{
public:
  void Leave(const int64_t &nodeID, atHierarchy<atSceneNode> *pHierarchy) {}
  bool Visit(const int64_t &nodeID, atHierarchy<atSceneNode> *pHierarchy) { return pHierarchy->Get(nodeID)->Init(); }
};

class atSceneUpdator
{
public:
  void Leave(const int64_t &nodeID, atHierarchy<atSceneNode> *pHierarchy) {}
  bool Visit(const int64_t &nodeID, atHierarchy<atSceneNode> *pHierarchy) { return pHierarchy->Get(nodeID)->Update(m_deltaTime); }

  double m_deltaTime = 0.016;
};

class atSceneRenderer
{
public:
  void Leave(const int64_t &nodeID, atHierarchy<atSceneNode> *pHierarchy) {}
  bool Visit(const int64_t &nodeID, atHierarchy<atSceneNode> *pHierarchy) { return pHierarchy->Get(nodeID)->Render(m_vp); }

  atMat4D m_vp = atMat4D::Identity();
};

class atSceneDestroyer
{
public:
  void Leave(const int64_t &nodeID, atHierarchy<atSceneNode> *pHierarchy) {}
  bool Visit(const int64_t &nodeID, atHierarchy<atSceneNode> *pHierarchy) { return pHierarchy->Get(nodeID)->Destroy(); }
};

class atSceneSetParent
{
public:
  atSceneSetParent(atScene *pScene) { m_pScene = pScene; }

  void Leave(const int64_t &nodeID, atHierarchy<atSceneNode> *pHierarchy) {}
  bool Visit(const int64_t &nodeID, atHierarchy<atSceneNode> *pHierarchy) { pHierarchy->Get(nodeID)->SetScene(m_pScene); return true; }

protected:
  atScene *m_pScene = nullptr;
};

atScene::atScene(atScene &&o) { *this = std::move(o); }
atScene::atScene(const atScene &o) { *this = o; }

atScene& atScene::operator=(atScene &&o)
{
  m_hierarchy = std::move(o.m_hierarchy);

  atSceneSetParent sceneSetter(this);
  m_hierarchy.Visit(&sceneSetter);
  return *this;
}

atScene& atScene::operator=(const atScene &o)
{
  m_hierarchy = o.m_hierarchy;

  atSceneSetParent sceneSetter(this);
  m_hierarchy.Visit(&sceneSetter);
  return *this;
}

void atScene::Init()
{
  atSceneInitializer visitor;
  m_hierarchy.Visit(&visitor);
  m_lastClock = clock();
}

void atScene::Update()
{
  atSceneUpdator visitor;
  visitor.m_deltaTime = double(clock() - m_lastClock) / CLOCKS_PER_SEC;
  m_hierarchy.Visit(&visitor);
  m_lastClock = clock();
}

void atScene::Render(const atMat4D &vp)
{
  atSceneRenderer visitor;
  visitor.m_vp = vp;
  m_hierarchy.Visit(&visitor);
}

void atScene::Destroy()
{
  atSceneDestroyer visitor;
  m_hierarchy.Visit(&visitor);
}

int64_t atScene::Add(atSceneNode &&o, const int64_t &parentID)
{
  int64_t id = m_hierarchy.Add(std::move(o), parentID);
  m_hierarchy.Get(id)->SetScene(this);
  return id;
}

int64_t atScene::Add(const atSceneNode &o, const int64_t &parentID)
{
  int64_t id = m_hierarchy.Add(std::move(o), parentID);
  m_hierarchy.Get(id)->SetScene(this);
  return id;
}

int64_t atScene::Insert(atScene *pScene, const int64_t &parentID, const bool &move)
{
  int64_t nodeID = m_hierarchy.Insert(&pScene->m_hierarchy, parentID, move);

  atSceneSetParent sceneSetter(this);
  m_hierarchy.Visit(&sceneSetter, parentID);
  return nodeID;
}

atScene atScene::Extract(const int64_t &parentID, const bool &move)
{
  atScene subscene;
  subscene.m_hierarchy = m_hierarchy.Extract(parentID);
  return subscene;
}

void atScene::Remove(const int64_t &nodeID) { m_hierarchy.Remove(nodeID); }

void atScene::Move(const int64_t &nodeID, const int64_t &toNodeID) { m_hierarchy.Move(nodeID, toNodeID); }
void atScene::MoveChildren(const int64_t &nodeID, const int64_t &toNodeID) { m_hierarchy.MoveChildren(nodeID, toNodeID); }

atSceneNode* atScene::Get(const int64_t &nodeID) { return m_hierarchy.Get(nodeID); }
const atSceneNode* atScene::Get(const int64_t &nodeID) const { return m_hierarchy.Get(nodeID); }

atSceneNode* atScene::GetParent(const int64_t &nodeID)
{
  int64_t id = m_hierarchy.Parent(nodeID);
  return id == nodeID ? nullptr : Get(id);
}

const atSceneNode* atScene::GetParent(const int64_t &nodeID) const
{
  int64_t id = m_hierarchy.Parent(nodeID);
  return id == nodeID ? nullptr : Get(id);
}

int64_t atScene::GetNodeID(const atSceneNode *pNode) const { return m_hierarchy.GetID(pNode); }

atSceneNode* atScene::GetChild(const int64_t &nodeID, const int64_t &index) { return m_hierarchy.Child(nodeID, index); }
const atSceneNode* atScene::GetChild(const int64_t &nodeID, const int64_t &index) const { return m_hierarchy.Child(nodeID, index); }

int64_t atScene::ChildCount(const int64_t &nodeID) const { return m_hierarchy.ChildCount(nodeID); }
atVector<int64_t> atScene::Children(const int64_t &nodeID) const { return m_hierarchy.Children(nodeID); }
