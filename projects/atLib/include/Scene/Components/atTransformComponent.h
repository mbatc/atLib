#ifndef atTransformComponent_h__
#define atTransformComponent_h__

#include "atTransformable.h"
#include "atSceneComponent.h"

class atTransformComponent : public atTransformable<double>, atSceneComponent
{
public:
  atTransformComponent(const atString &name = "");

  // Get the scale matrix
  Mat GlobalScaleMat() const;

  // Get the translation matrix
  Mat GlobalTranslationMat() const;

  // Get the transformation matrix. This matrix consists of the following
  // transformations:
  //
  //  - Translation by -pivot
  //  - Non-Uniform Scale by m_scale
  //  - Rotation by m_rotation
  //  - Translation by m_translation
  //
  //  Applied in the described order
  Mat GlobalTransformMat() const;

  // Get the rotation as euler angles (x-pitch, y-yaw, z-roll)
  Vec GlobalRotationEuler() const;

  // Get the rotation as a quaternion
  const Quat GlobalOrientation() const;

  // Get the scale
  const Vec GlobalScale() const;

  // Get the translation
  const Vec GlobalTranslation() const;

  // Get the rotation matrix
  Mat GlobalRotationMat() const;

  atSceneComponent* Copy() const override;

  static atCoreComponentType TypeID();
};

#endif // atTransformComponent_h__