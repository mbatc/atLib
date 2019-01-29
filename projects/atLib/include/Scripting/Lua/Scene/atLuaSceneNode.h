#ifndef atLuaSceneNode_h__
#define atLuaSceneNode_h__

#include "atSceneNode.h"
#include "atLuaSceneComponent.h"

// Functions are named to reflect there parameters as this
// makes it easier to implement overloaded functions in lua
// as the compiler struggles to resolve overloaded member functions
// when taking a pointer to them

class atLuaScene;

class atLuaSceneNode
{
  friend atLuaScene;

public:
  atLuaSceneNode(atSceneNode *pNode = nullptr);
  atLuaSceneNode(const atLuaSceneNode &node);
  atLuaSceneNode(atLuaSceneNode &&move);

  int64_t GetID() const;
  atVec3D GetScale() const;
  atVec3D GetPosition() const;
  atVec3D GetRotation() const;

  atVec3D GetGlobalScale() const;
  atVec3D GetGlobalPosition() const;
  atVec3D GetGlobalRotation() const;
  
  const char* GetName() const;

  int64_t ChildCount() const;
  int64_t SiblingCount() const;

  atLuaSceneNode GetParent() const;
  atLuaSceneNode GetChild(const int64_t index) const;
  atLuaSceneNode GetSibling(const int64_t index) const;

  int64_t GetParentID() const;
  int64_t GetChildID(const int64_t index) const;
  int64_t GetSiblingID(const int64_t index) const;

  std::vector<atLuaSceneNode> GetChildren() const;
  std::vector<atLuaSceneNode> GetSiblings() const;

  void SetName(const char *name);
  void SetScale(const atVec3D &scl);
  void SetPosition(const atVec3D &pos);
  void SetRotation(const atVec3D &rot);

  bool AddChildP(const atLuaSceneNode &node, const bool preserveTransforms);
  bool AddChildByIDP(const int64_t id, const bool preserveTransforms);
  
  bool AddChild(const atLuaSceneNode &node);
  bool AddChildByID(const int64_t id);

  bool RemoveChild(const atLuaSceneNode &node);
  bool RemoveChildP(const atLuaSceneNode &node, bool preserveTransforms);
  bool RemoveChildByID(const int64_t id);
  bool RemoveChildByIDP(const int64_t id, bool preserveTransforms);

  std::vector<atLuaSceneComponent> GetComponents();
  std::vector<atLuaSceneComponent> GetComponentsOyType(const int64_t type);
  int64_t ComponentCount();
  int64_t ComponentCountOfType(const int64_t type);
  atLuaSceneComponent GetComponent(const int64_t index);
  atLuaSceneComponent GetComponentOfType(const int64_t type, const int64_t index);

  const atLuaSceneNode& operator=(const atLuaSceneNode &rhs);
  const atLuaSceneNode& operator=(atLuaSceneNode &&rhs);

protected:
  atSceneNode *m_pNode = nullptr;
};

#endif // atLuaSceneNode_h__
