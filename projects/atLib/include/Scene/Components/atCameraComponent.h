#ifndef atCameraComponent_h__
#define atCameraComponent_h__

#include "atTransformable.h"
#include "atSceneComponent.h"
#include "atCamera.h"

class atCameraComponent : public atSceneComponent, public atProjection
{
public:
  atCameraComponent(const atString &name = "");

  static atCoreComponentType TypeID();

  atMat4D ViewMat() const;
  atMat4D ViewProjMat() const;

  atSceneComponent* Copy() const override;

protected:

};

#endif // atTransform_h__