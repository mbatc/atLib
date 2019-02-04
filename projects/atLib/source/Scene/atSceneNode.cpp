
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "atSceneNode.h"
#include "atScene.h"

atSceneNode::atSceneNode() {}

const atString &atSceneNode::Name() { return m_name; }
void atSceneNode::SetName(const atString &name) { m_name = name; }

bool atSceneNode::Update(const double dt)
{
  bool res = true;
  for (atSceneComponent *pComp : m_components)
    res &= pComp->OnUpdate(dt);
  return res;
}

bool atSceneNode::Draw(const atMat4D &vp)
{
  bool res = true;
  for (atSceneComponent *pComp : m_components)
    res &= pComp->OnDraw(vp);
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
    pChild->m_translation -= GlobalWorldMat().Inverse() * atVec3D::zero();
  }

  // Update relationships
  pChild->m_siblings = m_children;
  for (atSceneNode *pNode : m_children)
    pNode->m_siblings.push_back(pChild);
  pChild->m_pParent = this;
  m_children.push_back(pChild);
  return true;
}

bool atSceneNode::RemoveChild(atSceneNode *pChild, const bool preserveTransform)
{
  if (!pChild || pChild->Parent() != this) return false;

  // Remove from all sibling lists
  for (atSceneNode *&pNode : m_children)
    for (atSceneNode *&pSibling : pNode->m_siblings)
      if (pSibling == pChild)
      {
        pNode->m_siblings.erase(&pSibling - pNode->m_siblings.begin());
        break;
      }

  // Remove from child list
  for (atSceneNode *&pNode : m_children)
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

atVector<atSceneComponent*> atSceneNode::Components(const int64_t type) const
{
  atVector<atSceneComponent*> ret;
  for (atSceneComponent *pComp : m_components)
    if((pComp->TypeID() & type) > 0)
      ret.push_back(pComp);
  return ret;
}

atSceneComponent* atSceneNode::Component(int64_t index, const int64_t type) const
{
  int64_t count = 0;
  for (atSceneComponent *pComp : m_components)
    if ((pComp->TypeID() & type) > 0)
    {
      if (count == index)
        return pComp;
      ++count;
    }
    else if (count > index)
    {
      break;
    }

  return nullptr;
}

int64_t atSceneNode::ComponentCount(const int64_t type) const
{
  int64_t count = 0;
  for (atSceneComponent *pComp : m_components)
    count += (pComp->TypeID() & type) > 0;
  return count;
}

bool atSceneNode::RemoveComponent(const int64_t index)
{
  if (index < 0 || index >= m_components.size())
    return false;

  // swap and pop that bad boy
  atDelete(m_components[index]);
  std::swap(m_components[index], m_components.back());
  m_components.pop_back();
  return true;
}

bool atSceneNode::RemoveComponent(const atSceneComponent *pComponent)
{
  for (int64_t i = 0; i < m_components.size(); ++i)
    if (m_components[i] == pComponent)
      return RemoveComponent(i);
  return false;
}

atSceneComponent* atSceneNode::Component(int64_t index) const { return index >= 0 && index < m_components.size() ? m_components[index] : nullptr; }
int64_t atSceneNode::ComponentCount() const { return m_components.size(); }
int64_t atSceneNode::ID() const { return m_pScene->GetNodeID(this); }
atSceneNode* atSceneNode::Root() const { return m_pScene->GetRoot(); }

bool atSceneNode::RemoveChild(const int64_t id, const bool preserveTransform) { return RemoveChild(m_pScene->GetNode(id), preserveTransform); }
bool atSceneNode::AddChild(const int64_t id) { return AddChild(m_pScene->GetNode(id)); }

int64_t atSceneNode::SiblingID(const int64_t index) const { return Sibling(index)->ID(); }
int64_t atSceneNode::ChildID(const int64_t index) const { return Child(index)->ID(); }
int64_t atSceneNode::ParentID()  const { return m_pParent->ID(); }

atVector<int64_t> atSceneNode::SiblingIDs() const { return m_pScene->GetNodeIDs(m_siblings); }
atVector<int64_t> atSceneNode::ChildIDs() const { return m_pScene->GetNodeIDs(m_children); }

atSceneNode* atSceneNode::Sibling(const int64_t index) const { return m_siblings[index]; }
int64_t atSceneNode::ChildCount() const { return m_children.size(); }
int64_t atSceneNode::SiblingCount() const { return m_siblings.size(); }
atSceneNode* atSceneNode::Child(const int64_t index) const { return m_children[index]; }
atSceneNode* atSceneNode::Parent() const { return m_pParent; }

const atVector<atSceneNode*>& atSceneNode::Children() const { return m_children; }
const atVector<atSceneNode*>& atSceneNode::Siblings() const { return m_siblings; }

// Transformations and Matrices

atMat4D atSceneNode::GlobalTranslationMat() const { return TranslationMat() * ParentTranslationMat(); }
atMat4D atSceneNode::GlobalRotationMat() const { return RotationMat() * ParentRotationMat(); }
atMat4D atSceneNode::GlobalScaleMat() const { return ScaleMat() * ParentScaleMat(); }
atMat4D atSceneNode::GlobalWorldMat() const { return WorldMat() * ParentWorldMat(); }

atVec3D atSceneNode::GlobalPosition() const { return ParentWorldMat() * m_translation; }
atVec3D atSceneNode::GlobalRotation() const { return ParentRotation() + m_rotation; }
atVec3D atSceneNode::GlobalScale() const { return ParentScale() * m_scale; }

atScene* atSceneNode::Scene() { return m_pScene; }

atVec3D atSceneNode::ParentPosition() const { return Parent() ? Parent()->GlobalPosition() : 0; }
atVec3D atSceneNode::ParentRotation() const { return Parent() ? Parent()->GlobalRotation() : 0; }
atVec3D atSceneNode::ParentScale() const { return Parent() ? Parent()->GlobalScale() : 1; }

atMat4D atSceneNode::ParentTranslationMat() const { return Parent() ? Parent()->GlobalTranslationMat() : atMat4D::Identity(); }
atMat4D atSceneNode::ParentRotationMat() const { return Parent() ? Parent()->GlobalRotationMat() : atMat4D::Identity(); }
atMat4D atSceneNode::ParentScaleMat() const { return Parent() ? Parent()->GlobalScaleMat() : atMat4D::Identity(); }
atMat4D atSceneNode::ParentWorldMat() const { return Parent() ? Parent()->GlobalWorldMat() : atMat4D::Identity(); }
