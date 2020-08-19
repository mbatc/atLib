#ifndef atScene_h__
#define atScene_h__

#include "atString.h"
#include "atHierarchy.h"
#include "atSceneNode.h"

static const int64_t atScene_RootNodeID = atHierarchy_RootNodeID;
static const int64_t atScene_InvalidNodeID = atHierarchy_InvalidNodeID;

class atScene
{
  friend atSceneNode;
public:
  atScene(const atString &name = "scene");

  // Get the root node of the scene
  atSceneNode* GetRoot() const;

  // Get a node in the scene
  atSceneNode* GetNode(const int64_t &nodeID) const;

  // Add a node to the scene, optionally providing a name
  atSceneNode* AddNode(const atString &name = "", const int64_t &parentID = atScene_RootNodeID);
  
  // Remove a node from the scene
  bool RemoveNode(const int64_t &nodeID);

  // Find a node in the scene by name
  atSceneNode* FindByTag(const atString &tag, const int64_t &root = atScene_RootNodeID) const;

  // Find a node in the scene by tag
  atSceneNode* FindByName(const atString &name, const int64_t &root = atScene_RootNodeID) const;

  bool Init(const int64_t &root = atScene_RootNodeID);
  bool Update(const int64_t &root = atScene_RootNodeID);
  bool Destroy(const int64_t &root = atScene_RootNodeID);

  atString name;

protected:
  atHierarchy<atSceneNode*> m_hierarchy;
};

#include "atScene.inl"
#endif // atScene_h__
