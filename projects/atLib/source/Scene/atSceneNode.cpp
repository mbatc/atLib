
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
#include "atCamera.h"
#include "atSkybox.h"
#include "atMeshRenderable.h"

atSceneNode::atSceneNode() {}

const atString &atSceneNode::Name() { return m_name; }
void atSceneNode::SetName(const atString &name) { m_name = name; }

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
  for (atSceneNode *&pNode : m_children)
    if (pNode == pChild)
    {
      m_children.erase(&pNode - m_children.begin());
    }
    else
    {
      for(atSceneNode *&pSibling : pNode->m_siblings)
        if (pSibling == pChild)
        {
          pNode->m_siblings.erase(&pSibling - pNode->m_siblings.begin());
          break;
        }
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
    if((pComp->TypeID() & type) > 0)
      ret.push_back(pComp);
  return ret;
}

atSceneComponent *atSceneNode::AddComponent(const int64_t type)
{
  atSceneComponent *pComponent = nullptr;
  switch (type)
  {
  case atSCT_Camera: pComponent = (atSceneComponent*)atNew<atCamera>(); break;
  case atSCT_MeshRenderable: pComponent = (atSceneComponent*)atNew<atMeshRenderable>(); break;
  case atSCT_Script: pComponent = nullptr; break;
  case atSCT_Collidable: pComponent = nullptr; break;
  case atSCT_Effect: pComponent = nullptr; break;
  case atSCT_Skybox: pComponent = (atSceneComponent*)atNew<atSkybox>(); break;
  }
  if (!pComponent) 
    return nullptr;

  pComponent->m_pNode = this;
  
  m_components.push_back(pComponent);
  return pComponent;
}

int64_t atSceneNode::ID() const { return m_pScene->GetNodeID(this); }

atSceneNode* atSceneNode::Root() const { return m_pScene->GetRoot(); }

bool atSceneNode::RemoveChild(const int64_t id) { return RemoveChild(m_pScene->GetNode(id)); }
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

atVec3D atSceneNode::ParentPosition() const { return Parent() ? Parent()->GlobalPosition() : 0; }
atVec3D atSceneNode::ParentRotation() const { return Parent() ? Parent()->GlobalRotation() : 0; }
atVec3D atSceneNode::ParentScale() const { return Parent() ? Parent()->GlobalScale() : 1; }

atMat4D atSceneNode::ParentTranslationMat() const { return Parent() ? Parent()->GlobalTranslationMat() : atMat4D::Identity(); }
atMat4D atSceneNode::ParentRotationMat() const { return Parent() ? Parent()->GlobalRotationMat() : atMat4D::Identity(); }
atMat4D atSceneNode::ParentScaleMat() const { return Parent() ? Parent()->GlobalScaleMat() : atMat4D::Identity(); }
atMat4D atSceneNode::ParentWorldMat() const { return Parent() ? Parent()->GlobalWorldMat() : atMat4D::Identity(); }
