#include "atLuaScene.h"

atLuaScene::atLuaScene(atLuaScene &&move) { m_pScene = move.m_pScene; move.m_pScene = nullptr; }
atLuaScene::atLuaScene(const atLuaScene &copy) { m_pScene = copy.m_pScene; }
atLuaScene::atLuaScene(atScene *pScene) { m_pScene = pScene; }
atLuaScene::~atLuaScene() { m_pScene = nullptr; }

int64_t atLuaScene::CreateNodePRS(const char *name, const atVec3D &position, const atVec3D &rotation, const atVec3D &scale) { return m_pScene->CreateNode(name, position, rotation, scale)->ID(); }
int64_t atLuaScene::CreateNodePRSP(const char * name, const atVec3D & position, const atVec3D & rotation, const atVec3D & scale, const int64_t parentID) { return m_pScene->CreateNode(name, position, rotation, scale, m_pScene->GetNode(parentID))->ID(); }
int64_t atLuaScene::CreateNodePR(const char *name, const atVec3D &position, const atVec3D &rotation) { return m_pScene->CreateNode(name, position, rotation)->ID(); }
bool atLuaScene::DeleteNodeM(const atLuaSceneNode &node, bool migrateChildren) { return m_pScene->DeleteNode(node.m_pNode, migrateChildren); }
bool atLuaScene::DeleteNodeFromIDM(const int64_t id, bool migrateChildren) { return m_pScene->DeleteNode(id, migrateChildren); }
int64_t atLuaScene::CreateNodeP(const char *name, const atVec3D &position) { return m_pScene->CreateNode(name, position)->ID(); }
bool atLuaScene::RemoveActiveCamera(const atLuaSceneNode &node) { return m_pScene->RemoveActiveCamera(node.m_pNode); }
bool atLuaScene::IsActiveCameraFromID(const atLuaSceneNode &node) { return m_pScene->IsActiveCamera(node.m_pNode); }
void atLuaScene::SetViewport(const atVec4I &vp) { m_pScene->m_viewport = vp; }
void atLuaScene::SetViewportA(const int64_t x, const int64_t y, const int64_t w, const int64_t h) { m_pScene->m_viewport = { x, y, w, h }; }
bool atLuaScene::AddActiveCamera(const atLuaSceneNode &node) { return m_pScene->AddActiveCamera(node.m_pNode); }
atLuaSceneNode atLuaScene::GetNode(const int64_t id) const { return atLuaSceneNode(m_pScene->GetNode(id)); }
bool atLuaScene::RemoveActiveCameraFromID(const int64_t id) { return m_pScene->RemoveActiveCamera(id); }
bool atLuaScene::AddActiveCameraFromID(const int64_t id) { return m_pScene->AddActiveCamera(id); }
atLuaSceneNode atLuaScene::GetRootNode() const { return atLuaSceneNode(m_pScene->GetRoot()); }
int64_t atLuaScene::CreateNode(const char *name) { return m_pScene->CreateNode(name)->ID(); }
bool atLuaScene::DeleteNodeFromID(const int64_t id) { return DeleteNodeFromIDM(id, false); }
bool atLuaScene::DeleteNode(const atLuaSceneNode &node) { return DeleteNodeM(node, false); }
bool atLuaScene::IsActiveCamera(const int64_t id) { return m_pScene->IsActiveCamera(id); }
std::vector<int64_t> atLuaScene::GetNodeIDs() const { return m_pScene->GetNodeIDs(); }
int64_t atLuaScene::GetRootNodeID() const { return m_pScene->GetRootID(); }
const char* atLuaScene::GetName() const { return m_pScene->GetName(); }

const atLuaScene& atLuaScene::operator=(const atLuaScene & rhs) { m_pScene = rhs.m_pScene; return *this; }
const atLuaScene& atLuaScene::operator=(atLuaScene && rhs) { m_pScene = rhs.m_pScene; rhs.m_pScene = nullptr; return *this; }

void atLuaScene::SetName(const char * name) { m_pScene->SetName(name); }