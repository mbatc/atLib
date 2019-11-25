#ifndef atScriptComponent_h__
#define atScriptComponent_h__

#include "atSceneComponent.h"

class atScriptComponent : public atSceneComponent
{
public:
  atScriptComponent(const atString &name);

  static atCoreComponentType TypeID();

  atSceneComponent* Copy() const override;
};

#endif // atScriptComponent_h__
