#include "atSceneNode.h"
#include "atScene.h"

atSceneNode::atSceneNode() {}

bool atSceneNode::Update(const double dt)
{
  bool res = true;
  for (atSceneComponent *pComp : m_components)
    res &= pComp->Update(dt);
  return res;
}

bool atSceneNode::Draw(const atMat4D &vp)
{
  bool res = true;
  for (atSceneComponent *pComp : m_components)
    res &= pComp->Draw(vp);
  return res;
}

bool atSceneNode::AddChild(atSceneNode *pChild, const bool preserveTransform)
{
  if (!pChild || pChild->Parent() == this) return false;
  if (pChild->Parent() != nullptr) pChild->Parent()->RemoveChild(pChild, preserveTransform);

  if (preserveTransform)
  {
    pChild->m_rotation -= GlobalRotation();
    pChild->m_scale /= GlobalScale();
    pChild->m_translation -= GlobalWorldMat().Inverse() * atVec3F64::zero();
  }

  // Update relationships
  pChild->m_siblings = m_children;
  pChild->m_pParent = this;
  m_children.push_back(pChild);
  return true;
}

bool atSceneNode::RemoveChild(atSceneNode *pChild, const bool preserveTransform)
{
  if (!pChild || pChild->Parent() != this) return false;
  for (const atSceneNode * const &pNode : m_children)
    if (pNode == pChild)
    {
      m_children.erase(&pNode - m_children.begin());
      break;
    }

  if (preserveTransform)
  {
    pChild->m_rotation += GlobalRotation();
    pChild->m_scale *= GlobalScale();
    pChild->m_translation = GlobalWorldMat() * pChild->m_translation;
  }

  pChild->m_pParent = nullptr;
  pChild->m_siblings.clear();
  return true;
}

int64_t atSceneNode::ComponentCount(const int64_t type) const
{
  int64_t count = 0;
  for (atSceneComponent *pComp : m_components)
    count += (pComp->TypeID() & type) > 0;
  return count;
}

atSceneComponent* atSceneNode::Component(const int64_t type, int64_t index) const
{
  int64_t count = -1;
  for (atSceneComponent *pComp : m_components)
  {
    count += (pComp->TypeID() & type) > 0;
    if (count == index)
      return pComp;
  }
  return nullptr;
}

atVector<atSceneComponent*> atSceneNode::Components(const int64_t type) const
{
  atVector<atSceneComponent*> ret;
  for (atSceneComponent *pComp : m_components)
    ret.push_back(pComp);
  return ret;
}

int64_t atSceneNode::ID() const { return m_pScene->GetNodeID(this); }

atSceneNode* atSceneNode::Root() const { return m_pScene->GetRoot(); }

bool atSceneNode::RemoveChild(const int64_t id) { return RemoveChild(m_pScene->GetNode(id)); }
bool atSceneNode::AddChild(const int64_t id) { return AddChild(m_pScene->GetNode(id)); }

int64_t atSceneNode::SiblingID(const int64_t index) const { return Sibling(index)->ID(); }
int64_t atSceneNode::ChildID(const int64_t index) const { return Child(index)->ID(); }
int64_t atSceneNode::ParentID(const int64_t index)  const { return m_pParent->ID(); }

atVector<int64_t> atSceneNode::SiblingIDs() const { return m_pScene->GetNodeIDs(m_siblings); }
atVector<int64_t> atSceneNode::ChildIDs() const { return m_pScene->GetNodeIDs(m_children); }

atSceneNode* atSceneNode::Sibling(const int64_t index) const { return m_siblings[index]; }
atSceneNode* atSceneNode::Child(const int64_t index) const { return m_children[index]; }
atSceneNode* atSceneNode::Parent() const { return m_pParent; }

const atVector<atSceneNode*>& atSceneNode::Children() const { return m_children; }
const atVector<atSceneNode*>& atSceneNode::Siblings() const { return m_siblings; }

// Transformations and Matrices

atMat4D atSceneNode::GlobalTranslationMat() const { return TranslationMat() * ParentTranslationMat(); }
atMat4D atSceneNode::GlobalRotationMat() const { return RotationMat() * ParentRotationMat(); }
atMat4D atSceneNode::GlobalScaleMat() const { return ScaleMat() * ParentScaleMat(); }
atMat4D atSceneNode::GlobalWorldMat() const { return WorldMat() * ParentWorldMat(); }

atVec3F64 atSceneNode::GlobalPosition() const { return ParentWorldMat() * m_translation; }
atVec3F64 atSceneNode::GlobalRotation() const { return ParentRotation() + m_rotation; }
atVec3F64 atSceneNode::GlobalScale() const { return ParentScale() * m_scale; }

atVec3F64 atSceneNode::ParentPosition() const { return Parent() ? Parent()->GlobalPosition() : 0; }
atVec3F64 atSceneNode::ParentRotation() const { return Parent() ? Parent()->GlobalRotation() : 0; }
atVec3F64 atSceneNode::ParentScale() const { return Parent() ? Parent()->GlobalScale() : 1; }

atMat4D atSceneNode::ParentTranslationMat() const { return Parent() ? Parent()->GlobalTranslationMat() : atMat4D::Identity(); }
atMat4D atSceneNode::ParentRotationMat() const { return Parent() ? Parent()->GlobalRotationMat() : atMat4D::Identity(); }
atMat4D atSceneNode::ParentScaleMat() const { return Parent() ? Parent()->GlobalScaleMat() : atMat4D::Identity(); }
atMat4D atSceneNode::ParentWorldMat() const { return Parent() ? Parent()->GlobalWorldMat() : atMat4D::Identity(); }
