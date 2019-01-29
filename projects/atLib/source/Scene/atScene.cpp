
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "atScene.h"
#include "atSceneRenderer.h"

static int64_t _idCounter = 0;
static atLua *_pLua = nullptr;

int64_t _GetNextID() { return _idCounter++; }

atScene::atScene() : m_pRoot(nullptr), m_pLua(nullptr), m_name("Scene") { m_pRoot = CreateNode("Root"); }
atScene::~atScene() { DeleteNode(m_pRoot, false, true); }

int64_t atScene::GetNodeID(const atSceneNode *pNode) const
{
  const int64_t *pID = m_nodeIDs.TryGet(pNode);
  return pID ? *pID : AT_INVALID_ID;
}

bool atScene::DeleteNode(atSceneNode *pNode, bool migrateChildren, bool allowRoot)
{
  if (!pNode || pNode->m_pScene != this || (pNode == m_pRoot && migrateChildren)) return false;
  atVector<atSceneNode*> children = pNode->Children();
  bool res = true;
  for (atSceneNode *pNode : children)
    if (migrateChildren)
      pNode->Parent()->AddChild(pNode);
    else
      res &= DeleteNode(pNode, migrateChildren);

  if (pNode->Parent())
    pNode->Parent()->RemoveChild(pNode);

  if (!allowRoot && pNode == m_pRoot)
    return true;

  int64_t id = m_nodeIDs[pNode];
  m_nodeIDs.Remove(pNode);
  m_nodes.Remove(id);
  atDelete(pNode);
  return res;
}

bool atScene::Update(atSceneNode *pNode)
{
  bool res = pNode->Update(m_dt);
  for (atSceneNode *pChild : pNode->Children())
    res &= Update(pChild);
  return res;
}

atVector<int64_t> atScene::GetNodeIDs(const atVector<atSceneNode*> nodes) const
{
  atVector<int64_t> ret;
  for (atSceneNode *pNode : nodes)
    ret.push_back(GetNodeID(pNode));
  return ret;
}

atSceneNode* atScene::CreateNode(const atString &name, const atVec3D &position, const atVec3D &rotation, const atVec3D &scale, atSceneNode *pParent)
{
  atSceneNode *pNode = atNew<atSceneNode>();
  pNode->m_name = name;
  pNode->m_translation = position;
  pNode->m_rotation = rotation;
  pNode->m_scale = scale;
  pNode->m_pScene = this;
  if (pParent) pParent->AddChild(pNode);
  else if(m_pRoot) m_pRoot->AddChild(pNode);
  int64_t id = _GetNextID();
  m_nodes.Add(id, pNode);
  m_nodeIDs.Add(pNode, id);
  return pNode;
}

bool atScene::DeleteNode(atSceneNode *pNode, bool migrateChildren)
{
  return DeleteNode(pNode, migrateChildren, false);
}

bool atScene::AddActiveCamera(const int64_t id)
{
  for (const int64_t camera : m_activeCameras)
    if (camera == id)
      return false;
  m_activeCameras.push_back(id);
  return true;
}

bool atScene::RemoveActiveCamera(const int64_t id)
{
  for(const int64_t &camera : m_activeCameras)
    if (id == camera)
    {
      m_activeCameras.erase(&id - m_activeCameras.begin());
      return true;
    }
  return false;
}

bool atScene::Draw(atSceneNode *pCamera)
{
  if (!pCamera)
    return false;
  return Draw(pCamera->ID());
}

bool atScene::Draw(const int64_t camera)
{
  atVector<int64_t> lastCamList = m_activeCameras;
  m_activeCameras.clear();
  m_activeCameras.push_back(camera);
  bool res = Draw();
  m_activeCameras = lastCamList;
  return res;
}

const atSceneNode* atScene::GetNode(const int64_t id) const { atSceneNode * const *ppNode = m_nodes.TryGet(id); return ppNode ? *ppNode : nullptr; }
bool atScene::RemoveActiveCamera(atSceneNode *pNode) { return pNode->m_pScene == this ? RemoveActiveCamera(GetNodeID(pNode)) : false; }
const atString& atScene::GetName() { return m_name; }
void atScene::SetName(const atString &name) { m_name = name; }
atSceneNode* atScene::GetNode(const int64_t id) { atSceneNode **ppNode = m_nodes.TryGet(id); return ppNode ? *ppNode : nullptr; }
bool atScene::AddActiveCamera(atSceneNode *pNode) { return pNode->m_pScene == this ? AddActiveCamera(GetNodeID(pNode)) : false; }
bool atScene::DeleteNode(const int64_t id, bool migrateChildren) { return DeleteNode(GetNode(id), migrateChildren); }
atVector<int64_t> atScene::GetNodeIDs() const  { return m_nodes.GetKeys(); }
const atVector<int64_t>& atScene::GetActiveCameras() const { return m_activeCameras; }
bool atScene::Draw() { return atSceneRenderer::Render(this); }
int64_t atScene::GetRootID() { return m_pRoot->ID(); }
void atScene::SetLua(atLua *pLua) { m_pLua = pLua; }
atSceneNode* atScene::GetRoot() { return m_pRoot; }
bool atScene::Update() { return Update(m_pRoot); }
atLua* atScene::GetLua() { return m_pLua; }
