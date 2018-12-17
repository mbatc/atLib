#ifndef atSceneComponent_h__
#define atSceneComponent_h__

#include "atMath.h"

class atSceneNode;

enum atSceneComponentType : int64_t
{
  atSCT_None = 1 << 0,
  atSCT_Mesh = 1 << 1,
  atSCT_Script = 1 << 2,
  atSCT_Camera = 1 << 3,
  atSCT_Collidable = 1 << 4,
  atSCT_All = INT64_MAX
};

class atSceneComponent
{
public:
  virtual bool Update(const double dt) { return true; }
  virtual bool Draw(const atMat4D &vp) { return true; }

  virtual int64_t TypeID() const = 0;

protected:
  atSceneNode *m_pNode;
};

#endif // atSceneComponent_h__
