#ifndef atControlComponent_h__
#define atControlComponent_h__

#include "atSceneComponent.h"

class atControlComponent : public atSceneComponent
{
public:
  atControlComponent(const atString &name = "");
  
  bool Update(const double &deltaTime) override;

  static atCoreComponentType TypeID();

  atSceneComponent* Copy() const override;

protected:
  atQuatD m_yaw;
  atQuatD m_pitch;
};

#endif // atControlComponent_h__
