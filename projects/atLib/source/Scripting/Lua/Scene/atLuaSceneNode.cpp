#include "atLuaSceneNode.h"
#include "atScene.h"

atLuaSceneNode::atLuaSceneNode(atSceneNode *pNode) { m_pNode = pNode; }
atLuaSceneNode::atLuaSceneNode(const atLuaSceneNode &node) { m_pNode = node.m_pNode; }
atLuaSceneNode::atLuaSceneNode(atLuaSceneNode &&move) { m_pNode = move.m_pNode; move.m_pNode = nullptr; }

atLuaSceneNode atLuaSceneNode::GetChild(const int64_t index) const { return m_pNode->Child(index); }
bool atLuaSceneNode::AddChildP(const atLuaSceneNode &node, const bool preserveTransforms) { return m_pNode->AddChild(node.m_pNode, preserveTransforms); }
bool atLuaSceneNode::AddChildByIDP(const int64_t id, const bool preserveTransforms) { return AddChildP(m_pNode->Scene()->GetNode(id), preserveTransforms); }
bool atLuaSceneNode::RemoveChildP(const atLuaSceneNode &node, bool preserveTransforms) { return m_pNode->RemoveChild(node.m_pNode, preserveTransforms); }
atLuaSceneComponent atLuaSceneNode::GetComponentOfType(const int64_t type, const int64_t index) { return m_pNode->Component(type, index); }
bool atLuaSceneNode::RemoveChildByIDP(const int64_t id, bool preserveTransforms) { return m_pNode->RemoveChild(id, preserveTransforms); }
atVector<atLuaSceneComponent> atLuaSceneNode::GetComponentsOyType(const int64_t type) { return m_pNode->Components(type); }
atVector<atLuaSceneNode> atLuaSceneNode::GetSiblings() const { return atVector<atLuaSceneNode>(m_pNode->Siblings()); }
bool atLuaSceneNode::RemoveChild(const atLuaSceneNode &node) { return m_pNode->RemoveChild(node.m_pNode); }
atLuaSceneComponent atLuaSceneNode::GetComponent(const int64_t index) { return m_pNode->Component(index); }
int64_t atLuaSceneNode::ComponentCountOfType(const int64_t type) { return m_pNode->ComponentCount(type); }
atLuaSceneNode atLuaSceneNode::GetSibling(const int64_t index) const { return m_pNode->Sibling(index); }
int64_t atLuaSceneNode::GetSiblingID(const int64_t index) const { return m_pNode->SiblingID(index); }
atVector<atLuaSceneNode> atLuaSceneNode::GetChildren() const { return atVector<atLuaSceneNode>(); }
int64_t atLuaSceneNode::GetChildID(const int64_t index) const { return m_pNode->ChildID(index); }
atVector<atLuaSceneComponent> atLuaSceneNode::GetComponents() { return m_pNode->Components(); }
bool atLuaSceneNode::AddChild(const atLuaSceneNode &node) { return AddChildP(node, true); }
bool atLuaSceneNode::RemoveChildByID(const int64_t id) { return m_pNode->RemoveChild(id); }
bool atLuaSceneNode::AddChildByID(const int64_t id) { return AddChildByIDP(id, true); }
atVec3D atLuaSceneNode::GetGlobalPosition() const { return m_pNode->GlobalPosition(); }
atVec3D atLuaSceneNode::GetGlobalRotation() const { return m_pNode->GlobalRotation(); }
void atLuaSceneNode::SetPosition(const atVec3D &pos) { m_pNode->m_translation = pos; }
void atLuaSceneNode::SetRotation(const atVec3D &rot) { m_pNode->m_rotation = rot; }
atVec3D atLuaSceneNode::GetGlobalScale() const { return m_pNode->GlobalScale(); }
int64_t atLuaSceneNode::SiblingCount() const { return m_pNode->SiblingCount(); }
atVec3D atLuaSceneNode::GetPosition() const { return m_pNode->m_translation; }
atLuaSceneNode atLuaSceneNode::GetParent() const { return m_pNode->Parent(); }
int64_t atLuaSceneNode::ComponentCount() { return m_pNode->ComponentCount(); }
void atLuaSceneNode::SetScale(const atVec3D &scl) { m_pNode->m_scale = scl; }
int64_t atLuaSceneNode::ChildCount() const { return m_pNode->ChildCount(); }
void atLuaSceneNode::SetName(const char * name) { m_pNode->SetName(name); }
int64_t atLuaSceneNode::GetParentID() const { return m_pNode->ParentID(); }
atVec3D atLuaSceneNode::GetRotation() const { return m_pNode->m_rotation; }
const char* atLuaSceneNode::GetName() const { return m_pNode->Name(); }
atVec3D atLuaSceneNode::GetScale() const { return m_pNode->m_scale; }
int64_t atLuaSceneNode::GetID() const { return m_pNode->ID(); }
