#ifndef atTransform_h__
#define atTransform_h__

#include "atTransformable.h"
#include "atSceneComponent.h"
#include "atMaterial.h"
#include "atRenderable.h"

class atMeshComponent : public atSceneComponent
{
public:
  atMeshComponent(const atString &name = "");

  bool Init() override;
  bool Update(const double &deltaTime) override;
  bool Render(const atMat4D &vp) override;
  bool Destroy() override;

  atSceneComponent* Copy() const override;

  atMaterial m_material;
  atRenderable m_renderable;

  static atCoreComponentType TypeID();
};

#endif // atTransform_h__