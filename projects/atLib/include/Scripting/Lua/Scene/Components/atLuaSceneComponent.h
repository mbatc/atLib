#ifndef atLuaSceneComponent_h__
#define atLuaSceneComponent_h__

#include "atSceneComponent.h"

class atLuaSceneComponent
{
public:
  atLuaSceneComponent(atSceneComponent *pComp = nullptr);
  atLuaSceneComponent(const atLuaSceneComponent &copy);
  atLuaSceneComponent(atLuaSceneComponent &&move);
  ~atLuaSceneComponent();

  int64_t TypeID() const;

protected:
  atSceneComponent *m_pComponent;
};

#endif // atLuaSceneComponent_h__
