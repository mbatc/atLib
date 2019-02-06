
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

#ifndef atScene_h__
#define atScene_h__

#include "atLua.h"
#include "atHashMap.h"
#include "atSceneNode.h"

class atScene
{
public:
  atScene();
  ~atScene();

  const atString& GetName();
  void SetName(const atString &name);

  bool Update();
  bool Draw();
  bool Draw(atSceneNode *pCamera);
  bool Draw(const int64_t camera);

  atLua* GetLua();
  void SetLua(atLua *pLua);

  int64_t GetRootID();
  atSceneNode* GetRoot();
  atSceneNode* GetNode(const int64_t id);
  const atSceneNode* GetNode(const int64_t id) const;

  int64_t GetNodeID(const atSceneNode *pNode) const;
  atVector<int64_t> GetNodeIDs() const;
  atVector<int64_t> GetNodeIDs(const atVector<atSceneNode*> nodes) const;

  atSceneNode* CreateNode(const atString &name, const atVec3D &position = 0, const atVec3D &rotation = 0, const atVec3D &scale = atVec3D::one(), atSceneNode *pParent = nullptr);

  bool DeleteNode(atSceneNode *pNode, bool migrateChildren = false);
  bool DeleteNode(const int64_t id, bool migrateChildren = false);

  bool IsActiveCamera(const int64_t id);
  bool AddActiveCamera(const int64_t id);
  bool RemoveActiveCamera(const int64_t id);

  bool IsActiveCamera(atSceneNode *pNode);
  bool AddActiveCamera(atSceneNode *pNode);
  bool RemoveActiveCamera(atSceneNode *pNode);

  const atVector<int64_t>& GetActiveCameras() const;

  atVec4I m_viewport;
protected:
  bool DeleteNode(atSceneNode *pNode, bool migrateChildren, bool allowRoot);
  bool Update(atSceneNode *pNode);
  
  atString m_name;

  double m_dt;

  atLua *m_pLua;
  atSceneNode *m_pRoot;
  atHashMap<int64_t, atSceneNode*> m_nodes;
  atHashMap<const atSceneNode*, int64_t> m_nodeIDs;

  atVector<int64_t> m_activeCameras;
};

#endif // atScene_h__
