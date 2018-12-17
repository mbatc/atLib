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

  atSceneNode* CreateNode(const atVec3F64 &position = 0, const atVec3F64 &rotation = 0, const atVec3F64 &scale = 1, atSceneNode *pParent = nullptr);

  bool DeleteNode(const atSceneNode *pNode, bool migrateChildren = false);
  bool DeleteNode(const int64_t id, bool migrateChildren = false);

  bool AddActiveCamera(const int64_t id);
  bool RemoveActiveCamera(const int64_t id);

  bool AddActiveCamera(atSceneNode *pNode);
  bool RemoveActiveCamera(atSceneNode *pNode);

  const atVector<int64_t>& GetActiveCameras() const;

protected:
  bool Update(atSceneNode *pNode);
  
  double m_dt;

  atSceneNode *m_pRoot;
  atHashMap<int64_t, atSceneNode*> m_nodes;
  atHashMap<const atSceneNode*, int64_t> m_nodeIDs;

  atVector<int64_t> m_activeCameras;
};

#endif // atScene_h__
