#include "atLuaSceneNode.h"
#include "atScene.h"

atLuaSceneNode::atLuaSceneNode(atSceneNode *pNode) { m_pNode = pNode; }
atLuaSceneNode::atLuaSceneNode(const atLuaSceneNode &node) { m_pNode = node.m_pNode; }
atLuaSceneNode::atLuaSceneNode(atLuaSceneNode &&move) { m_pNode = move.m_pNode; move.m_pNode = nullptr; }

atLuaSceneNode atLuaSceneNode::GetChild(const int64_t index) const { return m_pNode->Child(index); }
bool atLuaSceneNode::AddChildP(const atLuaSceneNode &node, const bool preserveTransforms) { return m_pNode ? m_pNode->AddChild(node.m_pNode, preserveTransforms) : false; }
bool atLuaSceneNode::AddChildByIDP(const int64_t id, const bool preserveTransforms) { return m_pNode ? AddChildP(m_pNode->Scene()->GetNode(id), preserveTransforms) : false; }
bool atLuaSceneNode::RemoveChildP(const atLuaSceneNode &node, bool preserveTransforms) { return m_pNode ? m_pNode->RemoveChild(node.m_pNode, preserveTransforms) : false; }
atLuaSceneComponent atLuaSceneNode::GetComponentOfType(const int64_t type, const int64_t index) { return m_pNode ? m_pNode->Component(type, index) : atLuaSceneComponent(); }
const atLuaSceneNode& atLuaSceneNode::operator=(const atLuaSceneNode & rhs) { m_pNode = rhs.m_pNode; return *this; }
const atLuaSceneNode& atLuaSceneNode::operator=(atLuaSceneNode && rhs) { m_pNode = rhs.m_pNode; rhs.m_pNode = nullptr; return *this; }
bool atLuaSceneNode::RemoveChildByIDP(const int64_t id, bool preserveTransforms) { return m_pNode ? m_pNode->RemoveChild(id, preserveTransforms) : false; }
std::vector<atLuaSceneComponent> atLuaSceneNode::GetComponentsOyType(const int64_t type) { return m_pNode ? m_pNode->Components(type) : std::vector<atLuaSceneComponent>(); }
std::vector<atLuaSceneNode> atLuaSceneNode::GetSiblings() const { return m_pNode ? atVector<atSceneNode*>(m_pNode->Siblings()) : std::vector<atLuaSceneNode>(); }
std::vector<atLuaSceneNode> atLuaSceneNode::GetChildren() const { return m_pNode ? atVector<atSceneNode*>(m_pNode->Children()) : std::vector<atLuaSceneNode>(); }
bool atLuaSceneNode::RemoveChild(const atLuaSceneNode &node) { return m_pNode ? m_pNode->RemoveChild(node.m_pNode) : false; }
atLuaSceneComponent atLuaSceneNode::GetComponent(const int64_t index) { return m_pNode ? m_pNode->Component(index) : atLuaSceneComponent(); }
int64_t atLuaSceneNode::ComponentCountOfType(const int64_t type) { return m_pNode ? m_pNode->ComponentCount(type) : 0; }
atLuaSceneNode atLuaSceneNode::GetSibling(const int64_t index) const { return  m_pNode ? m_pNode->Sibling(index) : atLuaSceneNode(); }
int64_t atLuaSceneNode::GetSiblingID(const int64_t index) const { return  m_pNode ? m_pNode->SiblingID(index) : AT_INVALID_ID; }
std::vector<atLuaSceneComponent> atLuaSceneNode::GetComponents() { return m_pNode ? m_pNode->Components() : std::vector<atLuaSceneComponent>(); }
int64_t atLuaSceneNode::GetChildID(const int64_t index) const { return m_pNode ? m_pNode->ChildID(index) : AT_INVALID_ID; }
bool atLuaSceneNode::AddChild(const atLuaSceneNode &node) { return m_pNode ? AddChildP(node, true) : false; }
bool atLuaSceneNode::RemoveChildByID(const int64_t id) { return m_pNode ? m_pNode->RemoveChild(id) : false; }
bool atLuaSceneNode::AddChildByID(const int64_t id) { return m_pNode ? AddChildByIDP(id, true) : false; }
atVec3D atLuaSceneNode::GetGlobalPosition() const { return m_pNode ? m_pNode->GlobalPosition() : 0; }
atVec3D atLuaSceneNode::GetGlobalRotation() const { return m_pNode ? m_pNode->GlobalRotation() : 0; }
void atLuaSceneNode::SetPosition(const atVec3D &pos) { if(m_pNode) m_pNode->m_translation = pos; }
void atLuaSceneNode::SetRotation(const atVec3D &rot) { if(m_pNode) m_pNode->m_rotation = rot; }
atVec3D atLuaSceneNode::GetGlobalScale() const { return m_pNode ? m_pNode->GlobalScale() : 0; }
int64_t atLuaSceneNode::SiblingCount() const { return m_pNode ? m_pNode->SiblingCount() : 0; }
atVec3D atLuaSceneNode::GetPosition() const { return m_pNode ? m_pNode->m_translation : atVec3D::zero(); }
atLuaSceneNode atLuaSceneNode::GetParent() const { return m_pNode->Parent(); }
int64_t atLuaSceneNode::ComponentCount() { return m_pNode ? m_pNode->ComponentCount() : 0; }
void atLuaSceneNode::SetScale(const atVec3D &scl) { if (m_pNode) m_pNode->m_scale = scl; }
int64_t atLuaSceneNode::ChildCount() const { return m_pNode ? m_pNode->ChildCount() : 0; }
void atLuaSceneNode::SetName(const char *name) { if(m_pNode) m_pNode->SetName(name); }
int64_t atLuaSceneNode::GetParentID() const { return m_pNode ? m_pNode->ParentID() : AT_INVALID_ID; }
atVec3D atLuaSceneNode::GetRotation() const { return m_pNode ? m_pNode->m_rotation : atVec3D::zero(); }
const char* atLuaSceneNode::GetName() const { return m_pNode ? m_pNode->Name().c_str() : "Invalid Node"; }
atVec3D atLuaSceneNode::GetScale() const { return  m_pNode ? m_pNode->m_scale : atVec3D::zero(); }
int64_t atLuaSceneNode::GetID() const { return  m_pNode ? m_pNode->ID() : -1; }
