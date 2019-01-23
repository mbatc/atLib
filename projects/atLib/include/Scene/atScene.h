
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

#include "atHashMap.h"
#include "atSceneNode.h"

class atScene
{
public:
  atScene();
  ~atScene();

  bool Update();
  bool Draw();

  atSceneNode* GetRoot();
  atSceneNode* GetNode(const int64_t id);
  const atSceneNode* GetNode(const int64_t id) const;

  int64_t GetNodeID(const atSceneNode *pNode) const;
  atVector<int64_t> GetNodeIDs() const;
  atVector<int64_t> GetNodeIDs(const atVector<atSceneNode*> nodes) const;

  atSceneNode* CreateNode(const atVec3D &position = 0, const atVec3D &rotation = 0, const atVec3D &scale = atVec3D::one(), atSceneNode *pParent = nullptr);

  bool DeleteNode(const atSceneNode *pNode, bool migrateChildren = false);
  bool DeleteNode(const int64_t id, bool migrateChildren = false);

  bool AddActiveCamera(const int64_t id);
  bool RemoveActiveCamera(const int64_t id);

  bool AddActiveCamera(atSceneNode *pNode);
  bool RemoveActiveCamera(atSceneNode *pNode);

  const atVector<int64_t>& GetActiveCameras() const;

  atVec4I m_viewport;
protected:
  bool Update(atSceneNode *pNode);
  
  double m_dt;

  atSceneNode *m_pRoot;
  atHashMap<int64_t, atSceneNode*> m_nodes;
  atHashMap<const atSceneNode*, int64_t> m_nodeIDs;

  atVector<int64_t> m_activeCameras;
};

#endif // atScene_h__
