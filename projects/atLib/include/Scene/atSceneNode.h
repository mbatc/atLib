#ifndef atSceneNode_h__
#define atSceneNode_h__

#include "atSceneComponent.h"
#include "atSceneComponentFactory.h"
#include "atTransformable.h"
#include "atHashSet.h"
#include "atString.h"

class atScene;

class atSceneNode : public atTransformable<double>
{
  friend atScene;

  atSceneNode(const int64_t &id, const atString &name);

public:
  // Get a pointer to the scene that this node belongs to.
  atScene* GetScene() const;
  
  // Get a pointer to this nodes parent.
  // Returns nullptr if this is the root node
  atSceneNode* GetParent() const;

  // Set this nodes parent
  bool SetParent(const int64_t &parentID);
  bool SetParent(atSceneNode *pNode);

  // Add a child relation to this node
  bool AddChild(const int64_t &childID);
  bool AddChild(atSceneNode *pNode);

  // Remove a child relation from this node
  bool RemoveChild(const int64_t &childID, const int64_t &moveTo = -1 /*atScene_RootNodeID*/);
  bool RemoveChild(atSceneNode *pNode, atSceneNode *pMoveTo = nullptr);

  // Get a component that is attached to this node by index.
  // Returns nullptr if index is out of range.
  atSceneComponent* GetComponent(const int64_t &index) const;

  // Get a component that is attached to this node by type.
  // Returns nullptr if no such component exists.
  atSceneComponent* GetComponent(const atString &type) const;
  template<typename T> T* GetComponent() const;

  int64_t GetComponentCount() const;

  // Find the index a component of the specified type
  int64_t FindComponent(const atString &type) const;
  template<typename T> int64_t FindComponent() const;

  // Add a component of the specified type to this node.
  // Returns a pointer to the created component.
  // Returns nullptr if the component could not be added.
  // Only 1 component of each type may be attached to a node.
  atSceneComponent* AddComponent(const atString &type);
  template<typename T> T* AddComponent();

  // Check if the node has a component of the specified attached to it.
  bool HasComponent(const atString &type) const;
  template<typename T> bool HasComponent() const;

  // Remove a component from the node by index
  bool RemoveComponent(const int64_t &index);

  // Remove a component from the node by type
  bool RemoveComponent(const atString &type);
  template<typename T> bool RemoveComponent();

  // Returns true if this is the root node of a scene
  bool IsRoot() const;

  // Get the global transform of the node.
  // If reEvaluate == false, this will be the global transform calculated
  // in the last update step.
  atTransformable<double> GlobalTransform(const bool &reEvaluate = false);

  bool Init();
  bool Update();
  bool Render();
  bool Destroy();

  // Node Properties
  const int64_t       id;
  atString            name;
  atHashSet<atString> tags;

protected:
  atSceneComponent* AddComponent(atSceneComponent *pComponent);

  atScene *m_pScene;
  atVector<atSceneComponent*> m_components;
  atTransformable<double> m_globalTransform;
};

#include "atSceneNode.inl"

#endif // atSceneNode_h__

