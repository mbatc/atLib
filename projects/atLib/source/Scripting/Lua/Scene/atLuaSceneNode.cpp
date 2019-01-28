#include "atLuaSceneNode.h"

atLuaSceneNode::atLuaSceneNode(atSceneNode *pNode)
{
}

atLuaSceneNode::atLuaSceneNode(const atLuaSceneNode & node)
{
}

atLuaSceneNode::atLuaSceneNode(atLuaSceneNode && move)
{
}

int64_t atLuaSceneNode::GetID() const { return m_pNode->ID(); }

atVec3D atLuaSceneNode::GetScale() const { return m_pNode->m_scale; }
atVec3D atLuaSceneNode::GetPosition() const { return m_pNode->m_translation; }
atVec3D atLuaSceneNode::GetRotation() const { return m_pNode->m_rotation; }
const char* atLuaSceneNode::GetName() const { return m_pNode->Name(); }
int64_t atLuaSceneNode::ChildCount() const { return m_pNode->ChildCount(); }
int64_t atLuaSceneNode::SiblingCount() const { return m_pNode->SiblingCount(); }
atLuaSceneNode atLuaSceneNode::GetParent() const { return m_pNode->Parent(); }
atLuaSceneNode atLuaSceneNode::GetChild(const int64_t index) const { return m_pNode->Child(index); }
atLuaSceneNode atLuaSceneNode::GetSibling(const int64_t index) const { return m_pNode->Sibling(index); }
int64_t atLuaSceneNode::GetParentID() const { return m_pNode->ParentID(); }
int64_t atLuaSceneNode::GetChildID(const int64_t index) const { return m_pNode->ChildID(index); }
int64_t atLuaSceneNode::GetSiblingID(const int64_t index) const { return m_pNode->SiblingID(index); }
atVector<atLuaSceneNode> atLuaSceneNode::GetChildren() const { return atVector<atLuaSceneNode>(); }
atVector<atLuaSceneNode> atLuaSceneNode::GetSiblings() const { return atVector<atLuaSceneNode>(m_pNode->Siblings()); }
void atLuaSceneNode::SetName(const char * name) { m_pNode->SetName(name); }
void atLuaSceneNode::SetScale(const atVec3D &scl) { m_pNode->m_scale = scl; }
void atLuaSceneNode::SetPosition(const atVec3D &pos) { m_pNode->m_translation = pos; }
void atLuaSceneNode::SetRotaiton(const atVec3D &rot) { m_pNode->m_rotation = rot; }
bool atLuaSceneNode::AddChildP(const atLuaSceneNode &node, const bool preserveTransforms) { return m_pNode->AddChild(node.m_pNode, preserveTransforms); }
bool atLuaSceneNode::AddChildIDByP(const int64_t id, const bool preserveTransforms) { return AddChild(m_pNode->m_pScene->GetNode(id), preserveTransforms); }

bool atLuaSceneNode::AddChildP(const int64_t id, const bool preserveTransforms)
{
  return false;
}

bool atLuaSceneNode::AddChild(const atLuaSceneNode & node)
{
  return false;
}

bool atLuaSceneNode::AddChildIDBy(const int64_t id)
{
  return false;
}

bool atLuaSceneNode::AddChild(const int64_t id)
{
  return false;
}
