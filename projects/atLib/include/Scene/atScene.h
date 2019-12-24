#ifndef atScene_h__
#define atScene_h__

#include "atSceneNode.h"
#include "atHierarchy.h"

// Core components
#include "atCameraComponent.h"
#include "atTransformComponent.h"
#include "atMeshComponent.h"
#include "atScriptComponent.h"
#include "atControlComponent.h"

class atScene
{
public:
  atScene() = default;
  atScene(atScene &&o);
  atScene(const atScene &o);
  
  atScene& operator=(atScene &&o);
  atScene& operator=(const atScene &o);

  virtual void Init();
  virtual void Update();
  virtual void Render(const atMat4D &vp);
  virtual void Destroy();

  int64_t Add(atSceneNode &&o, const int64_t &parentID = atHierarchy_atRootNodeID);
  int64_t Add(const atSceneNode &o = atSceneNode(), const int64_t &parentID = atHierarchy_atRootNodeID);

  void Remove(const int64_t &nodeID);

  void Move(const int64_t &nodeID, const int64_t &toNodeID);
  void MoveChildren(const int64_t &nodeID, const int64_t &toNodeID);

  int64_t Insert(atScene *pScene, const int64_t &parentID = atHierarchy_atRootNodeID, const bool &move = false);

  atScene Extract(const int64_t &parentID = atHierarchy_atRootNodeID, const bool &move = false);
  
  int64_t GetNodeID(const atSceneNode *pNode) const;

  atSceneNode* Get(const int64_t &nodeID);
  const atSceneNode* Get(const int64_t &nodeID) const;

  atSceneNode* GetParent(const int64_t &nodeID);
  const atSceneNode* GetParent(const int64_t &nodeID) const;

  atSceneNode* GetChild(const int64_t &nodeID, const int64_t &index);
  const atSceneNode* GetChild(const int64_t &nodeID, const int64_t &index) const;
  
  int64_t ChildCount(const int64_t &nodeID) const;
  atVector<int64_t> Children(const int64_t &nodeID) const;

  // Retrieve some nodes by name
  atSceneNode* GetNode(const atString &name);
  atVector<atSceneNode*> GetNodes(const atString &name);
  
protected:
  int64_t m_lastClock = 0;
  atHierarchy<atSceneNode> m_hierarchy;
};

#endif // atScene_h__
