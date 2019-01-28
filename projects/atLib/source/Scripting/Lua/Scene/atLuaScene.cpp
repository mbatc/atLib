#include "atLuaScene.h"

atLuaScene::atLuaScene(atScene *pScene)
{
}

atLuaScene::atLuaScene(const atLuaScene &copy)
{
}

atLuaScene::atLuaScene(atLuaScene &&move)
{
}

atLuaScene::~atLuaScene()
{
}

const char * atLuaScene::GetName() const
{
  return nullptr;
}

int64_t atLuaScene::GetRootNodeID() const
{
  return int64_t();
}

atLuaSceneNode atLuaScene::GetRootNode() const { return atLuaSceneNode(m_pScene->GetRoot()); }
atLuaSceneNode atLuaScene::GetNode(const int64_t id) const { return atLuaSceneNode(m_pScene->GetNode(id)); }
atVector<int64_t> atLuaScene::GetNodeIDs() const { m_pScene->GetNodeIDs(); }

int64_t atLuaScene::CreateNodePRSP(const char * name, const atVec3D & position, const atVec3D & rotation, const atVec3D & scale, const int64_t parentID) { return m_pScene->CreateNode(name, position, rotation, scale, m_pScene->GetNode(parentID))->ID(); }
int64_t atLuaScene::CreateNodePRS(const char *name, const atVec3D &position, const atVec3D &rotation, const atVec3D &scale) { return m_pScene->CreateNode(name, position, rotation, scale)->ID(); }
int64_t atLuaScene::CreateNodePR(const char *name, const atVec3D &position, const atVec3D &rotation) { return m_pScene->CreateNode(name, position, rotation)->ID(); }
int64_t atLuaScene::CreateNodeP(const char *name, const atVec3D &position) { return m_pScene->CreateNode(name, position)->ID; }
int64_t atLuaScene::CreateNode(const char *name) { return m_pScene->CreateNode(name)->ID(); }

bool atLuaScene::DeleteNode(const int64_t id, bool migrateChildren)
{
  return false;
}

bool atLuaScene::DeleteNodeFromID(const atLuaSceneNode & node, bool migrateChildren)
{
  return false;
}

bool atLuaScene::AddActiveCamera(const int64_t id)
{
  return false;
}

bool atLuaScene::RemoveActiveCamera(const int64_t id)
{
  return false;
}
