#ifndef atNode_h__
#define atNode_h__

#include "atVector.h"
#include "atTransformable.h"

class atNode : public atTransformable<double>
{
public:
  atNode* Parent();
  atNode* Root();
  atNode* Child(const int64_t index);
  int64_t ChildCount();
  const atVector<atNode*>& Children();
  
  bool SetParent(atNode *pParent);
  bool RemoveParent();
  bool AddChild(atNode *pChild);
  bool RemoveChild(atNode *pChild);
  bool RemoveChild(const int64_t index);

protected:
  atNode *m_pRoot;
  atNode *m_pParent;
  atVector<atNode*> m_children;
};

#endif // atNode_h__
