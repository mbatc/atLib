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

  virtual bool Update(const double dt);
  virtual bool Draw(const atMat4D &vp);
  
  int64_t ID() const;

  atSceneNode* Root() const;
  atSceneNode* Parent() const;
  const atVector<atSceneNode*>& Children() const;
  const atVector<atSceneNode*>& Siblings() const;

  atSceneNode* Child(const int64_t index) const;
  atSceneNode* Sibling(const int64_t index) const;

  int64_t ParentID(const int64_t index) const;
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

  atVec3F64 GlobalPosition() const;
  atVec3F64 GlobalRotation() const;
  atVec3F64 GlobalScale() const;

  int64_t ComponentCount(const int64_t type) const;
  atSceneComponent* Component(const int64_t type, int64_t index) const;
  atVector<atSceneComponent*> Components(const int64_t type = atSCT_All) const;

protected:
  atVec3F64 ParentPosition() const;
  atVec3F64 ParentRotation() const;
  atVec3F64 ParentScale() const;

  atMat4D ParentTranslationMat() const;
  atMat4D ParentRotationMat() const;
  atMat4D ParentScaleMat() const;
  atMat4D ParentWorldMat() const;

  atScene *m_pScene;
  atSceneNode *m_pParent;
  atVector<atSceneNode*> m_children;
  atVector<atSceneNode*> m_siblings;

  atVector<atSceneComponent*> m_components;
};

#endif // atSceneEntity_h__
