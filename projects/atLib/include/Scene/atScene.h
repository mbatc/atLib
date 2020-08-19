#ifndef atScene_h__
#define atScene_h__

#include "atString.h"
#include "atHierarchy.h"
#include "atSceneNode.h"

class atScene
{
public:
  atScene();
  ~atScene();



protected:
  atString m_name;
  atHierarchy<atSceneNode*> m_hierarchy;
};

#endif // atScene_h__
