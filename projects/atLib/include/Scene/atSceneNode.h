
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

#ifndef atSceneEntity_h__
#define atSceneEntity_h__

#include "atString.h"
#include "atTransformable.h"
#include "atSceneComponent.h"

class atScene;

class atSceneNode : public atTransformable<double>
{
  friend atScene;

public:
  atSceneNode();

  const atString& Name();
  void SetName(const atString &name);

  virtual bool Update(const double dt);
  virtual bool Draw(const atMat4D &vp);
  
  int64_t ID() const;

  atSceneNode* Root() const;
  atSceneNode* Parent() const;
  const atVector<atSceneNode*>& Children() const;
  const atVector<atSceneNode*>& Siblings() const;

  atSceneNode* Child(const int64_t index) const;
  atSceneNode* Sibling(const int64_t index) const;

  int64_t ChildCount() const;
  int64_t SiblingCount() const;

  int64_t ParentID() const;
  int64_t ChildID(const int64_t index) const;
  int64_t SiblingID(const int64_t index) const;

  atVector<int64_t> ChildIDs() const;
  atVector<int64_t> SiblingIDs() const;

  bool AddChild(atSceneNode *pChild, const bool preserveTransform = true);
  bool RemoveChild(atSceneNode *pChild, const bool preserveTransform = true);

  bool AddChild(const int64_t id);
  bool RemoveChild(const int64_t id);

  atMat4D GlobalTranslationMat() const;
  atMat4D GlobalRotationMat() const;
  atMat4D GlobalScaleMat() const;
  atMat4D GlobalWorldMat() const;

  atVec3D GlobalPosition() const;
  atVec3D GlobalRotation() const;
  atVec3D GlobalScale() const;

  int64_t ComponentCount(const int64_t type) const;
  atSceneComponent* Component(const int64_t type, int64_t index) const;
  atVector<atSceneComponent*> Components(const int64_t type = atSCT_All) const;

  atSceneComponent* AddComponent(const int64_t type);

protected:
  atVec3D ParentPosition() const;
  atVec3D ParentRotation() const;
  atVec3D ParentScale() const;

  atMat4D ParentTranslationMat() const;
  atMat4D ParentRotationMat() const;
  atMat4D ParentScaleMat() const;
  atMat4D ParentWorldMat() const;

  atString m_name;

  atScene *m_pScene = nullptr;
  atSceneNode *m_pParent = nullptr;
  atVector<atSceneNode*> m_children;
  atVector<atSceneNode*> m_siblings;
  atVector<atSceneComponent*> m_components;
};

#endif // atSceneEntity_h__
