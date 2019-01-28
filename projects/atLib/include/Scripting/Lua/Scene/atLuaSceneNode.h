#ifndef atLuaSceneNode_h__
#define atLuaSceneNode_h__

#include "atSceneNode.h"

// Functions are named to reflect there parameters as this
// makes it easier to implement overloaded functions in lua
// as the compiler struggles to resolve overloaded member functions
// when taking a pointer to them

class atLuaSceneNode
{
public:
  atLuaSceneNode(atSceneNode *pNode);
  atLuaSceneNode(const atLuaSceneNode &node);
  atLuaSceneNode(atLuaSceneNode &&move);

  int64_t GetID() const;
  atVec3D GetScale() const;
  atVec3D GetPosition() const;
  atVec3D GetRotation() const;
  const char* GetName() const;

  int64_t ChildCount() const;
  int64_t SiblingCount() const;

  atLuaSceneNode GetParent() const;
  atLuaSceneNode GetChild(const int64_t index) const;
  atLuaSceneNode GetSibling(const int64_t index) const;

  int64_t GetParentID() const;
  int64_t GetChildID(const int64_t index) const;
  int64_t GetSiblingID(const int64_t index) const;

  atVector<atLuaSceneNode> GetChildren() const;
  atVector<atLuaSceneNode> GetSiblings() const;

  void SetName(const char *name);
  void SetScale(const atVec3D &scl);
  void SetPosition(const atVec3D &pos);
  void SetRotaiton(const atVec3D &rot);

  bool AddChildP(const atLuaSceneNode &node, const bool preserveTransforms);
  bool AddChildIDByP(const int64_t id, const bool preserveTransforms);
  bool AddChildP(const int64_t id, const bool preserveTransforms);

  bool AddChild(const atLuaSceneNode &node);
  bool AddChildIDBy(const int64_t id);
  bool AddChild(const int64_t id);

protected:
  atSceneNode *m_pNode;
};

#endif // atLuaSceneNode_h__
